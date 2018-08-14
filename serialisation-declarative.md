
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


### Structure to/from tagged structure

{.ednote} This handles tags, identifiers/pointers, SCHMA;
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
- a (possibly empty) sequence of *contained tagged substructures*
- optionally, a *payload*, which is a *string*

The conversion between *structures* and *tagged structures* makes use of the *serialisation schema*
which is a *tagged structure* with *tag* `SCHMA` contained in the *header*.

#### Serialisation Schema {#schema}

The *serialisation schema* contains the set of *structure type identifiers* in the data set,
their inheritance relationships,
and the *tags* used to represent each.
It is represented as a *pseudo-structure* with *tag* `SCHMA` in the *header*.
Its contents may be represented in two ways:
by *data set IRI* or by *structure type description*.
The *serialisation schema* is the set of all provided *structure type descriptions*
and all *structure type descriptions* defined in each provided *data set IRI*.

##### Data set IRI

A **data set IRI** is denoted by a *tagged substructure* of the *serialisation schema*
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

A **structure type description** is denoted by a *tagged substructure* of the *serialisation schema*
with *tag* `IRI` and payload the *structure type identifier* of a particular *structure type*.

The `IRI` *tagged structure* *must* have a *tagged substructure* for each of its direct *supertypes*
(excepting than `elf:Structure`),
each with *tag* `ISA` and *payload* of the *structure type identifier* of the *supertype*.
Each *supertype* defined in an `ISA` *tagged structure* *must* also be defined in the *serialisation schema*.

{.example ...} The `[elf:SUBMITTER_RECORD]` has two supertypes
so it would have two `ISA` *tagged substructures*:

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
it *must* have at least one *tagged substructure* with *tag* `TAG`
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
contain `TAG` *tagged substructures* with the same first token in their *payloads*
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
c. contain `TAG` *tagged substructures* with the same first token in their *payloads*

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

{.ednote} To do: add more about pseudo-structures

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
