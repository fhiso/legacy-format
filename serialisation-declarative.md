
{.ednote ...} The goal of this document is to define how to move between adjacent abstraction levels.
My initial list of abstraction levels and their correspondence was quite large:

    bytes
        character set
    characters
        line endings
    lines
        parts
    tagged lines
        levels
    split structures
        cont/conc
    id'd structures
        id
    tagged structures
        SCHMA
    structures

However, I believe several of these can fruitfully be combined.

Following are the sections I have drafted so far.
{/}




### Split structure to/from line

{.ednote} This will handle levels, ordering, CONT/CONC, and escapes.

Each *split structure* has

- optionally, a *cross-reference identifier*
- a *tag*
- a *containing tagged structure*, which may be the *header* pseudo-structure or none
- a (possibly empty) sequence of *contained tagged structures*
- optionally, a *payload*, which is a *string* with no *line breaks*

The *split structures* of within a given *containing split structure* are held in a fixed order.

Each *line* is a sequence of characters containing no line breaks.
All *lines* are held in a fixed order.

For the remainder of this section, let $T'$ represent an arbitrary *split structure*.

The **level** of $T'$ is a non-negative integer, defined recursively as

- 0 if $T$ has no *containing tagged structure*
- 1 + the *level* of $T$'s *containing tagged structure* otherwise

The **line** corresponding to $T'$ is a *string* consisting of the following components concatenated in order:

1. The *level* of $T'$ encoded as a base-10 integer with no leading 0.
2. A single space character (U+0020)
3. If $T'$ has a *cross reference identifier*, then
    a. a single COMMERCIAL AT character (U+0040)
    b. the *cross reference identifier* of $T'$
    c. a single COMMERCIAL AT character (U+0040)
    d. a single space character (U+0020)
4. The *tag* of $T'$
5. If $T'$ has a *payload*, then 
    a. a single space character (U+0020)
    b. the *payload* of $T'$

{.ednote} add about ordering of all lines






### Split Structures

A **split structure** is a *tagged structure* that either (a) has no *payload* or (b) has no *line breaks* in its payload,
where a **line break** is a substring matching the production `LB`:

    LB  ::=  #xD #xA? | #xA

It is *recommended* that the *payload* of a *split structure* be short enough
that its *line* be less than 255 bytes when encoded in UTF-8.

{.note} GEDCOM *required* lines not exceed 255 *characters*;
this does not seem to be a real restriction in most current applications,
and hence has been reduced to *recommended* status.
We recommend bytes in UTF-8 instead of *characters* because the implied 
purpose of this limit (fixed-width buffers) would limit by bytes, not
characters.


#### Splitting

Each *tagged structure* $T$ corresponds to a *split structure* $T'$
that is identical to $T$ except

- $T$ and $T'$ may differ in *payload*
- $T'$ may contain additional *pseudo-structures* not present in $T$.

If $T$ has no *payload*, $T$ and $T'$ are the same.
Otherwise, there differences *must* be equivalent to ones that can be created by the following steps.

Identify a (possibly empty) set of **split points** in the *payload* of $T$.
This set *must* include each *line break*
and *may* include any other position between two characters preceded by a non-*whitespace* character
that is that are not part of a substring matching the `Escape` production.

    Escape  ::= "@#" [^#x40#xA#xD]* "@"

Split points *must* only be placed between between *characters* and
*should not* be placed between two *characters* which represent part
of the same glyph.

{.example}  In the UTF-8 character set, the *character* "é" is
encoded using two bytes `C3 A9`.  A split point *must not* be placed
between these bytes, as doing so would prevent the file from being
a valid UTF-8 document.

{.example}  The glyph "é" can also be encoded using two characters,
a Latin letter "e" (U+0065) followed by a combining acute accent
(U+0301), which are rendered as a single glyph, "é".  An application
*should not* place a split point between these characters.

Initialize $T'$ to be equivalent to $T$ (including with the same *contained tagged structures*)
except it's *payload* is the substring of $T$'s payload preceding the first *split point*.

{.ednote ...} If a note structure's payload begins with a newline,
it is visually encodes as, e.g.,

````gedcom
2 NOTE
3 CONT text
````

It is not clear from GEDCOM if there should be a space after the tag NOTE (and empty-string payload)
or not (no payload).
As currently written, this specification implies the former.
{/}

For each split point identified, add a *pseudo-substructure* to $T'$
with a *payload* of the characters following that split point and preceding the next split point (if any).
If the split point was placed at a *line break*, the *pseudo-structure*'s *tag* is `CONT`;
otherwise it is `CONC`.

{.example ...}
Suppose a structure's *extended line* was *level* = 2, *tag* = `NOTE`, and *payload string* "`This is a test\nwith one line break`".
This *payload string* requires at least one split point (because it contains one *line break*) and may contain more.
It could be serialised in many ways, such as

````gedcom
2 NOTE This is a test
3 CONT with one line break
````

or

````gedcom
2 NOTE This i
3 CONC s a test
3 CONT with on
3 CONC e line break
````
{/}

These *pseudo-structures* *must* be placed before any other *contained tagged structures* of $T'$
and *must* be placed in the same order as the *split points* they represent.

{.example ...} The following arguably has unambiguous meaning, but MUST NOT be created because it violates the `CONT` and `CONC` first rule:

````gedcom
2 SOUR This is a
3 NOTE You cannot put a substructure before a CONT or CONC
3 CONC n illegal ordering
````
{/}


##### Unsplitting

{.ednote} rewrite this

All `CONT` and `CONT` *pseudo-structures* must be removed as part of deserialisation.
The result of such removal *shall* be the same as if the following two steps
were repeatedly applied until not no `CONT` or `CONC` *pseudo-structures* remained:

- If the first *contained tagged structure* $C$ of a $T$ *tag* `CONT`, append to the *payload* of $T$
    a *line break*  followed by the payload of $C$, and remove $C$ from $T$.

- If the first *contained tagged structure* $C$ of a $T$ *tag* `CONC`, append to the *payload* of $T$
    the payload of $C$, and remove $C$ from $T$.





### Structure to/from tagged structure

{.ednote} This handles tags, identifiers/pointers, SCHMA, ordering;
it introduces pseudo-structures and defines "undeclared extension".
It does not address levels, CONT/CONC, escapes, or character sets.

Each *structure* has

- a *structure type*, which has a *structure type identifier*
- one or more *supertypes*; the abstract type *elf:Structure* is used if it is not part of another type hierarchy
- a *superstructure*, which may be the special values `elf:Document` or `elf:Metadata` if the *structure* is not contained in another *structure*
- a (possibly empty) set of *substructures*, where the order to *substructures* with the same *structure type* is known.
- optionally, a *payload*, which is either a *payload string* or  a *pointer* to a *structure* with *superstructure* `eld:Document`

Each *tagged structure* has

- optionally, a *cross-reference identifier*
- a *tag*
- a *containing tagged structure*, which may be the *header* pseudo-structure or none
- a (possibly empty) ordered sequence *contained tagged structures*
- optionally, a *payload*, which is a *string*

The conversion between *structures* and *tagged structures* makes use of the *serialisation schema*
which is a *tagged structure* with *tag* `SCHMA` contained in the *header*.

#### Serialisation Schema {#schema}

{.ednote} To do: add prefix shortening

The *serialisation schema* contains the set of *structure type identifiers* in the data set,
their inheritance relationships,
and the *tags* used to represent each.
It is represented as a *pseudo-structure* with *tag* `SCHMA` in the *header*.
Its contents may be represented in two ways:
by *data set IRI* or by *structure type description*.
The *serialisation schema* is the set of all provided *structure type descriptions*
and all *structure type descriptions* defined in each provided *data set IRI*.

##### Data set IRI

A **data set IRI** is denoted by a *contained tagged structure* of the *serialisation schema*
with *tag* `SCHMA` and payload the IRI of a defined data set.

{.example ...} When using the [ELF-DataModel] version 1.0.0,
the *serialisation schema* could be serialised as

````gedcom
0 HEAD
1 SCHMA
2 SCHMA https://fhiso.org/TR/elf-data-model/v1.0.0
````
{/}

The data set IRI *should* use the `http` or `https` scheme
and an HTTP request sent to it with an `Accept` header of `application/x-fhiso-elf1-schema`
*should* return a document serialised in accordance with this specification
containing a *serialisation schema* defining the full data model in *structure type descriptions*.

##### Structure type description

A **structure type description** is denoted by a *contained tagged structure* of the *serialisation schema*
with *tag* `IRI` and payload the *structure type identifier* of a particular *structure type*.

The `IRI` *tagged structure* *must* have a *contained tagged structure* for each of its direct *supertypes*
(excepting than `elf:Structure`),
each with *tag* `ISA` and *payload* of the *structure type identifier* of the *supertype*.
Each *supertype* defined in an `ISA` *tagged structure* *must* also be defined in the *serialisation schema*.

{.example ...} The `[elf:SUBMITTER_RECORD]` has two supertypes
so it would have two `ISA` *contained tagged structures*:

````gedcom
0 HEAD
1 SCHMA
2 IRI elf:SUBMITTER_RECORD
3 ISA elf:Agent
3 ISA elf:Record
3 TAG SUBM elf:Document
````
{/}

If the `IRI` *tagged structure* defines a type present in the data set,
it *must* have at least one *contained tagged structure* with *tag* `TAG`
and a *payload* consisting of a white space-separated list of at least two tokens.
The first token matches production `Tag`;
the remaining tokens are the *structure type identifiers* of *superstructures*
(or one or more of their *eventual supertypes*) of the *structure type*.

    Tag  ::= [0-9a-zA-Z_]+

{.example ...} The `[elf:SOURCE_CITATION]` has several superstructures,
with the same *tag* in each:

````gedcom
0 HEAD
1 SCHMA
2 IRI elf:SOURCE_CITATION
3 TAG SOUR elf:ASSOCIATION_STRUCTURE elf:Event elf:FAM_RECORD elf:INDIVIDUAL_RECORD elf:PersonalName
````
{/}

It *must not* be the case that two `IRI` *tagged structures*
with distinct *payloads*
contain `TAG` *contained tagged structures* with the same first token in their *payloads*
unless the set of *structure type identifier* tokens in the two `TAG`s,
when extended to include all of their subtypes' identifiers,
are disjoint sets.

{.example ...} The `[elf:SOURCE_CITATION]` and  `[elf:SOURCE_RECORD]` both use the same *tag*:

````gedcom
0 HEAD
1 SCHMA
2 IRI elf:SOURCE_CITATION
3 TAG SOUR elf:ASSOCIATION_STRUCTURE elf:Event elf:FAM_RECORD elf:INDIVIDUAL_RECORD elf:PersonalName
2 IRI elf:SOURCE_RECORD
3 ISA elf:Record
3 TAG SOUR elf:Document
````

Even though these are

a. two `IRI` *tagged structures*
b. with distinct *payloads*
c. contain `TAG` *contained tagged structures* with the same first token in their *payloads*

there is no conflict because `elf:Document` is neither a supertype nor subtype
of any of the structure types where `elf:SOURCE_CITATION` may exist.

If an extension decided that `elf:SOURCE_RECORD`s could also appear inside `elf:SCHOLASTIC_ACHIEVEMENT`s,
there would be a conflict and the data invalid
because `elf:Event` is an *eventual supertype* of `elf:SCHOLASTIC_ACHIEVEMENT`.

````gedcom
0 HEAD
1 SCHMA
2 IRI elf:SOURCE_CITATION
3 TAG SOUR elf:ASSOCIATION_STRUCTURE elf:Event elf:FAM_RECORD elf:INDIVIDUAL_RECORD elf:PersonalName
2 IRI elf:SOURCE_RECORD
3 ISA elf:Record
3 TAG SOUR elf:Document elf:SCHOLASTIC_ACHIEVEMENT
3 NOTE The above is illegal, as it makes tag SOUR ambiguous inside elf:
4 CONC SCHOLASTIC_ACHIEVEMENT: is it a SOURCE_RECORD or a SOURCE_CITATION?
````

{/}

{.ednote} Ugh... this whole section is stilted and confusing. The example help, but...

#### (Un)tagging

Each *structure* corresponds to a *tagged structure* as layed out in this section.
Each *tagged structure* corresponds either to a *structure* as layed out here,
or is a *pseudo-structure*.

**Pseudo-structures** are *tagged structures* used only during serialisation,
having no presence in the underlying data model itself.
This specification makes use of the following *pseudo-structures*:

- The **header**, a *pseudo-structure* with *tag* `HEAD` and no *payload*,
    used to
    
    - mark the beginning of a data set.
    - contain of serialisation-wide *pseudo-structures*.
    - contain *structures* with *superstructure* `elf:Metadata`.

- The **trailer**, a *pseudo-structure* with *tag* `TRLR` and no *payload* or *substructures*,
    used to make the end of a data set.

- The `CHAR` *substructure* of the *header*, used to identify the character encoding in use.

- The *serialisation schema* substructure see  of the *header*
    and all of its contained *tagged structures*,
    used to identify serialisation particulars.
    These are described in more detail in {§schema}.
    

- `CONC` and `CONT`, used to encode long and multi-line *payloads*.


For the remainder of this section, let $S$ represent an arbitrary *structure* and $T$ represent its corresponding *tagged structure*.

##### Identifiers, pointers, and payloads

If $S$'s *superstructure* is `elf:Document`, $T$ has a *cross-reference identifier* matching production `ID`:

    ID  ::= [0-9A-Z_a-z] [#x20-#x3F#x41-#x7E]*

{.note} The production `ID` is an alphanum followed by any number of non-control non-`@` 7-bit ASCII characters.
This is more limited than GEDCOM (which also allows the non-ASCII characters in ANSEL) as support for non-ASCII `ID`s is not consistently implemented.
Implementations are *recommended* to support parsing a larger set of identifiers, but *must not* output them.

{.note ...} Some software have traditionally made additional assumptions of identifiers, such as
 
-   identifier strings are durable, expressing historical identity
-   having all `elf:NOTE_RECORD`'s identifiers match `N[1-9][0-9]*`
-   using a specific custom identifier for the `elf:INDIVIDUAL_RECORD` representing the user

... and so on.
While implementations are welcome to use such patterns themselves, they MUST NOT rely on them being true in imported data.
{/}

Two distinct *tagged structures* in a single *dataset* *must not* have the same *cross-reference identifier*.

If $S$'s *superstructure* is not `elf:Document`, $T$ *should not* have a *cross-reference identifier*
and if it does that *cross-reference identifier* has no meaning.

When serialising, 

- a *cross-reference identifier* *must* be generated for each *substructure* of `eld:Document`
- a *cross-reference identifier* *must not* be generated for any other *substructure*

When deserialising, 

- a missing *cross-reference identifier* *shall* be treated as if it were an unspecified unique string matching production `ID`
- an unexpected *cross-reference identifier* *must* be ignored

If $S$'s *payload* is a *pointer*,
then $T$'s *payload* is the *cross-reference identifier* of the *tagged structure* corresponding to the *structure* pointed to by $S$'s payload, surrounded by U+0040 (`@`).
If $T$'s *payload* begins and ends with U+0040 (`@`) an the substring in between matches production `ID`
then $S$'s *payload* is a *pointer* to the *structure* corresponding to the *tagged structure* with that *cross-reference identifier*.
In all other cases, $S$ and $T$ have the same *payload*.

{.note ...} Implicit in the above identifier:pointer conversion is the fact
that in any compliant dataset all pointers point to top-level structures
and in any compliant serialisation all ID payloads have a matching top-level
structure to point to. If these are not true, the data is non-compliant and
the implementation may abort or proceed in an implementation-defined manner.

GEDCOM provided reserved syntax for more flexible pointers, but not semantics
for them. We are unaware of implementations making used of this reserved
syntax and have thus not included it in ELF.
{/}

##### Tags

The *tag* of $T$ corresponds to the *structure type identifier* of $S$,
with dependency on $S$'s *superstructure*'s *structure type identifier*.
This correspondence is recorded in the *serialisation schema* as defined in {§schema}.

The *tag* of $T$ is the first token of the `TAG` *tagged structure*
whose `IRI` *superstructure*'s *payload* is $S$'s *structure type identifier*,
where the *structure type identifier* of the *superstructure* of $S$,
or one of that *superstructure*'s *eventual supertypes* is also a token in the `TAG`'s *payload*.
If more than one such *tag* exists, the first *should* be used.

The *structure type identifier* of $S$ is the *payload* of the `IRI` *tagged structure*
which as a `TAG` substructure whose first token is the *tag* of $T$,
where the *structure type identifier* of the *superstructure* of $S$,
or one of that *superstructure*'s *eventual supertypes* is also a token in the `TAG`'s *payload*.
If more than one such *structure type identifier* exists,
the data is not invalid.

If no *structure type identifier* for $S$ is defined by the *serialisation schema*,
then $T$ is an **undefined extension structure**.
Implementations *should* preserve $T$ as-is, but *may* discard it from the data set.
They *must not* edit it in any other way.

##### Containment

The *containing tagged structure* of $T$ is 

- none if $S$'s *superstructure* is `elf:Document` or if $T$ is the *header*
- the *header* if $S$'s *superstructure* is `elf:Metadata`
- the *tagged structure* corresponding to the *superstructure* of $S$ otherwise

The *superstructure* of $S$ is

- `elf:Document` if $T$ has not *containing tagged superstructure*
- `elf:Metadata` if $T$'s *containing tagged superstructure* is the *header*
- the *structure* corresponding to the *containing tagged superstructure* of $T$ otherwise


The *substructures* of $S$ are the *structures* corresponding to the *contained tagged structures* of $T$.
*Substructures* with the same *structure type* are preserved in the same order as their corresponding *contained tagged structures*.

The *contained tagged structures* of $T$ are the *tagged structures* corresponding to the *substructures* of $S$,
together with any *pseudo-substructures* of $T$.
*Contained tagged structures* with the same *tag* are preserved in the same order as their corresponding *substructures*.
The order of *dontained tagged structures* with distinct *tags* may be selected arbitrarily,
although some *pseudo-structures* impose additional ordering constraints.

