...
title: ELF Serialisation
subtitle: declarative redux pre-alpha draft
---

## Intermediate forms

Serialisation and deserialisation are described in this section using a set of intermediate datastructures.

The set of datastructures used is as follows:

Structure
:   Consisting of

    - A **structure type identifier**, which is a *term*
    - At most one of the following **payload** types:
        - A **pointer** to another *structure*, which *must* be a *record* within the same *dataset*.
        - A *string* or subtype thereof.
    - At most one **superstructure**, which is another *structure* if present.
    - Any number of **substructures**, which are *structures*.
        If a *structure* contains more than one *substructure* with the same *structure type*,
        those *substructures* are stored in a specific order.
        However, the order of *substructures* with different *structure types* is not preserved by their *superstructure*.
    
    A *structure* that has no *superstructure* (or equivalently, one that is said to have the non-*structure* placeholder `elf:Document` as its *superstructure*) is called a *record*.

{.ednote} To do: compare that definition of *record* and `elf:Document` with [Elf-DataModel]... I'm not sure they match.

Tagged Structure
:   Consisting of

    - A *tag*, a *string* matching production `Tag`
    - Optionally, an *xref_id*
    - At most one of the following payloads:
        - The *xref_id* of another *tagged structure*
        - A *string* or subtype thereof.
    - At most one *superstructure*, which is another *tagged structure* if present.
    - A (possibly empty) sequence of *substructures*, which are *tagged structures*.

    A *tagged structure* that has no *superstructure* is said to correspond to a *record*.


Extended Line
:   Consisting of

    1. A *level*, a non-negative *integer*
    2. Optionally, an *xref_id*, a *string* matching production `XrefID`
    3. A *tag*, a *string* matching production `Tag`
    4. Optionally, a **payload string**, which is an arbitrary *string* and may contain *line-breaks*.

Line
:   Consisting of 
    
    1. A *level*, a non-negative *integer*
    2. An optional *xref_id*
    3. A *tag*, a *string* matching production `Tag`
    4. An optional **payload line**, which is a *string* containing any number of *characters*, but *must not* contain a *line-break*.

Serialised Line
:   Consisting of a *string* matching the production `ELFLine`:

        ELFLine ::= Level (S XrefID)? Tag (S (XrefID | Payload))?
        Level   ::= 0 | [1-9][0-9]*
        XrefID  ::= "@" ID "@"
        ID      ::= [0-9A-Z_a-z] [#x20-#x3F#x41-#x7E]*
        Tag     ::= [0-9a-zA-Z_]+
        Payload ::= ([^#xD#xA#x40] | "@@" | Escape)*
        Escape   ::= "@#" EscType EscText "@ "
        EscType  ::= [A-Z]
        EscText  ::= [^#xA#xD#x40]*


A **serialised ELF document** is a *string* consisting of multiple *serialised lines* separated by *line-breaks*.
An **ELF document** is an `elf:Document` containing *structures*, as outlined more fully in [ELF-DataModel].

{.ednote ...} This set of structures was selected to provide a single home for each of the major steps of serialisation:

High-level          Low-level           Conversion handles these concepts
---------------     ---------------     ------------------------------------------------
*structure*         *tagged structure*  *structure type identifier*/*tag*; *pointer*/*xref_id*
*tagged structure*  *extended line*     *level*/*substructure*; escapes
*extended line*     *line*              *line-break*/"`CONT`"; splitting with "`CONC`"
*line*              *serialised line*   

Additionally, having named intermediate forms allows us to easily describe elements that do not manifest as *structure*s in the data model, such as the header being defined as a *tagged structure*.
{/}

## Serialised representations

The following subsections define how data is represented in the next intermediate datastructure.

### Document

An *ELF document* is represented as a sequence of at least two *tagged structures*.
The first *tagged structure* in the sequence is the *[Header]*.
The last *tagged structure* in the sequence is the **trailer**, which has *level* 0, *tag* "`TRLR`", an no *xref_id*, payload, or substructures.
In between these two, in arbitrary order, is a *tagged structure* corresponding to each of the *structures*  in the *ELF document*.

{.note} ELF requires that *substructures* with the same *tag* or *structure type identifier* are preserved in order. It makes no such requirement on *structures* that have no *superstructure*, and applications *should not* rely upon or assign meaning to the order of *tagged structures* in a serialisation.

### Structure

A *structure* is represented as a single *tagged structure*, as defined by the following table:

*structure* component       *tagged structure* component    Notes
--------------------------- ------------------------------- ---------------------------------------------
*structure type identifier* *tag*                           Correspondence determined by the [ELF Schema]
no correspondence           *xref_id*                       see [xref_id] below
*pointer* *payload*         *xref_id* payload               see [xref_id] below
*string* *payload*          *string* payload                same *string* in both
*superstructure*            *superstructure*                
*substructures*             *substructures*                 total order for *tagged structure*, partial order for *structure*

: Correspondence between *structure* and *tagged structure*.

During serialisation, the order of *substructures* of a *tagged structure* MAY be picked arbitrarily, subject to the limitation that it MUST preserve the relative order of *substructures* with the same *structure type identifier* that was provided by the *structure*.
It is RECOMMENDED that serialisations place all *substructures* with the same *tag* together in this order, but doing so is NOT REQUIRED.
Applications MUST NOT attach significance to the order of *tagged structures* with different *tags*.


#### xref_id

The *tagged structure* corresponding to a *structure*

- MUST have an *xref_id* if the corresponding *structure* is pointed to by the *payload* of another *structure*
- SHOULD have an *xref_id* if it has no *superstructure*
- SHOULD NOT have an *xref_id* otherwise

{.ednote ...} [GEDCOM 5.5.1] is ambiguous as to whether records may exist without *xref_ids* and whether *substructures* may have *xref_ids*. For maximal compatibility, we could change the above to something like:

- During serialisation, *conformant* applications MUST give *xref_ids* to all *structures* that have no *superstructures* and MUST NOT give *xref_ids* to any *structure* that has a *superstructure*.
- During deserialisation, *conformant* applications MUST accept structures both with and without *xref_ids* anywhere a *structure* is allowed.
{/}

During serialisation, applications may select any *xref_id* that matches production `XrefID`,
but MUST ensure that no two *structures* contained within a single dataset have the same *xref_id*.

A *tagged structure* $X$ with an *xref_id* payload *must* have a payload that is the *xref_id* of another *tagged structure* $Y$. The *structure* corresponding to $X$ has a *pointer* payload, pointing to the *structure* that  corresponds to $Y$.

#### Deserialisation error recovery

If two or more *tagged structures* are encountered that have the same *xref_id*, a *conformant* application *may* do any of the following:

- Reject the entire dataset as invalid.
- Reject the all of the *tagged structures* with that *xref_id*, and all *tagged structures* with that *xref_id* as their payload, as invalid. See [Rejecting subparts of a dataset] for what this means.
- If no *tagged structures* uses that *xref_id* as a payload, treat all *tagged structures* with that *xref_id* as if they had no *xref_id*.
- If only one of the *tagged structures* with that *xref_id* lacks a *superstructure*, treat all *tagged structures* with that *xref_id* that have a *superstructure* as if they had no *xref_id*.

### Tagged Structure

A *tagged structure* is represented as a non-empty ordered sequence of *extended lines*.

The first *extended line* in this sequence is said to be the *extended line* that corresponds to the *tagged structure*.
It has the same *xref_id* and *tag* as its corresponding the *tagged structure*.
It's *level* is 0 if the *tagged structure* corresponds to a *record*;
otherwise it is 1 greater than the *level* of its *superstructure*.
The conversion of *payloads* is described in [Payload conversion].

The remainder of the sequence of *extended lines* representing a *tagged structure* is a concatenation of the sequences of *extended lines* corresponding to each of the *tagged structure*'s *substructures*, in the same order they have in the sequence of *substructures*.

{.note} The above definitions mean that the *superstructure* of the *tagged structure* corresponding to an *extended line* is the *tagged structure* corresponding to the nearest preceding *extended line* with a *level* exactly one less than the *extended line* in question.

#### Payload conversion

A *tagged structure* payload that is an *xref_id* (or equivalently, one that matches production `XrefID`) is preserved unchanged as the corresponding *extended line*'s *payload string*.

Otherwise,

-   Each U+0040 in the *tagged structure*'s payload
    is represented as two consecutive U+0040 in the *extended line*'s *payload string*
    (i.e., each `@` is represented as `@@`),
    unless the *extended line*'s (equivalently, *tagged structure*'s) *tag*
    is an *escape-preserving tag*
    and the U+0040 is part of a substring matching the production `Escape`
    with an `EscType` in the *escape-preserving tag*'s list of *preserved escape types*.

-   Any *character* MAY be represented with a **unicode escape** consisting of:

    1.  The three characters U+0040, U+0023, and U+0055 (i.e., "`@#U`")
    2.  A hexadecimal encoding of the *character*'s *code point*
    3.  The two characters U+0040 and U+0020 (i.e., "`@ `")
    
    A *unicode escape* MUST be used for each *character* that cannot be encoded
    in the target *character encoding*;
    SHOULD be used if the *character* will begin or end a *payload line*
    after the *extended line* is converted to the *lines*;
    and SHOULD NOT be used otherwise.

These replacements MUST NOT be applied to one another's results.

{.example} The payload "`example@fhiso.com`" is represented as the *payload string* "`example@@fhiso.org`"

{.example} The payload "`☺`" may be represented in a *payload line* as "`☺`" or "`@#U263A@ `" or "`@#U263a@ `". The former SHOULD be used unless serialising to a *character encoding* (such as ASCII) that lacks a representation for U+263A, in which case one of the other two MUST be used instead.

#### Deserialisation error recovery

During deserialisation, if a U+0040 is encountered in a *payload string* which could not have been generated by the representation rules above, the *payload string* SHALL be handled as follows:

- If a substring matches production `Escape` except that it lacks the final space (U+0020), it SHALL be treated as if that space was present.
- If a substring matches production `Escape` but is neither a *unicode escape* nor a *preserved escape type*, it SHALL be removed from the *string*.
- Otherwise, each U+0040 SHALL be left unchanged.

{.example} The *payload string* "`@#DGREGORIAN@1980`" SHALL be treated the same as *payload string* "`@#DGREGORIAN@ 1980`"

{.example} The *payload string* "`@#DGREGORIAN@ 1980`" SHALL be treated the same as *payload string* "`1980`" unless it is part of an *extended line* with an *escape-preserving tag* and `D` is in the *escape-preserving tag*'s list of *preserved escape types*.

{.ednote ...} There is a (somewhat obscure) problem with the above rules.
Consider the *escape-preserving tag* `DATE`.
A serialisation/deserialisation sequence applied to the *string* "`@@#Dx@@ yz`" yields

1. *string* "`@@#Dx@@ yz`"
2. *payload line* "`@#Dx@ yz`"
3. *string* "`@#Dx@ yz`" -- not re-encoded because it matches a date escape

An alternative is to say that *escape-preserving tags* do not perform any replacements,
leaving `@@` as `@@`. That may possibly complicate the handling of *escape-preserving tags* in the data model.
{/}

If an *extended line* is encountered that has a *level* more than 1 greater than the preceding *extended line*, a *conformant* application *may* do any of the following:

- Reject the entire dataset as invalid.
- Reject that *extended line*, and all subsequent *extended lines* with a *level* no more than one greater than the *level* of the preceding *extended line*, as invalid. See [Rejecting subparts of a dataset] for what this means.

A *conformant* application MUST NOT adjust the *level* of an *extended line* during deserialisation, even if it believes it knows the *level* is in error.

{.example ...} If an application encounters the following invalid dataset

````gedcom
0 HEAD
1 GEDC
2 VERS 5.5.1
2 ELF 1.0.0
2 FORM LINEAGE-LINKED
1 CHAR UTF-8
0 @pepin@ INDI
1 NAME Pepin the Short
1 @charlemagne@ INDI
3 NAME Charlemagne
2 BIRT
3 DATE @#DJULIAN@ BET 2 APR 742 AND 2 APR 748
0 TRLR
````

it *may* reject the dataset in its entirety or it *may* reject the line `3 NAME Charlemagne`{.gedcom}.
It *must not* change the line `1 @charlemagne@ INDI` to `0 @charlemagne@ INDI` even if it knows that `INDI` is the *tag* corresponding to a *structure type identifier* that only appears in *records*.
{/}


### Extended Line

An *extended line* is represented as a non-empty ordered sequence of *lines*.

The first *line* in this sequence has the same *level*, *xref_id*, and *tag* as the *extended line* it represents.
Each subsequent *line*, if any, has a 1-higher *level* , no *xref_id*, and either "`CONT`" or "`CONC`" as its *tag*.

{.note} Because an *extended line* MUST NOT have either "`CONC`" or "`CONT`" as its *tag*, it is unambiguous which *lines* correspond to a single *extended line*.

If there is only one *line* in the sequence, its *payload line* is the same as the *payload string* of the *extended line*. The sequence MUST be a single *line* if there is no *payload string* or if the entire *payload string* matches the production `XrefID`. A multi-*line* sequence MUST be used if the *payload string* contains one or more *line-break*s. Otherwise, an *extended line* MAY be represented by one or more *lines*.

When multiple *lines* are in the sequence, the *payload string* is the concatenation of the *payload lines* of the *lines*, with a *line-break* inserted during concatenation before *payload line* of each *line* that has the *tag* "`CONT`".
Because a *payload line* MAY NOT contain a *line-break*, there MUST be exactly one "`CONT`"-*tagged* *line* in the sequence per *line-break* in the *payload string*.
It is RECOMMENDED that there be sufficient "`CONC`"-*tagged* *lines* that no *serialised line* is longer than 255 bytes when encoded in UTF-8.

{.note} GEDCOM *required* that *serialised lines* not exceed 255 *characters*;
this does not seem to be a real restriction in most current applications,
and hence has been reduced to *recommended* status.
We recommend bytes in UTF-8 instead of *characters* because the implied 
purpose of this limit (enabling code to use fixed-width buffers)
would limit by bytes, not characters.

{.ednote} Should we add something like "A "`CONC`"-*tagged* *line*'s *payload line* SHOULD NOT be empty, nor begin nor end with *whitespace*."? [GEDCOM 5.5.1] does not require this but does imply that it is good practice.

During serialisation, each *line* in the sequence MUST NOT contain an odd number of "`@`" characters (U+0040, COMMERCIAL AT).
However, deserialisation MUST NOT take any particular action if this constraint was not followed, concatenating *lines* into the *extended line* without regard to the number of "`@`" characters in each *line* or in the resulting *extended line*.

{.note} [GEDCOM 5.5.1] gives no guidance how to handle unmatched "`@`", but they are relatively common in gedcom files. The above policy, combined with the handling when deserialising *extended lines* into *tagged structures*, is intended to resolve common invalid gedcom files in an intuitive way.

{.example ...} The following invalid gedcom. 

    1 EMAI name@example.com
    2 DATE @#DGREG
    3 CONC ORIAN@ 2 JAN 2019

will be concatentated into the extended lines

    1 EMAI name@example.com
    2 DATE @#DGREGORIAN@ 2 JAN 2019

creating a valid date escape in the `DATE`-*tagged* *extended line*.
The unmatched `@` in the `EMAI`-*tagged* *extended line* will be left unchanged when deserialising into a *tagged structure*.

Upon reserialisation, the unmatched @ in the "`EMAI`" will be doubled when converting to an *extended line*,
but the date escape will not be modified

    1 EMAI name@@example.com
    2 DATE @#DGREGORIAN@ 2 JAN 2019

If the serialisation decides to split either *extended line* with `CONC`s, it MUST NOT do so
in a way that aplits up the pairs of "`@`"s.
{/}

{.ednote} Previous drafts of this document defined "`CONT`" and "`CONC`" as a formalised construct called a "pseudo-structure" and have provided algorithm outlines for selecting "split points" during serialisation. I think this draft is equally unambiguous while being more descriptive rather than process-oriented.

### Line

A *line* is represented as a *string* by concatentating the following, in order:

1. The *level* encoded as a base-10 integer with no leading zeros, followed by a *delimiter* which SHOULD be a single space U+0020.
1. If present, the *xref_id*, followed by a *delimiter* which SHOULD be a single space U+0020.
1. The *tag*.
1. If present, the *payload line*, preceded by a single space U+0020.

#### Deserialisation error recovery

During deserialisation, if a line is encountered that cannot be parsed as a *line*, but which has a *level*, a *conformant* application *may* do any of the following:

- Reject the entire dataset as invalid.
- Reject that line as invalid. See [Rejecting subparts of a dataset] for what this means.

## Header


### ELF Schema

### Escape-preserving tags


## Rejecting subparts of the dataset




