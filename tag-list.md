---
title: "Extended Legacy Format (ELF)"
subtitle: Lineage-Linked Form
date: 17 February 2017
numbersections: true
...
This is an early exploratory draft of a proposed FHISO standard that is fully compatible with GEDCOM 5.5.
Goals include

-   Clarifying ambiguities
-   Updating presentation
-   Providing extension mechanisms suitable for anticipated FHISO extensions
-   Creating a readily available standard, as GEDCOM 5.5 is no longer available from its authoring corporation.
-   Codifying current practice.

The working title of this draft proposed standard is the FHISO Extended Legacy Format Data Model,
also known as the ELF data model
or ELF-mod.

This document attempts to list the known ELF tags, their contexts and substructures.
Other documents describe requisite terminology, serialization, etc.

This draft is intended to provide enough context for meaningful discussion of the value and desirable content of a potential future standard.
It will likely change significantly prior to being released.

----

# Notation    {#Notation}

This document uses tag names instead of URIs.
A mapping between tag name and URI is expected as part of a separate document or a future extension to the document.

A **structure** is either a *record* or a *substructure*.

**Records** appear at the top-level of an ELF dataset.
Known records types are

-   `[HEAD]` -- each data set has exactly one of these, at the beginning of the dataset.
-   `[TRLR]` -- each data set has exactly one of these, at the end of the dataset.
-   `[SUBN]` -- each data set has zero or one of these; if present, it immediately follows the `[HEAD]` record.
-    `[FAM]`, `[INDI]`, `[OBJE]`, `[NOTE]`, `[REPO]`, `[SOUR]`, and `[SUBM]` -- may be omitted or may appear in any number and any order.

**Substructures** appear nested within other structures.

The **context** of a structure specifies where it appears.  The following notation is used to define structure contexts:

-   `TAGNAME` matches any structure or pointer with that tag, anywhere it appears
-   `.TAGNAME` matches any record with that tag, but not substructures
-   *context specifier*`..TAGNAME` matches any structure or pointer with that tag, provided it is nested under a structure specified by the *context specifier*

    {.ednote} we do not currently use the `..` syntax in this document
-   *context specifier*`.TAGNAME` matches any structure or pointer with that tag, provided it is a substructure of a structure specified by the *context specifier*
-   `(TAGNAME)` refers to any structure with that tag name or any tag inherited from it.

{.ednote} We might also want to define `@TAGNAME` to refer to any structure with that tag name that has, as its payload, a pointer; and `#TAGNAME` to refer to any structure with that tag name that does not have, as its payload, a pointer; these might be necessary to discuss different kinds of `OBJE` and `SOUR` tags that cannot be distinguished from context alone.

The **cardinality** of a substructure specifies with what plurality it appears within its containing structure.  Cardinality is specified by adding one of the following suffixes to a tag name:

- `!` means "required": there must be one and only one of these.
- `?` means "optional": there may be one and no more than one of these.
- `*` means "set-valued": there may be zero or more of these.  Some implementations may provide a maximum number for some or all set-values substructures.
- `+` means "non-empty set-valued": there must be one and may be more than one of these. In the current spec, `+` only appears for the .`[OBJE]`.`[FILE]`.

# Supertypes

A limited type hierarchy is known to exist:

- `[TopLevel]`
    -   `[HEAD]`
    -   `[SUBN]`
    -   `[TRLR]`
    -   `[Record]`
        -   `[FAM]`
        -   `[INDI]`
        -   `[OBJE]`
        -   `[NOTE]`
        -   `[REPO]`
        -   `[SOUR]`
        -   `[SUBM]`
- `[Event]`
    -   `[IndividualEvent]`
        - `[ADOP]`
        - `[BAPM]`
        - `[BARM]`
        - `[BASM]`
        - `[BIRT]`
        - `[BLES]`
        - `[BURI]`
        - `[CENS]`
        - `[CHR]`
        - `[CHRA]`
        - `[CONF]`
        - `[CREM]`
        - `[DEAT]`
        - `[EMIG]`
        - `[FCOM]`
        - `[GRAD]`
        - `[IMMI]`
        - `[NATU]`
        - `[ORDN]`
        - `[PROB]`
        - `[RETI]`
        - `[WILL]`
        - `[EVEN]` -- note that there is also an `[EVEN]` that is a subtype of `[FamilyEvent]`
    -   `[FamilyEvent]`
        - `[ANUL]`
        - `[CENS]`
        - `[DIV]`
        - `[DIVF]`
        - `[ENGA]`
        - `[MARR]`
        - `[MARB]`
        - `[MARC]`
        - `[MARL]`
        - `[MARS]`
        - `[EVEN]` -- note that there is also an `[EVEN]` that is a subtype of `[IndividualEvent]`
    -   `[IndividualAttribute]`
        - `[CAST]`
        - `[DSCR]`
        - `[EDUC]`
        - `[IDNO]`
        - `[NATI]`
        - `[NCHI]`
        - `[NMR]`
        - `[OCCU]`
        - `[PROP]`
        - `[RELI]`
        - `[RESI]`
        - `[SSN]`
        - `[TITL]`
        - `[FACT]` -- note that prior to GEDCOM 5.5.1, `[EVEN]` was used in place of `[FACT]`

All other tags in this document have no supertype.

# Known Tags and Contexts

## Abstract Types

The following tags are not encountered directly, but are instead present only as abstract supertypes from which other tags derive.

### TopLevel   {#TopLevel}

The *TopLevel* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for those types that may exist at the top level of a data set.

Known subtypes
:   `[HEAD]`
:   `[SUBN]`
:   `[TRLR]`
:   `[Record]`

Contexts
:   .(`[TopLevel]`)

Description
:   Varies by subtype

Payload
:   Varies by subtype

Substructures
:   Varies by subtype


### Record   {#Record}

The *Record* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for those [TopLevel] types that describe the principle contents of the data set (as opposed to other [TopLevel] subtypes, which are metadata).

Known subtypes
:   `[FAM]`
:   `[INDI]`
:   `[OBJE]`
:   `[NOTE]`
:   `[REPO]`
:   `[SOUR]`
:   `[SUBM]`

Contexts
:   .(`[Record]`)

Description
:   Varies by subtype

Supertype
:   `[TopLevel]`

Payload
:   Varies by subtype

Substructures
:   Varies by subtype


### Event   {#Event}

The *Event* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Known Subtypes
:   `[IndividualEvent]`
:   `[FamilyEvent]`
:   `[IndividualAttribute]`

Contexts
:   `[INDI]`.(`[Event]`)

Description
:   Varies by subtype

Payload
:   Varies by subtype

Substructures
:   `[ADDR]`?
:   `[AGNC]`?
:   `[CAUS]`?
:   `[DATE]`?
:   `[NOTE]`\*
:   `[OBJE]`\*
:   `[PHON]`\*
:   `[PLAC]`?
:   `[SOUR]`\*
:   `[TYPE]`?
:   `[RESN]`?
:   `[RELI]`?

{.note} GEDCOM 5.5 also includes `[AGE]` as a substructure of `[Event]`, but GEDCOM 5.5.1 moves that to `[IndividualEvent]` and `[IndividualAttribute]` instead.

### IndividualEvent   {#IndividualEvent}

The *IndividualEvent* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Supertype
:   `[Event]`

Known Subtypes
:   `[ADOP]`
:   `[BAPM]`
:   `[BARM]`
:   `[BASM]`
:   `[BIRT]`
:   `[BLES]`
:   `[BURI]`
:   `[CENS]`
:   `[CHR]`
:   `[CHRA]`
:   `[CONF]`
:   `[CREM]`
:   `[DEAT]`
:   `[EMIG]`
:   `[FCOM]`
:   `[GRAD]`
:   `[IMMI]`
:   `[NATU]`
:   `[ORDN]`
:   `[PROB]`
:   `[RETI]`
:   `[WILL]`
:   `[EVEN]`

Contexts
:   `[INDI]`.(`[IndividualEvent]`)

Description
:   Varies by subtype

Payload
:   Either the string `Y` or not present.
    If any of the following are true

    - the payload is `Y`
    - there is a `[DATE]` substructure
    - there is a `[PLAC]` substructure
    - the tag is `[EVEN]`

    then the IndividualEvent asserts that the event occurred; otherwise it just discusses the idea of the event.

    There SHOULD NOT be a payload for the IndividualEvent subclass `[EVEN]`.

Substructures
:   Those inherited from `[Event]`
:   `[AGE]`?



### FamilyEvent   {#FamilyEvent}

The *FamilyEvent* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Supertype
:   `[Event]`

Known Subtypes
:   `[ANUL]`
:   `[CENS]`
:   `[DIV]`
:   `[DIVF]`
:   `[ENGA]`
:   `[MARR]`
:   `[MARB]`
:   `[MARC]`
:   `[MARL]`
:   `[MARS]`
:   `[EVEN]`

Contexts
:   `[INDI]`.(`[FamilyEvent]`)

Description
:   Varies by subtype

Payload
:   Either the string `Y` or not present.
    If any of the following are true

    - the payload is `Y`
    - there is a `[DATE]` substructure
    - there is a `[PLAC]` substructure
    - the tag is `[EVEN]`

    then the FamilyEvent asserts that the event occurred; otherwise it just discusses the idea of the event.

    There SHOULD NOT be a payload for the FamilyEvent subclass `[EVEN]`.

Substructures
:   Those inherited from `[Event]`
:   `[HUSB]`?
:   `[WIFE]`?


{.note} Prior to GEDCOM 5.5.1, `[AGE]`? was listed as a substructure of all (`[Event]`)s, including (`[FamilyEvent]`).  The semantics of a (`[FamilyEvent]`).`[AGE]` was not defined in any GEDCOM specification and it was removed from 5.5.1 without notice; this specification does not define what a (`[FamilyEvent]`).`[AGE]` might mean if present.


### IndividualAttribute   {#IndividualAttribute}

The *IndividualAttribute* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Represents attributes or facts are used to describe an individual's actions, physical description, employment, education, places of residence, etc.
These are not generally thought of as events.
However, they are often described like events because they were observed at a particular time and/or place.


{.note} Prior to GEDCOM 5.5.1, there was no `[FACT]` attribute and `[EVEN]` was sometimes used in its place.

Supertype
:   `[Event]`

Known Subtypes
:   `[CAST]`
:   `[DSCR]`
:   `[EDUC]`
:   `[IDNO]`
:   `[NATI]`
:   `[NCHI]`
:   `[NMR]`
:   `[OCCU]`
:   `[PROP]`
:   `[RELI]`
:   `[RESI]`
:   `[SSN]`
:   `[TITL]`
:   `[FACT]`

Contexts
:   `[INDI]`.(`[IndividualAttribute]`)

Description
:   Varies by subtype

Payload
:   Varies by subtype

Substructures
:   Those inherited from `[Event]`
:   `[AGE]`?




## Concrete Types


### ABBR  {#ABBR}

A short name of a title, description, or name.

Contexts
:   `[SOUR]`.`[ABBR]`

Description
:   A short title used for sorting, filing, and retrieving source records.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.

Substructures
:   None


### ADDR  {#ADDR}

The contemporary place, usually required for postal purposes, of an individual, a submitter of information, a repository, a business, a school, or a company.

Contexts
:   .`[HEAD]`.`[SOUR]`.`[CORP]`.`[ADDR]`
:   .`[REPO]`.`[ADDR]`
:   .`[SUBM]`.`[ADDR]`

Description
:   Address information that, when combined with NAME substructure, meets requirements for sending communications through the mail.

Payload
:   A string. It is RECOMMENDED that implementations support at least three lines of at least 60 characters each.

Substructures
:   `[ADR1]`?
:   `[ADR2]`?
:   `[CITY]`?
:   `[STAE]`?
:   `[POST]`?
:   `[CTRY]`?
:   `[PHON]`\* -- it is RECOMMENDED that at least three `ADDR.PHON` be supported per `ADDR`
:   `[EMAIL]`\* -- it is RECOMMENDED that at least three `ADDR.FAX` be supported per `ADDR`
:   `[FAX]`\* -- it is RECOMMENDED that at least three `ADDR.EMAIL` be supported per `ADDR`
:   `[WWW]`\* -- it is RECOMMENDED that at least three `ADDR.WWW` be supported per `ADDR`

{.note} `[EMAIL]` was introduced in GEDCOM 5.5.1 with two tag names: both `EMAIL` and `EMAI`.  `EMAIL` was used more consistently and is documented here, but it is RECOMMENDED that implementations treat `ADDR.EMAI` as synonymous with `ADDR.EMAIL`.




### ADR1  {#ADR1}

The first line of an address.

Contexts
:   `[ADDR]`.`[ADR1]`

Description
:   The first line of the address used for indexing.
    This SHOULD correspond to the first line of the superstructure's payload.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.

Substructures
:   None

### ADR2  {#ADR2}

The second line of an address.

Contexts
:   `[ADDR]`.`[ADR1]`

Description
:   The second line of the address used for indexing.
    This SHOULD correspond to the second line of the superstructure's payload.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.

Substructures
:   None


### ADOP   {#ADOP}

Pertaining to creation of a child-parent relationship that does not exist biologically.

Contexts
:   .`[INDI]`.`[ADOP]`
:   .`[ADOP]`.`[FAMC]`.`[ADOP]`

####  Context .`[INDI]`.`[ADOP]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent),
    `[FAMC]`?

####  Context `[ADOP]`.`[FAMC]`.`[ADOP]`
Description
:   Which parent(s) adopted

Payload
:   A string, which SHOULD be from the set {`HUSB`, `WIFE`, `BOTH`}

Substructures
:   None


### AGE   {#AGE}

The age of the individual at the time an event occurred, or the age listed in the document.

{.note} GEDCOM 5.5 the existence of (`[FamilyEvent]`).`AGE`, but 5.5.1 does not.  This document follows 5.5.1 because the meaning of a (`[FamilyEvent]`).`AGE` is not obvious.

Contexts
:   (`[FamilyEvent]`).`[HUSB]`.`[AGE]`
:   (`[FamilyEvent]`).`[WIFE]`.`[AGE]`
:   (`[IndividualEvent]`).`[AGE]`
:   (`[IndividualAttribute]`).`[AGE]`

Description
:   A number that indicates the age in years, months, and days that the principal was at the time of the associated event.

Payload
:   A string, which uses an age-specification microformat.
    The microformat supports three special values (`CHILD`, `INFANT`, and `STILLBORN`)
    as well as any text matching the regular expression

    > `([<>] ?)?(\d+y ?)?(\d+m ?)?(\d+d ?)?`

    The component pieces of the microformat have the following meanings:

    | Symbol | Meaning |
    |--------|---------|
    | `>`    | greater than indicated age |
    | `<`    | less than indicated age |
    | `\d+y` | a number of years |
    | `\d+m` | a number of months |
    | `\d+d` | a number of days |
    | `CHILD`| `<8y`   |
    | `INFANT`| `<1y`  |
    | `STILLBORN`| just prior, at, or near birth; or `0y` |


Substructures
:   None


{.note} The GEDCOM specifications were unclear as to the use of spaces in the age-specification microformat.  To maximize compatibility, it is RECOMMENDED that implementations be able to _parse_ any string matching the regular expression above but _generate_ no spaces when producing age strings.


### AGNC   {#AGNC}

The institution or individual having authority and/or responsibility to manage or govern.

Contexts
:   .`[SOUR]`.`[DATA]`.`[AGNC]`
:   (`[Event]`).`[AGNC]`

####  Context .`[SOUR]`.`[DATA]`.`[AGNC]`

Description
:   The organization, institution, corporation, person, or other entity that has authority or control interests in the associated context. For example, an organization responsible for creating and/or archiving records.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None

#### Context (`[Event]`).`[AGNC]`

Description
:   The organization, institution, corporation, person, or other entity that has authority or control interests in the associated context. For example, an employer of a person of an associated occupation, or a church that administered rites or event.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None



### ALIA   {#ALIA}

An indicator to link different record descriptions of a person who may be the same person.

Contexts
:   .`[INDI]`.`[ALIA]`

Payload
:   Pointer to an `[INDI]`

Substructures
:   None


### ANCE   {#ANCE}

Pertaining to forbearers of an individual.

See also `[DESC]`

Contexts
:   .`[SUBN]`.`[ANCE]`

Description
:   The number of generations of ancestors included in this transmission.

    This value should be considered a hint, not as any kind of hard limit on the contents of the data set.

Payload
:   A non-negative integer represented in base 10.

Substructures
:   None


### ANCI   {#ANCI}

Indicates an interest in additional research for ancestors of this individual. (See also `[DESI]`.)

Contexts
:   .`[INDI]`.`[ANCI]`

Payload
:   Pointer to an `[SUBM]`

Substructures
:   None


### ANUL  {#ANUL}

Declaring a marriage void from the beginning (never existed).

Contexts
:   .`[FAM]`.`[ANUL]`

Description
:   *see `[FamilyEvent]`*

Supertype
:   `[FamilyEvent]`

Payload
:   Either `Y` or None, as described under `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### ASSO   {#ASSO}

An indicator to link friends, neighbors, relatives, or associates of an individual.

Contexts
:   .`[INDI]`.`[ASSO]`

Payload
:   Pointer to an `[INDI]`

Substructures
:   `[RELA]`!
:   `[NOTE]`\*
:   `[SOUR]`\*


### AUTH   {#AUTH}

The name of the individual who created or compiled information.

Contexts
:   .`[SOUR]`.`[AUTH]`

Description
:   The person, agency, or entity who created the record. For a published work, this could be the author, compiler, transcriber, abstractor, or editor. For an unpublished source, this may be an individual, a government agency, church organization, or private organization, etc.

Payload
:   String of arbitrary length

Substructures
:   None


### BAPM   {#BAPM}

The event of baptism, performed in infancy or later.

See also `[BAPL]` and `[CHR]`

Contexts
:   .`[INDI]`.`[BAPM]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### BARM   {#BARM}

The ceremonial event held when a Jewish boy reaches age 13.

Contexts
:   .`[INDI]`.`[BARM]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### BASM   {#BASM}

The ceremonial event held when a Jewish girl reaches age 13, also known as "Bat Mitzvah."

Contexts
:   .`[INDI]`.`[BASM]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### BIRT   {#BIRT}

The event of entering into life.

Contexts
:   .`[INDI]`.`[BIRT]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)
:   `[FAMC]`?


### BLES   {#BLES}

A religious event of bestowing divine care or intercession. Sometimes given in connection with a naming ceremony.

Contexts
:   .`[INDI]`.`[BLES]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### BLOB   {#BLOB}

A grouping of data used as input to a multimedia system that processes binary data to represent images, sound, and video.

{.note} This tag was present in GEDCOM 5.5 but was **removed** from GEDCOM 5.5.1.  It is RECOMMENDED that `BLOB` not be used in new data streams.  See `[OBJE]` for more.

When multiple blobs are present in the same structure, their contents are concatenated in order.

Contexts:
:   .`[OBJE]`.`[BLOB]`

Description
:   Encoded binary data

Payload
:   Two or more lines of base-64 encoded data, in the custom format described below.

    The first line of of a blob is always empty.
    Each subsequent line is between 4 and 72 characters long, encoded in a base-64 format that differs from other base-64 encodings in two ways.

    First, it uses byte 0xFF as padding instead of the more common U+003D (EQUALS SIGN `=`)
    (how to represent the padding when byte 0xFF is not a legal character in the encoding is not defined by this specification).

    Second, it maps bytes to code points as follows:

    | Byte range | Code point mapping |
    |------------|--------------------|
    | 0x00--0x0B | byte + 0x2E        |
    | 0x0C--0x25 | byte + 0x35        |
    | 0x25--0x3F | byte + 0x3B        |

Substructures
:   None



### BURI   {#BURI}

The event of the proper disposing of the mortal remains of a deceased person.

{.ednote} GEDCOM calls burial "proper disposing" sans discussion of method, but cremation "disposal […] by fire" sans proper?  Why?

See also `[CREM]`

Contexts
:   .`[INDI]`.`[BURI]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### CALN   {#CALN}

The number used by a repository to identify the specific items in its collections.

Contexts
:   .`[SOUR]`.`[REPO]`.`[CALN]`

Description
:   An identification or reference description used to file and retrieve items from the holdings of a repository.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   `[MEDI]`?

{.note} Due to an example in the GEDCOM specification that is inconsistent with the grammar, it is RECOMMENDED that implementations parse a .`[SOUR]`.`[REPO]`.`[MEDI]` (i.e., coordinate with instead of subordinate to `CALN`) as if they were .`[SOUR]`.`[REPO]`.`[CALN]`.`[MEDI]`.


### CAST   {#CAST}

The name of an individual's rank or status in society, based
on racial or religious differences, or differences in wealth, inherited
rank, profession, occupation, etc.

Contexts
:   .`[INDI]`.`[CAST]`

Description
:   A name assigned to a particular group that this person was associated with, such as a particular racial group, religious group, or a group with an inherited status.

Supertype
:   `[IndividualAttribute]`

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Substructures
:   [*inherited*](#IndividualAttribute)


### CAUS   {#CAUS}

A description of the cause of the associated event or fact, such as the cause of death.

Contexts
:   ([Event]).`[CAUS]`

Description
:   Used in special cases to record the reasons which precipitated an event. Normally this will be used subordinate to a death event to show cause of death, such as might be listed on a death certificate.

Substructures
:   None


### CENS  {#CENS}

The event of the periodic count of the population for a designated locality, such as a national or state Census.

Contexts
:   .`[INDI]`.`[CENS]`
:   .`[FAM]`.`[CENS]`

Description
:   *see `[IndividualEvent]` or `[FamilyEvent]`*

Payload
:   Either `Y` or None, as described under `[IndividualEvent]` or `[FamilyEvent]`

Substructures
:   [*inherited from IndividualEvent*](#IndividualEvent) or [*inherited from FamilyEvent*](#FamilyEvent)

{.ednote} Should this tag have context-specific subsections like `[ADOP]`?  Not much varies based on context...


### CHAN   {#CHAN}

Indicates a change, correction, or modification. Typically used in connection with a `[DATE]` to specify when a change in information occurred.

Contexts
:   (`[Record]`).`[CHAN]`

Description
:   The change date is intended to only record the last change to a record. Some systems may want to manage the change process with more detail, but it is sufficient for GEDCOM purposes to indicate the last time that a record was modified.

Payload
:   None

Substructures
:   `[DATE]`!
:   `[NOTE]`\*


### CHAR   {#CHAR}

An indicator of the character set used in writing this automated information.

{.ednote} To do: I have not yet checked to see if this changed in 5.5.1

Contexts
:   .`[HEAD]`.`[CHAN]`

Description
:   A code value that represents the character set to be used to interpret this data. The default character set is ANSEL, which includes ASCII as a subset. UNICODE is not widely supported by most operating systems; therefore, GEDCOM produced using the UNICODE character set will be limited in acceptance for some time. See Chapter 3. ASCII contains the character set from 0x0 to 0x7F.

Payload
:   one of `ANSEL`, `UNICODE`, or `ASCII`

Substructures
:   `[VERS]`?


### CHIL   {#CHIL}

The natural, adopted, or sealed (LDS) child of a father and a mother.

Contexts
:   .`[FAM]`.`[CHIL]`

Payload
:   A pointer to an `[INDI]`

Substructures
:   None


{.note} In GEDCOM versions 3 and 4, `FAM.CHIL` had a substructure `[ADOP]`.


### CHR   {#CHR}

The religious event (not LDS) of baptizing and/or naming a child.

Contexts
:   .`[INDI]`.`[CHR]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent), `[FAMC]`?


### CHRA   {#CHRA}

The religious event (not LDS) of baptizing and/or naming an adult person.

Contexts
:   .`[INDI]`.`[CHRA]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### CITY   {#CITY}

A lower level jurisdictional unit. Normally an incorporated municipal unit.

Contexts
:   `[ADDR]`.`[CITY]`

Description
:   The name of the city used in the address. Isolated for sorting or indexing.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.

Substructures
:   None


### CONF   {#CONF}

The religious event (not LDS) of conferring the gift of the Holy Ghost and, among protestants, full church membership.

{.ednote} Is this the right definition of a confirmation?

Contexts
:   .`[INDI]`.`[CONF]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)



### COPR   {#COPR}

A statement that accompanies data to protect it from unlawful duplication and distribution.

Contexts
:   .`[HEAD]`.`[COPR]`
:   .`[HEAD]`.`[SOUR]`.`[DATA]`.`[COPR]`

#### Context .`[HEAD]`.`[COPR]`

Description
:   A copyright statement needed to protect the copyrights of the submitter of this GEDCOM file.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Substructures
:   None

#### Context .`[HEAD]`.`[SOUR]`.`[DATA]`.`[COPR]`

Description
:   A copyright statement required by the owner of data from which this information was downloaded. For example, when a GEDCOM download is requested from the Ancestral File, this would be the copyright statement to indicate that the data came from a copyrighted source.

Payload
:   A string. It is RECOMMENDED that implementations support multi-line payloads of arbitrary size.

Substructures
:   None



### CORP   {#CORP}

A name of an institution, agency, corporation, or company.

Contexts
:   `.HEAD.SOUR.CORP`

Description
:   Name of the business, corporation, or person that produced or commissioned the product.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Substructures
:   `[ADDR]`?
:   `[PHON]`\*


### CREM   {#CREM}

Disposal of the remains of a person's body by fire.

See also `[BURI]`

Contexts
:   .`[INDI]`.`[CREM]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### CTRY   {#CTRY}

The name or code of the country.

Contexts
:   `ADDR.CTRY`

Description
:   The name of the country that pertains to the associated address. Isolated by some systems for sorting or indexing. Used in most cases to facilitate automatic sorting of mail.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.

Substructures
:   None


### DATA   {#DATA}

Pertaining to stored automated information.

Contexts
:   .`[HEAD]`.`[SOUR]`.`[DATA]`
:   .`[SOUR]`.`[DATA]`
:   `[SOUR]`.`[DATA]`

#### Context .`[HEAD]`.`[SOUR]`.`[DATA]`

Description
:   The name of the electronic data source that was used to obtain the data in this transmission. For example, the data may have been obtained from a CD-ROM disc that was named "U.S. 1880 CENSUS CD-ROM vol. 13."

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Substructures
:   `[DATE]`?
:   `[COPR]`?

#### Context .`[SOUR]`.`[DATA]`

Payload
:   None

Substructures
:   `[EVEN]`\*
:   `[AGNC]`?
:   `[NOTE]`\*

#### Context `[SOUR]`.`[DATA]`

Payload
:   None

Substructures
:   `[DATE]`?
:   `[TEXT]`\*


### DATE   {#DATE}

The time of an event in a calendar format.

Contexts
:   `[CHAN]`.`[DATE]`
:   ([Event]).`[DATE]`
:   .`[HEAD]`.`[DATE]`
:   .`[HEAD]`.`[SOUR]`.`[DATA]`.`[DATE]`
:   .`[SOUR]`.`[DATA]`.`[EVEN]`.`[DATE]`
:   `[SOUR]`.`[DATA]`.`[DATE]`


#### DATE Payload Formatting

Dates are represented using a somewhat involved syntax, which shares a common subformat and has three entry points, documented below.

##### Date {#date-format}

At the core of the date syntax is a calendared date.
This consists of an optional *calender escape*, which is a substring beginning `@#D` and ending `@`, between which is a calender identifier.
Known calender identifiers are `GREGORIAN`, `FRENCH R`, `HEBREW`, `JULIAN`, `ROMAN`, and `UNKNOWN`.
If not calender escape is given, `GREGORIAN` is assumed.
After the optional escape is the content of the date, which takes a format dependent on the calender.

{.note} Some `[DATE]` payloads may have multiple Date values; it is not known if current implementations can handle situations where the dates are from different calenders, nor if they assume an uncalendered date paired with a calendered date is `GREGORIAN` or the same as the other date provided.  It is RECOMMENDED that the same calender be used for both Dates in such payloads.

The `ROMAN` and `UNKNOWN` calenders's date formats are not defined in this specification.

`GREGORIAN`, `FRENCH R`, `HEBREW`, and `JULIAN` dates all have the format "day month year", separated by spaces; the day may be omitted; if the day is omitted, the month may be omitted as well.
The three pieces are formatted as follows:
    
day
:   A decimal number of one or two digits.
    This SHOULD NOT be zero or greater than the number of days in the appropriate month.
    This specification does not specify whether single-digit days should begin with a zero or not.

month
:   Each calender has a set of strings that may be used.
    
    `GREGORIAN` or `JULIAN`
    :   One of the following three-character strings:
        `JAN`, `FEB`, `MAR`, `APR`, `M‌AY`, `JUN`, `JUL`, `AUG`, `SEP`, `OCT`, `NOV`, or `DEC`
    
    `FRENCH R`
    :   One of the following four-character strings:
        `VEND`, `BRUM`, `FRIM`, `NIVO`, `PLUV`, `VENT`, `GERM`, `FLOR`, `PRAI`, `MESS`, `THER`, `FRUC`, `COMP`
    
    `HEBREW`
    :   One of the following three-character strings:
        `TSH`, `CSH`, `KSL`, `TVT`, `SHV`, `ADR`, `ADS`, `NSN`, `IYR`, `SVN`, `TMZ`, `AAV`, `ELL`

year
:   A decimal number.
    
    For `GREGORIAN` (only), the number may be optionally followed by either or both of the following year suffixes:
    
    Alternate Year
    :   Represented as a `/` and two additional decimal digits, with no spaces.
        Shows the possible date alternatives for pre-1752 date brought about by a changing the beginning of the year from MAR to JAN in the English calendar change of 1752, for example, `15 APR 1699/00`.
        
        The `/` MUST NOT have a space on either side.
    
    BCE
    :   Represented as `(B.C.)`, which SHOULD be preceded by a space.
        Indicates a date before the birth of Christ.
    
    If both suffixes are present, `(B.C.)` comes last.

##### Exact Date {#exact-date}

Some `[DATE]` payloads are restricted to *exact dates*,
meaning `GREGORIAN` [Date](#date-format)s with the following constraints:

-   They MUST NOT include a *calender escape*
-   They MUST include the day and month
-   They MUST NOT have either year suffix

##### Date Period {#date-period}

Some `[DATE]` payloads are restricted to *date periods*,
meaning one of the following three forms:

-   `FROM` [Date](#date-format)
-   `TO` [Date](#date-format)
-   `FROM` [Date](#date-format) `TO` [Date](#date-format)

##### Date Value {#date-value}

Many `[DATE]` payloads, referred to below *date values*, may have any of a variety of formats:

-   [Date](#date-format)
-   [Date Period](#date-period)
-   `BEF` [Date](#date-format)
    
    Meaning: before the given date.
-   `AFT` [Date](#date-format)
    
    Meaning: after the given date.
-   `BET` [Date](#date-format) `AND` [Date](#date-format) 
    
    Meaning: between the given dates.

    The first date SHOULD be earlier than the second date.
-   `ABT` [Date](#date-format)
    
    Meaning: about; the given date is not exact.
-   `CAL` [Date](#date-format)
    
    Meaning: calculated mathematically, for example, from an event date and age.
-   `EST` [Date](#date-format)

    Meaning: estimated based on some other event date.
-   `INT` [Date](#date-format) `(`arbitrary text`)`

    Meaning: interpreted from knowledge about the associated date phrase included in parentheses.
-   `(`arbitrary text`)`
    
    The text gives information about when an event occurred but is not recognizable to a date parser.

#### Context `[CHAN]`.`[DATE]`

Description
:   The date that this data was changed.

Payload
:   A string matching the [Exact Date](#exact-date) syntax above.

Substructures
:   `[TIME]`?


#### Context ([Event]).`[DATE]`

Description
:   The date of an activity, attribute, or event.

Payload
:   A string matching the [Date Value](#date-value) syntax above.

Substructures
:   None


#### Context .`[HEAD]`.`[DATE]`

Description
:   The date that this transmission was created.

Payload
:   A string matching the [Exact Date](#exact-date) syntax above.

Substructures
:   `[TIME]`?



#### Context .`[HEAD]`.`[SOUR]`.`[DATA]`.`[DATE]`

Description
:   The date this source was published or created.

Payload
:   A string matching the [Exact Date](#exact-date) syntax above.

Substructures
:   None


#### Context .`[SOUR]`.`[DATA]`.`[EVEN]`.`[DATE]`

Description
:   The date that this event data was entered into the original source document.

Payload
:   A string matching the [Date Period](#date-period) syntax above.

Substructures
:   None

#### Context `[SOUR]`.`[DATA]`.`[DATE]`

Description
:   The date that this event data was entered into the original source document.

Payload
:   A string matching the [Date Value](#date-value) syntax above.

Substructures
:   None



### DEAT   {#DEAT}

The event when mortal life terminates.


Contexts
:   .`[INDI]`.`[DEAT]`

Description
:   *see `[IndividualEvent]`*

Supertype
:   `[IndividualEvent]`

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### DESC   {#DESC}

Pertaining to offspring of an individual.

See also `[ANCE]`

Contexts
:   .`[ANCE]`.`[DESC]`

Description
:   The number of generations of descendants included in this transmission.

    This value should be considered a hint, not as any kind of hard limit on the contents of the data set.

Payload
:   A non-negative integer represented in base 10.

Substructures
:   None


### DESI   {#DESI}

Indicates an interest in research to identify additional descendants of this individual. (See also `[ANCI]`.)

Contexts
:   .`[INDI]`.`[DESI]`

Payload
:   Pointer to an `[SUBM]`

Substructures
:   None


### DEST   {#DEST}

A system receiving data.

Contexts
:   .`[HEAD]`.`[DEST]`

Description
:   The name of the system expected to process the transmission.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 20 characters.

Substructures
:   None



### DIV   {#DIV}

An event of dissolving a marriage through civil action.

Contexts
:   .`[FAM]`.`[DIV]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None, as described under `[FamilyEvent]`

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### DIVF   {#DIVF}

An event of filing for a divorce by a spouse.

Contexts
:   .`[FAM]`.`[DIVF]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None, as described under `[FamilyEvent]`

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### DSCR   {#DSCR}

The physical characteristics of a person, place, or thing.

Contexts
:   .`[INDI]`.`[DSCR]`

Description
:   An unstructured list of the attributes that describe the physical characteristics of a person, place, or object. Commas separate each attribute.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 248 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### EDUC   {#EDUC}

Indicator of a level of education attained.

Contexts
:   .`[INDI]`.`[EDUC]`

Description
:   A description of a scholastic or educational achievement or pursuit.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 248 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### EMAIL   {#EMAIL}

An electronic mail address.

Contexts
:   `[ADDR]`.`[EMAIL]`

Description
:    An electronic address that can be used for contact such as an email address.  

Payload
:    A string. It is RECOMMENDED that implementations support payloads of at least 120 characters. 

Substructures
:    None

{.note} `[EMAIL]` was introduced in GEDCOM 5.5.1 with two tag names: both `EMAIL` and `EMAI`.  `EMAIL` was used more consistently and is documented here, but it is RECOMMENDED that implementations treat `ADDR.EMAI` as synonymous with `ADDR.EMAIL`.


### EMIG   {#EMIG}

An event of leaving one's homeland with the intent of residing elsewhere.

Contexts
:   .`[INDI]`.`[EMIG]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### ENGA   {#ENGA}

An event of recording or announcing an agreement between two people to become married.

Contexts
:   .`[FAM]`.`[ENGA]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None, as described under `[FamilyEvent]`

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### EVEN   {#EVEN}

`http://terms.fhiso.org/legacy/longform/EVENT`

A noteworthy happening related to an individual, a group, or an organization.

Contexts
:   .`[INDI]`.`[EVEN]`
:   .`[FAM]`.`[EVEN]`
:   .`[SOUR]`.`[DATA]`.`[EVEN]`
:   `[SOUR]`.`[EVEN]`

#### Context .`[INDI]`.`[EVEN]` and .`[FAM]`.`[EVEN]`

Description
:   The `EVEN` tag is for recording general events or attributes that are not other known subtypes of `[Event]`.
    The general event or attribute type is declared by using a subordinate `[TYPE]` tag to show what event or attribute is recorded.

Payload
:   Either `Y` or None, as described under `[IndividualEvent]` and `[FamilyEvent]`, respectively.

Supertype
:   `[IndividualEvent]` and `[FamilyEvent]`, respectively.

Sunstructures
:   inherited from the supertypes
:   `[TYPE]`!

{.ednote} Should these two be separated into context-specific subsections?


#### Context .`[SOUR]`.`[DATA]`.`[EVEN]`

Description
:   An enumeration of the different kinds of events that were recorded in a particular source. Each enumeration is separated by a comma. Such as a parish register of births, deaths, and marriages would be `BIRT`, `DEAT`, `MARR`.

Payload
:   A string, formatted as a comma-separated list of tokens.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    Known tokens include {`ANUL`, `CENS`, `DIV`, `DIVF`, `ENGA`, `MARR`, `MARB`, `MARC`, `MARL`, `MARS`, `EVEN`, `ADOP`, `BIRT`, `BAPM`, `BARM`, `BASM`, `BLES`, `BURI`, `CENS`, `CHR`, `CHRA`, `CONF`, `CREM`, `DEAT`, `EMIG`, `FCOM`, `GRAD`, `IMMI`, `NATU`, `ORDN`, `RETI`, `PROB`, `WILL`, `EVEN`, `CAST`, `EDUC`, `NATI`, `OCCU`, `PROP`, `RELI`, `RESI`, `TITL`, `FACT`}

Substructures
:   `[DATE]`?
:   `[PLAC]`?

#### Context `[SOUR]`.`[EVEN]`

Description
:   A code that indicates the type of event which was responsible for the source entry being recorded. For example, if the entry was created to record a birth of a child, then the type would be `BIRT` regardless of the assertions made from that record, such as the mother's name or mother's birth date. This will allow a prioritized best view choice and a determination of the certainty associated with the source used in asserting the cited fact.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 15 characters.
    
    Known values include {`ANUL`, `CENS`, `DIV`, `DIVF`, `ENGA`, `MARR`, `MARB`, `MARC`, `MARL`, `MARS`, `EVEN`, `ADOP`, `BIRT`, `BAPM`, `BARM`, `BASM`, `BLES`, `BURI`, `CENS`, `CHR`, `CHRA`, `CONF`, `CREM`, `DEAT`, `EMIG`, `FCOM`, `GRAD`, `IMMI`, `NATU`, `ORDN`, `RETI`, `PROB`, `WILL`, `EVEN`, `CAST`, `EDUC`, `NATI`, `OCCU`, `PROP`, `RELI`, `RESI`, `TITL`, `FACT`}

Substructures
:   `[ROLE]`


### FACT   {#FACT}

Pertaining to a noteworthy attribute or fact concerning an individual, a group, or an organization. A `FACT` structure is usually qualified or classified by a subordinate use of the `TYPE` tag.

Contexts
:   .`[INDI]`.`[FACT]`

Description
:   Text describing a particular characteristic or attribute assigned to an individual.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)
:   `[TYPE]`!


### FAM   {#FAM}

Identifies a legal, common law, or other customary relationship of man and woman and their children, if any, or a family created by virtue of the birth of a child to its biological father and mother.

Contexts
:   `.FAM`

Description
:   The `FAM` record is used to record marriages, common law marriages, and family unions caused by two people becoming the parents of a child. There can be no more than one `HUSB`/father and one `WIFE`/mother listed in each `FAM`. If, for example, a man participated in more than one family union, then he would appear in more than one `FAM`. The family record structure assumes that the `HUSB`/father is male and `WIFE`/mother is female.

Payload
:   None

Substructures
:   (`[FamilyEvent]`)\*
:   `[HUSB]`?
:   `[WIFE]`?
:   `[CHIL]`\* -- The preferred order of the `[CHIL]` substructures within a `[FAM]` structure is chronological by birth.
:   `[NCHI]`?
:   `[SUBM]`\*
:   `[SOUR]`\*
:   `[OBJE]`\*
:   `[NOTE]`\*
:   `[REFN]`\*
:   `[RIN]`?
:   `[CHAN]`?
:   `[RESN]`?



### FAMC   {#FAMC}

`http://terms.fhiso.org/legacy/longform/FAMILY_CHILD`

Identifies the family in which an individual appears as a child.

Contexts
:   `[BIRT]`.`[FAMC]`
:   `[CHR]`.`[FAMC]`
:   .`[INDI]`.`[FAMC]`
:   .`[INDI]`.`[ADOP]`.`[FAMC]`

Payload
:   Pointer to a `[FAM]`

Substructures
:   `[BIRT]`.`[FAMC]` and `[CHR]`.`[FAMC]` have None;
    others listed below.

#### Context .`[INDI]`.`[FAMC]`

Substructures
:   `[PEDI]`?
:   `[NOTE]`\*
:   `[STAT]`?

#### Context .`[INDI]`.`[ADOP]`.`[FAMC]`

Substructures
:   `[ADOP]`?



### FAMS   {#FAMS}

Identifies the family in which an individual appears as a spouse.

Contexts
:   .`[INDI]`.`[FAMS]`  

Payload
:    Pointer to a `[FAM]` 

Substructures
:    `[NOTE]`\*


### FAX   {#FAX}

Electronic facimilie transmission.

{.note} This tag was introduced in GEDCOM 5.5.1.

Contexts
:   `[ADDR]`.`[FAX]` 

Description
:    A FAX telephone number appropriate for sending data facsimiles.  

Payload
:    A string. It is RECOMMENDED that implementations support payloads of at least 60 characters. 

Substructures
:    None


### FCOM   {#FCOM}

A religious rite, the first act of sharing in the Lord's supper as part of church worship.

Contexts
:   .`[INDI]`.`[FCOM]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


{.ednote} Continue reformatting from here



### FILE   {#FILE}

`http://terms.fhiso.org/legacy/longform/FILE`

An information storage place that is ordered and arranged for preservation and reference.

#### GEDCOM 5.5-compatible version

Contexts
:   `OBJE.FILE`

Description
:   A complete local or remote file reference to the auxiliary data to be linked to the GEDCOM context. Remote reference would include a network address where the multimedia data may be obtained.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   None

#### GEDCOM 5.5.1-compatible version

Contexts
:   `OBJE.FILE`

Description
:   A complete local or remote file reference to the auxiliary data to be linked to the GEDCOM context. Remote reference would include a network address where the multimedia data may be obtained.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   `[FORM]`!, `[TITL]`?

#### Common information

Contexts
:   `.HEAD.FILE`

Description
:   The name of the GEDCOM transmission file. If the file name includes a file extension it must be shown in the form (filename.ext).

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Substructures
:   None


### FONE   {#FONE}

`http://terms.fhiso.org/legacy/longform/PHONETIC`

A phonetic variation of a superior text string.

{.note} This tag was introduced in GEDCOM 5.5.1.

The method of phonetic writing is indicated by the subordinate `TYPE` value; for example if hiragana was used to provide a reading of a name written in kanji, then the `TYPE` value would indicate `kana`.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.INDI.NAME.FONE` | The phonetic variation of the name in the same form as the was the name used in the superior primitive.  | 1--120 characters | `[TYPE]`!, `[NPFX]`?, `[GIVN]`?, `[NICK]`?, `[SPFX]`?, `[SURN]`?, `[NSFX]`?, `[NOTE]`\*, `[SOUR]`\*
`[Event]``.PLAC.FONE` | The phonetic variation of the place name in the same form as the was the name used in the superior primitive. | `[TYPE]`!


### FORM   {#FORM}

`http://terms.fhiso.org/legacy/longform/FORMAT`

An assigned name given to a consistent format in which information can be conveyed.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.PLAC.FORM` | Implies that all place names follow this jurisdictional format and each jurisdiction is accounted for by a comma, whether the name is known or not. May be overridden by `[Event]``.PLAC.FORM`. | 1--120 characters | None
`[Event]``.PLAC.FORM` | Jurisdictional entities that are named in a sequence from the lowest to the highest jurisdiction. The jurisdictions are separated by commas, and any jurisdiction's name that is missing is still accounted for by a comma. | 1--120 characters | None
`.HEAD.GEDC.FORM` | The GEDCOM form used to construct this transmission. | 14--20 characters, specifically `LINEAGE_LINKED` | None


#### GEDCOM 5.5-compatible version

Contexts
:   `OBJE.FORM`

Description
:   Indicates the format of the multimedia data with this tag.

Payload
:   one of {`bmp`, `gif`, `jpeg`, `ole`, `pcx`, `tiff`, `wav`}

Substructures
:   None

#### GEDCOM 5.5.1-compatible version

Contexts
:   `FILE.FORM`

Description
:   Indicates the format of the multimedia data with this tag.

Payload
:   one of {`bmp`, `gif`, `jpeg`, `ole`, `pcx`, `tiff`, `wav`}

Substructures
:   None


### GEDC   {#GEDC}

Information about the use of GEDCOM in a transmission.

Contexts
:   `.HEAD.GEDC`  

Description
:            

Payload
:    None    

Substructures
:    `[VERS]`!, `[FORM]`!


### GIVN   {#GIVN}

A given or earned name used for official identification of a person.

Contexts
:   `.INDI.NAME.GIVN`   

Description
:     Given name or earned name. Different given names are separated by a comma. 

Payload
:    A string.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    It is RECOMMENDED that implementations support at least 90 characters between each comma.

Substructures
:    None


### GRAD   {#GRAD}

`http://terms.fhiso.org/legacy/longform/GRADUATION`

An event of awarding educational diplomas or degrees to individuals.

Contexts
:   `.INDI.GRAD`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### HEAD   {#HEAD}

Identifies information pertaining to an entire GEDCOM transmission.

Contexts
:   `.HEAD`       

Description
:            

Payload
:    None    

Substructures
:    `[SOUR]`!, `[DEST]`?, `[DATE]`?, `[SUBM]`!, `[SUBN]`?, `[FILE]`?, `[COPR]`?, `[GEDC]`!, `[CHAR]`!, `[LANG]`?, `[PLAC]`?, `[NOTE]`?


### HUSB   {#HUSB}

`http://terms.fhiso.org/legacy/longform/HUSBAND`

An individual in the family role of a married man or father.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.FAM.``[FamilyEvent]``.HUSB` | | None | `[AGE]`!
`.FAM.HUSB`   |         | pointer to an `[INDI]` | None


### IDNO   {#IDNO}

`http://terms.fhiso.org/legacy/longform/IDENT_NUMBER`

A number assigned to identify a person within some significant external system.

See also `[SSN]`

Contexts
:   `.INDI.IDNO`

Description
:   A nationally-controlled number assigned to an individual.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute), `[TYPE]`!

{.note} Although called a "number", the payload of an `IDNO` is *not* restricted to numeric values.


### IMMI   {#IMMI}

`http://terms.fhiso.org/legacy/longform/IMMIGRATION`

An event of entering into a new locality with the intent of residing there.

Contexts
:   `.INDI.IMMI`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### INDI   {#INDI}

`http://terms.fhiso.org/legacy/longform/INDIVIDUAL`

A person.

Contexts
:   `.INDI`

Description
:   The individual record is a compilation of facts, known or discovered, about an individual. Sometimes these facts are from different sources. This form allows documentation of the source where each of the facts were discovered.

Payload
:   None

Substructures
:   `[RESN]`?, `[NAME]`\*, `[SEX]`?, `[IndividualEvent]`, `[IndividualAttribute]`, `[FAMC]`\*, `[FAMS]`\*, `[SUBM]`\*, `[ASSO]`\*, `[ALIA]`\*, `[ANCI]`\*, `[DESI]`\*, `[SOUR]`\*, `[OBJE]`\*, `[NOTE]`\*, `[RFN]`?, `[REFN]`\*, `[RIN]`?, `[CHAN]`?

See also [LDS-specific tags]


### LANG   {#LANG}

`http://terms.fhiso.org/legacy/longform/LANGUAGE`

The name of the language used in a communication or transmission of information.

The set of known language payload value is {`Afrikaans`, `Albanian`, `Anglo-Saxon`, `Catalan`, `Catalan_Spn`, `Czech`, `Danish`, `Dutch`, `English`, `Esperanto`, `Estonian`, `Faroese`, `Finnish`, `French`, `German`, `Hawaiian`, `Hungarian`, `Icelandic`, `Indonesian`, `Italian`, `Latvian`, `Lithuanian`, `Navaho`, `Norwegian`, `Polish`, `Portuguese`, `Romanian`, `Serbo_Croa`, `Slovak`, `Slovene`, `Spanish`, `Swedish`, `Turkish`, `Wendic`, `Amharic`, `Arabic`, `Armenian`, `Assamese`, `Belorusian`, `Bengali`, `Braj`, `Bulgarian`, `Burmese`, `Cantonese`, `Church-Slavic`, `Dogri`, `Georgian`, `Greek`, `Gujarati`, `Hebrew`, `Hindi`, `Japanese`, `Kannada`, `Khmer`, `Konkani`, `Korean`, `Lahnda`, `Lao`, `Macedonian`, `Maithili`, `Malayalam`, `Mandrin`, `Manipuri`, `Marathi`, `Mewari`, `Nepali`, `Oriya`, `Pahari`, `Pali`, `Panjabi`, `Persian`, `Prakrit`, `Pusto`, `Rajasthani`, `Russian`, `Sanskrit`, `Serb`, `Tagalog`, `Tamil`, `Telugu`, `Thai`, `Tibetan`, `Ukrainian`, `Urdu`, `Vietnamese`, `Yiddish`}

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.LANG` | The human language in which the data in the transmission is normally read or written. | 1--15 characters; see above | None
`.SUBM.LANG` | The language in which a person prefers to communicate. Multiple language preference is shown by using multiple occurrences in order of priority. | 1--90 characters; see above | None

{.note} There is no obvious reason why the two contexts in which `LANG` appear have the same set of values but different character lengths.

### LEGA   {#LEGA}

`http://terms.fhiso.org/legacy/longform/LEGATEE`

A role of an individual acting as a person receiving a bequest or legal devise.

This tag does not appear in any known context.


### LATI   {#LATI}

`http://terms.fhiso.org/legacy/longform/LATITUDE`

A value indicating a coordinate position on a line, plane, or space.

{.note} This tag was introduced in GEDCOM 5.5.1.

For example:  18 degrees, 9 minutes, and 3.4 seconds North would be formatted as `N18.150944`.
Minutes and seconds are converted by dividing the minutes value by 60 and the seconds value by 3600 and adding the results together.
This sum becomes the fractional part of the degree's value.

{.ednote} The example in the GEDCOM spec uses 10 characters, while restricting it to 8.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`MAP.LATI` | The value specifying the latitudinal coordinate of the place name. The latitude coordinate is the
direction North or South from the equator in degrees and fraction of degrees carried out to give the
desired accuracy. | 5--8 characters | None


### LONG   {#LONG}

`http://terms.fhiso.org/legacy/longform/LONGITUDE`

A value indicating a coordinate position on a line, plane, or space.

{.note} This tag was introduced in GEDCOM 5.5.1.

For example:  168 degrees, 9 minutes, and 3.4 seconds East would be formatted as `E168.150944`.
Minutes and seconds are converted by dividing the minutes value by 60 and the seconds value by 3600 and adding the results together.
This sum becomes the fractional part of the degree's value.

{.ednote} The example in the GEDCOM spec uses 11 characters, while restricting it to 8.

Contexts
:   `MAP.LONG`

Description
:   The value specifying the longitudinal coordinate of the place name. The longitude coordinate is Degrees and fraction of degrees east or west of the zero or base meridian coordinate.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 8 characters.

Substructures
:   None


### MAP   {#MAP}

`http://terms.fhiso.org/legacy/longform/MAP`

Pertains to a representation of measurements usually presented in a graphical form.

{.note} This tag was introduced in GEDCOM 5.5.1.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`[Event]``.PLAC.MAP` | | None | `[LATI]`!, `[LONG]`!


### MARB   {#MARB}

`http://terms.fhiso.org/legacy/longform/MARRIAGE_BANN`

An event of an official public notice given that two people intend to marry.

Contexts
:   `.FAM.MARB`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARC   {#MARC}

`http://terms.fhiso.org/legacy/longform/MARR_CONTRACT`

An event of recording a formal agreement of marriage, including the prenuptial agreement in which marriage partners reach agreement about the property rights of one or both, securing property to their children.

Contexts
:   `.FAM.MARC`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARL   {#MARL}

`http://terms.fhiso.org/legacy/longform/MARR_LICENSE`

An event of obtaining a legal license to marry.

Contexts
:   `.FAM.MARL`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARR   {#MARR}

`http://terms.fhiso.org/legacy/longform/MARRIAGE`

A legal, common-law, or customary event of creating a family unit of a man and a woman as husband and wife.

Contexts
:   `.FAM.MARR`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARS   {#MARS}

`http://terms.fhiso.org/legacy/longform/MARR_SETTLEMENT`

An event of creating an agreement between two people contemplating marriage, at which time they agree to release or modify property rights that would otherwise arise from the marriage.

Contexts
:   `.FAM.MARS`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MEDI   {#MEDI}

`http://terms.fhiso.org/legacy/longform/MEDIA`

Identifies information about the media or having to do with the medium in which information is stored.

Contexts
:   `.SOUR.REPO.CALN.MEDI`

Description
:   A code, selected from one of the media classifications choices above, that indicates the type of material in which the referenced source is stored.

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 15 characters.
    
    The following are known values for the payload: {`audio`, `book`, `card`, `electronic`, `fiche`, `film`, `magazine`, `manuscript`, `map`, `newspaper`, `photo`, `tombstone`, `video`}

Substructures
:   None


### NAME   {#NAME}

`http://terms.fhiso.org/legacy/longform/NAME`

A word or combination of words used to help identify an individual, title, or other item. More than one NAME line should be used for people who were known by multiple names.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.SOUR.NAME` | The name of the software product that produced this transmission. | 1--90 characters | None
`.REPO.NAME` | The official name of the archive in which the stated source material is stored. | 1--90 characters | None
`.SUBM.NAME` | The name of the submitter formatted for display and address generation. | 1--60 characters | None
`.INDI.NAME` | The name value is formed in the manner the name is normally spoken, with the given name and family name (surname) separated by slashes `/`. | 1--120 characters, optionally with a substring offset by `/`, optionally with portions elided with `...` | `[NPFX]`?, `[GIVN]`?, `[NICK]`?, `[SPFX]`?, `[SURN]`?, `[NSFX]`?, `[SOUR]`\*, `[NOTE]`\*, `[FONE]`\*, `[ROMN]`\*


### NATI   {#NATI}

`http://terms.fhiso.org/legacy/longform/NATIONALITY`

The national heritage of an individual.

Contexts
:   `.INDI.NATI`

Description
:   The person's division of national origin or other folk, house, kindred, lineage, or tribal interest. Examples: Irish, Swede, Egyptian Coptic, Sioux Dakota Rosebud, Apache Chiricawa, Navajo Bitter Water, Eastern Cherokee Taliwa Wolf, and so forth.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### NATU   {#NATU}

`http://terms.fhiso.org/legacy/longform/NATURALIZATION`

The event of obtaining citizenship.

Contexts
:   `.INDI.NATU`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### NCHI   {#NCHI}

`http://terms.fhiso.org/legacy/longform/CHILDREN_COUNT`

The number of children that this person is known to be the parent of (all marriages) when subordinate to an individual, or that belong to this family when subordinate to a FAM_RECORD.

Known Context | Meaning | Payload | Supertype | Substructures
--------------|---------|---------|-----------|--------------
`.INDI.NCHI`  | The known number of children of this individual from all marriages. | 1--3 characters | `[IndividualAttribute]` | [*inherited*](#IndividualAttribute)
`.FAM.NCHI`   | The reported number of children known to belong to this family, regardless of whether the associated children are represented in the corresponding structure. | 1--3 characters | None | None

{.ednote} Presumably this is supposed to be a base-10 integer?


### NICK   {#NICK}

`http://terms.fhiso.org/legacy/longform/NICKNAME`

A descriptive or familiar that is used instead of, or in addition to, one's proper name.

Contexts
:   `.INDI.NAME.NICK`

Description
:   A descriptive or familiar name used in connection with one's proper name.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   None

{.ednote} The `NICK` grammar in GEDCOM is for a comma-separated list, but unlikes other parts of the name there is not descriptive text describing the meaning of the commas.


### NMR   {#NMR}

`http://terms.fhiso.org/legacy/longform/MARRIAGE_COUNT`

The number of times this person has participated in a family as a spouse or parent.

Contexts
:   `.INDI.NMR`

Description
:   The number of different families that this person was known to have been a member of as a spouse or parent, regardless of whether the associated families are represented in the dataset.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 3 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)

{.ednote} Presumably this is supposed to be a base-10 integer?


### NOTE   {#NOTE}

`http://terms.fhiso.org/legacy/longform/NOTE`

Additional information provided by the submitter for understanding the enclosing data.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.NOTE` | Comments or opinions from the submitter. | `[SOUR]`\*, `[REFN]`\*, `[RIN]`?, `[CHAN]`?
`.HEAD.NOTE` | A note that a user enters to describe the contents of the lineage-linked file in terms of "ancestors or descendants of" so that the person receiving the data knows what genealogical information the transmission contains. | arbitrary-length text | None
`NOTE` | Comments or opinions from the submitter. | pointer to a `NOTE` | `[SOUR]`\*
`NOTE` | Comments or opinions from the submitter. | arbitrary-length text | `[SOUR]`\*

Every structures that is known to have substructures is known to admit an arbitrary number of the unanchored `[NOTE]` substructures, except:

-   `[ADDR]`
-   `[ADOP]`.`[FAMC]`
-   `[CALN]`
-   `[DATE]`
-   `[FamilyEvent]``.``[HUSB]` and `[FamilyEvent]``.``[WIFE]`
-   .`[HEAD]` and its nested substructures
-   `[NOTE]` itself (recursive notes are not known to be permitted)
-   `[REFN]`
-   `[SOUR]`.`[DATA]`
-   `[SOUR]`.`[DATA]`.`[EVEN]`
-   `[SOUR]`.`[EVEN]`


### NPFX   {#NPFX}

`http://terms.fhiso.org/legacy/longform/NAME_PREFIX`

Text which appears on a name line before the given and surname parts of a name.
i.e. `Lt. Cmndr. Joseph /Allen/ jr.`
In this example `Lt. Cmndr.` is considered as the name prefix portion.

Contexts
:   `.INDI.NAME.NPFX`

Description
:   Non indexing name piece that appears preceding the given name and surname parts. Different name prefix parts are separated by a comma.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   None


### NSFX   {#NSFX}

`http://terms.fhiso.org/legacy/longform/NAME_SUFFIX`

Text which appears on a name line after or behind the given and surname parts of a name.
i.e. `Lt. Cmndr. Joseph /Allen/ jr.`
In this example `jr.` is considered as the name suffix portion.

Contexts
:   `.INDI.NAME.NSFX`

Description
:   Non-indexing name piece that appears after the given name and surname parts. Different name suffix parts are separated by a comma.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   None


### OBJE   {#OBJE}

`http://terms.fhiso.org/legacy/longform/OBJECT`

Pertaining to a grouping of attributes used in describing something. Usually referring to the data required to represent a multimedia object, such an audio recording, a photograph of a person, or an image of a document.

The meaning and use of OBJE changed significantly between GEDCOM 5.5 and GEDCOM 5.5.1

#### GEDCOM 5.5-compatible version

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.OBJE` | | None | `[FORM]`!, `[TITL]`?, `[NOTE]`\*, `[SOUR]`\*, `[BLOB]`!, `[OBJE]`?, `[REFN]`\*, `[RIN]`?, `[CHAN]`?
`.OBJE.OBJE` | chain to continued object | pointer to an `.OBJE` | None
`OBJE` | linked form | pointer to an `.OBJE` | None
`OBJE` | embedded form | None | `[FORM]`!, `[TITL]`?, `[FILE]`!, `[NOTE]`\*

#### GEDCOM 5.5.1-compatible version

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.OBJE` | | None | `[FILE]`+, `[REFN]`\*, `[RIN]`?, `[NOTE]`\*, `[SOUR]`\*, `[CHAN]`?
`OBJE` | linked form | pointer to an `.OBJE` | None
`OBJE` | embedded form | None | `[FILE]`+, `[TITL]`?

#### Shared Information

The both unanchored forms of `OBJE` are known to exist in the following contexts:

-   .`[FAM]``.OBJE`
-   .`[FAM]`.`[SOUR]``.OBJE`
-   .`[INDI]``.OBJE`
-   .`[SOUR]``.OBJE`
-   .`[SUBM]``.OBJE`
-   `[Event]``.OBJE`
-   `[Event]`.`[SOUR]``.OBJE`
-   `[NAME]`.`[SOUR]``.OBJE`
-   `[SOUR]``.OBJE` if the payload of the `[SOUR]` is a pointer, not text

{.note} The reason for the inconsistent presence/absence of `SOUR.OBJE` is not understood by the author of this document

{.ednote} The "if the payload is a pointer" distinction in where `OBJE` is known to occur is annoying in that the context specifier alone cannot describe this, but is embedded in the table for `[SOUR]` using just context specifiers so it isn't itself sufficient reason to revise the context specifier language

#### Common


### OCCU   {#OCCU}

`http://terms.fhiso.org/legacy/longform/OCCUPATION`

The type of work or profession of an individual.

Contexts
:   `.INDI.OCCU`

Description
:   The kind of activity that an individual does for a job, profession, or principal activity.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### ORDN   {#ORDN}

`http://terms.fhiso.org/legacy/longform/ORDINATION`

A religious event of receiving authority to act in religious matters.

Contexts
:   `.INDI.ORDN`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### PAGE   {#PAGE}

`http://terms.fhiso.org/legacy/longform/PAGE`

A number or description to identify where information can be found in a referenced work.

Contexts
:   `SOUR.PAGE`

Description
:   Specific location with in the information referenced. For a published work, this could include the volume of a multi-volume work and the page number(s). For a periodical, it could include volume, issue, and page numbers. For a newspaper, it could include a column number and page number. For an unpublished source, this could be a sheet number, page number, frame number, etc. A census record might have a line number or dwelling and family numbers in addition to the page number.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 248 characters.

Substructures
:   None


### PEDI   {#PEDI}

`http://terms.fhiso.org/legacy/longform/PEDIGREE`

Information pertaining to an individual to parent lineage chart.

Contexts
:   `.INDI.FAMC.PEDI`

Description
:   A code used to indicate the child to family relationship for pedigree navigation purposes.

Payload
:   one of {`adopted`, `birth`, `foster`}

Substructures
:   None

`adopted`
:   indicates adoptive parents.

`birth`
:   indicates birth parents.

`foster`
:   indicates child was included in a foster or guardian family.

### PHON   {#PHON}

`http://terms.fhiso.org/legacy/longform/PHONE`

A unique number assigned to access a specific telephone.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.SOUR.CORP.PHON` || 1--25 characters | None
`.REPO.PHON`  |         | 1--25 characters | None
`.SUBM.PHON`  |         | 1--25 characters | None
`ADDR.PHON`   |         | 1--25 characters | None


### PLAC   {#PLAC}

`http://terms.fhiso.org/legacy/longform/PLACE`

A jurisdictional name to identify the place or location of an event.

Places are often represented by a **place hierarchy**.
This is a comma-separated list of place names, each subsumed by the place to its right.
No accommodation for place names that include commas is made by this structure.

See also `[FORM]` for the interpretation of the elements of a place hierarchy list.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.PLAC`  | see `[FORM]` | None | `[FORM]`!
`.SOUR.DATa.EVEN.PLAC` | The name of the lowest jurisdiction that encompasses all lower-level places named in this source. For example, "Oneida, Idaho" would be used as a source jurisdiction place for events occurring in the various towns within Oneida County. "Idaho" would be the source jurisdiction place if the events recorded took place in other counties as well as Oneida County. | 1--120 characters formatted as a *place hierarchy* | None
`[Event]``.PLAC` | | 1--120 characters formatted as a *place hierarchy* | `[FORM]`?, `[SOUR]`\*, `[NOTE]`\*


### POST   {#POST}

`http://terms.fhiso.org/legacy/longform/POSTAL_CODE`

A code used by a postal service to identify an area to facilitate mail handling.

Contexts
:   `ADDR.POST`

Description
:   The ZIP or postal code used by the various localities in handling of mail. Isolated for sorting or indexing.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 10 characters.

Substructures
:   None


### PROB   {#PROB}

`http://terms.fhiso.org/legacy/longform/PROBATE`

An event of judicial determination of the validity of a will. May indicate several related court activities over several dates.

Contexts
:   `.INDI.PROB`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### PROP   {#PROP}

`http://terms.fhiso.org/legacy/longform/PROPERTY`

Pertaining to possessions such as real estate or other property of interest.

Contexts
:   `.INDI.ORDN`

Description
:   A list of possessions (real estate or other property) belonging to this individual.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 248 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### PUBL   {#PUBL}

`http://terms.fhiso.org/legacy/longform/PUBLICATION`

Refers to when and/or were a work was published or created.

When and where the record was created. For published works, this includes information such as the city of publication, name of the publisher, and year of publication.

For an unpublished work, it includes the date the record was created and the place where it was created. For example, the county and state of residence of a person making a declaration for a pension or the city and state of residence of the writer of a letter.

Contexts
:   `.SOUR.PUBL`

Description
:   see above

Payload
:   248 or more characters

Substructures
:   None

{.ednote} It is almost certainly an error in GEDCOM that asserts that every `PUBL` payload must be *at least* 248 characters and *cannot* have a newline in the first 248 characters.


### QUAY   {#QUAY}

`http://terms.fhiso.org/legacy/longform/QUALITY_OF_DATA`

An assessment of the certainty of the evidence to support the conclusion drawn from evidence.

The QUAY tag's value conveys the submitter's quantitative evaluation of the credibility of a piece of information, based upon its supporting evidence. Some systems use this feature to rank multiple conflicting opinions for display of most likely information first. It is not intended to eliminate the receiver's need to evaluate the evidence for themselves.

0
:   Unreliable evidence or estimated data

1
:   Questionable reliability of evidence (interviews, census, oral genealogies, or potential for bias for example, an autobiography)

2
:   Secondary evidence, data officially recorded sometime after event

3
:   Direct and primary evidence used, or by dominance of the evidence

Contexts
:   `SOUR.QUAY`

Description
:   see above

Payload
:   one of {`0`, `1`, `2`, `3`}

Substructures
:   None


### REFN   {#REFN}

`http://terms.fhiso.org/legacy/longform/REFERENCE`

A description or number used to identify an item for filing, storage, or other reference purposes.

Contexts
:   `REFN`

Description
:   A user-defined number or text that the submitter uses to identify this record. For instance, it may be a record number within the submitter's automated or manual system, or it may be a page and position number on a pedigree chart.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 20 characters.

Substructures
:   `[TYPE]`?

`REFN` is known to be a substructure of .`[FAM]`, .`[INDI]`, .`[NOTE]`, .`[OBJE]`, .`[REPO]`, and .`[SOUR]`.

### RELA   {#RELA}

`http://terms.fhiso.org/legacy/longform/RELATIONSHIP`

A relationship value between the indicated contexts.

Contexts
:   `ASSO.RELA`

Description
:   A word or phrase that states object 1's relation is object 2.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 25 characters.

Substructures
:   None

Example:
You would read the following as "Joe Jacob's great grandson is the person described by the record with id "jim":

````gedcom
0 INDI
  1 NAME Joe /Jacob/
  1 ASSO @jim@
    2 RELA great grandson
````

{.ednote} This example, does not have an ID for Joe; that is also true of the example in the GEDCOM specification, despite that specification not allowing `0 INDI` without an ID.


### RELI   {#RELI}

`http://terms.fhiso.org/legacy/longform/RELIGION`

A religious denomination to which a person is affiliated or for which a record applies.

Known Context | Meaning | Payload | Supertype | Substructures
--------------|---------|---------|-----------|--------------
`.INDI.RELI`  | A name of the religion with which this person, event, or record was affiliated. | 1--90 characters | `[IndividualAttribute]` | [*inherited*](#IndividualAttribute)
`[Event]``.RELI` | A name of the religion with which this person, event, or record was affiliated. | 1--90 characters | None | None

{.ednote} Although the text from the GEDCOM specification suggests `RELI` can be a substructure of `[SOUR]` and `[Event]`, the specification only lists it as an individual attribute.


### REPO   {#REPO}

`http://terms.fhiso.org/legacy/longform/REPOSITORY`

An institution or person that has the specified item as part of their collection(s).

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.REPO` | | None | `[NAME]`?, `[ADDR]`?, `[NOTE]`\*, `[REFN]`\*, `[RIN]`?, `[CHAN]`?
`.SOUR.REPO` | This structure is used within a source record to point to a name and address record of the holder of the source document. | pointer to `.REPO` | `[NOTE]`\*, `[CALN]`\*


### RESI   {#RESI}

`http://terms.fhiso.org/legacy/longform/RESIDENCE`

The act of dwelling at an address for a period of time.

Known Context | Meaning | Payload | Supertype | Substructures
--------------|---------|---------|-----------|--------------
`.INDI.RESI`  |  | None | `[IndividualAttribute]` | [*inherited*](#IndividualAttribute)

{.note} The `RESI` is the only known `[IndividualAttribute]` that does not have a payload.


### RESN   {#RESN}

`http://terms.fhiso.org/legacy/longform/RESTRICTION`

A processing indicator signifying access to information has been denied or otherwise restricted.

`confidential`
:   This data was marked as confidential by the user.  In some systems data marked as confidential will be treated differently, for example, there might be an option that would stop confidential data from appearing on printed reports or would prevent that information from being exported.

`locked`
:   Some records in Ancestral File have been satisfactorily proven by evidence, but because of source conflicts or incorrect traditions, there are repeated attempts to change this record. By arrangement, the Ancestral File Custodian can lock a record so that it cannot be changed without an agreement from the person assigned as the steward of such a record. The assigned steward is either the submitter listed for the record or Family History Support when no submitter is listed.

`privacy`
:   Information concerning this record is not present due to rights of or an approved request for privacy.

{.note} `confidential` was added in GEDCOM 5.5.1.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.INDI.RESN` | The restriction notice is defined for Ancestral File usage. Ancestral File download GEDCOM files may contain this data. | one of `confidential`, `locked`, or `privacy` | None
`.FAM.RESN` | The restriction notice is defined for Ancestral File usage. Ancestral File download GEDCOM files may contain this data. | one of `confidential`, `locked`, or `privacy` | None
`[Event]``.RESN` | The restriction notice is defined for Ancestral File usage. Ancestral File download GEDCOM files may contain this data. | one of `confidential`, `locked`, or `privacy` | None

{.ednote} I have not put this in the LDS section because, although it says "Ancestral File", I surmise it might be used for the same purpose by other data providers.


### RETI   {#RETI}

`http://terms.fhiso.org/legacy/longform/RETIREMENT`

An event of exiting an occupational relationship with an employer after a qualifying time period.

Contexts
:   `.INDI.RETI`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### RFN   {#RFN}

`http://terms.fhiso.org/legacy/longform/REC_FILE_NUMBER`

A permanent number assigned to a record that uniquely identifies it within a known file.


Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.INDI.RFN`   | see below | 1--90 characters | None
`.SUBM.RFN`   | A registered number of a submitter of Ancestral File data. This number is used in subsequent submissions or inquiries by the submitter for identification purposes. | 1--90 characters | None

An individual `RFN`

-   uniquely identifies this record within a registered network resource.
-   is usable as a cross-reference pointer.
-   may contain a colon (`:`) in which case the portion preceding the colon is an identifier assigned to a resource database that is available through access to a network and the portion following the colon is an an identification number assigned to each record within a specific database; or it may omit a colon and refer to a record within the current dataset.
-   marked in GEDCOM 5.5 as being "for future use".

{.ednote} It is not clear what benefit a `.INDI.RFN` has over an ID.


### RIN   {#RIN}

`http://terms.fhiso.org/legacy/longform/REC_ID_NUMBER`

A number assigned to a record by an originating automated system that can be used by a receiving system to report results pertaining to that record.

Contexts
:   `RIN`

Description
:   A unique record identification number assigned to the record by the source system. This number is intended to serve as a more sure means of identification of a record between two interfacing systems.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 12 characters.

Substructures
:   None

`RIN` is known to be a substructure of all records *except* `[HEAD]` and `[TRLR]`.


### ROLE   {#ROLE}

`http://terms.fhiso.org/legacy/longform/ROLE`

A name given to a role played by an individual in connection with an event.

Contexts
:   `SOUR.EVEN.ROLE`

Description
:   Either one of {`CHIL`, `HUSB`, `WIFE`, `MOTH`, `FATH`, `SPOU`}, or parentheses surrounding a word or phrase that identifies a person's role in an event being described---the same word or phrase, and in the same language, that the recorder used to define the role in the actual record.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 25 characters.

Substructures
:   None


### ROMN   {#ROMN}

`http://terms.fhiso.org/legacy/longform/ROMANIZED`

A romanized variation of a superior text string.

{.note} This tag was introduced in GEDCOM 5.5.1.

The method used to romanize the name is indicated by the subordinate `TYPE`; for example if romaji was used to provide a reading of a name written in kanji, then the `TYPE` subordinate to the `ROMN` tag would indicate `romaji`.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.INDI.NAME.ROMN` | The romanized variation of the name in the same form as the was the name used in the superior primitive.  | 1--120 characters | `[TYPE]`!, `[NPFX]`?, `[GIVN]`?, `[NICK]`?, `[SPFX]`?, `[SURN]`?, `[NSFX]`?, `[NOTE]`\*, `[SOUR]`\*
`[Event]``.PLAC.ROMN` | The romanized variation of the place name in the same form as the was the name used in the superior primitive. | `[TYPE]`!


### SEX   {#SEX}

`http://terms.fhiso.org/legacy/longform/SEX`

Indicates the sex of an individual---male or female.

Contexts
:   `.INDI.SEX`

Description
:   A code that indicates the sex of the individual

Payload
:   1--7 characters, which is one of {`M`, `F`}

Substructures
:   None


### SOUR   {#SOUR}

`http://terms.fhiso.org/legacy/longform/SOURCE`

The initial or original material from which information was obtained.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.SOUR` |  A system identification name which was obtained through the GEDCOM registration process. This name must be unique from any other product. Spaces within the name must be substituted with a 0x5F (underscore `_`) so as to create one word. | 1--20 characters | `[VERS]`?, `[NAME]`?, `[CORP]`?, `[DATA]`?
`.SOUR` | Source records are used to provide a bibliographic description of the source cited. | None | `[DATA]`?, `[AUTH]`?, `[TITL]`?, `[ABBR]`?, `[PUBL]`?, `[TEXT]`?, `[REPO]`?, `[OBJE]`\*, `[NOTE]`\*, `[REFN]`\*, `[RIN]`?, `[CHAN]`?
`SOUR` | pointer to source record | pointer to `.SOUR` | `[PAGE]`?, `[EVEN]`?, `[DATA]`?, `[QUAY]`?, `[OBJE]`\*, `[NOTE]`\*
`SOUR` | for systems not using source records | unlimited numbers of characters | `[TEXT]`\*, `[NOTE]`\*

The non-pointer unanchored `SOUR` is described as follows:

> A free form text block used to describe the source from which information was obtained. This text block is used by those systems which cannot use a pointer to a source record. It must contain a descriptive title, who created the work, where and when it was created, and where is source data stored. The developer should encourage users to use an appropriate style for forming this free form bibliographic reference. Developers are encouraged to support the `.SOUR` method of reporting bibliographic reference descriptions.

Both unanchored `SOUR` are known to be substructures of .`[FAM]`, .`[INDI]`, .`[OBJE]`, .`[NOTE]`, `[ASSO]`, `[Event]`, `[NAME]`, and `[PLAC]`.


### SPFX   {#SPFX}

`http://terms.fhiso.org/legacy/longform/SURN_PREFIX`

A name piece used as a non-indexing pre-part of a surname.

Contexts
:   `.INDI.NAME.SPFX`

Description
:   Surname prefix or article used in a family name. Different surname articles are separated by a comma, for example in the name "de la Cruz", this value would be "de, la".

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   None


### SSN   {#SSN}

`http://terms.fhiso.org/legacy/longform/SOC_SEC_NUMBER`

A number assigned by the United States Social Security Administration. Used for tax identification purposes.

See also `[IDNO]`

Contexts
:   `.INDI.SSN`

Description
:   A number assigned to a person in the United States for identification purposes.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 11 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### STAE   {#STAE}

`http://terms.fhiso.org/legacy/longform/STATE`

A geographical division of a larger jurisdictional area, such as a State within the United States of America.

Contexts
:   `ADDR.STAE`

Description
:   The name of the state used in the address. Isolated for sorting or indexing.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.

Substructures
:   None


### STAT   {#STAT}

`http://terms.fhiso.org/legacy/longform/STATUS`

An assessment of the state or condition of something.

{.note} Most uses of this tag are [LDS-specific tags].  The one documented here was introduced in GEDCOM 5.5.1.

Payload meanings are defined as

challenged
:   Linking this child to this family is suspect, but the linkage has been neither proven nor disproven.

disproven
:   There has been a claim by some that this child belongs to this family, but the linkage has been disproven.

proven
:   There has been a claim by some that this child does not belongs to this family, but the linkage has been proven.


Contexts
:   `.INDI.FAMC.STAT`

Description
:   A status code that allows passing on the users opinion of the status of a child to family link.

Payload
:   1--15 characters; one of `challenged`, `disproven`, or `proven`

Substructures
:   None



### SUBM   {#SUBM}

`http://terms.fhiso.org/legacy/longform/SUBMITTER`

An individual or organization who contributes genealogical data to a file or transfers it to someone else.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.SUBM`       | The submitter record identifies an individual or organization that contributed information contained in the dataset. | None | `[NAME]`!, `[ADDR]`?, `[OBJE]`\*, `[LANG]`\*, `[RFN]`?, `[RIN]`?, `[CHAN]`?
`.HEAD.SUBM`  |         | pointer to `.SUBM` | None
`.FAM.SUBM`   |         | pointer to `.SUBM` | None
`.INDI.SUBM`  |         | pointer to `.SUBM` | None
`.SUBN.SUBM`  |         | pointer to `.SUBM` | None


### SUBN   {#SUBN}

`http://terms.fhiso.org/legacy/longform/SUBMISSION`

Pertains to a collection of data issued for processing.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.SUBN` | The sending system uses a submission record to send instructions and information to the receiving system. | None | `[SUBM]`?, `[ANCE]`?, `[DESC]`?, `[RIN]`?; also various [LDS-specific tags]
`.HEAD.SUBN` | | pointer to a `.SUBN` | None


### SURN   {#SURN}

`http://terms.fhiso.org/legacy/longform/SURNAME`

A family name passed on or used by members of a family.

Contexts
:   `.INDI.NAME.SURN`

Description
:   Surname or family name. Different surnames are separated by a comma.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None


### TEXT   {#TEXT}

`http://terms.fhiso.org/legacy/longform/TEXT`

The exact wording found in an original source document.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`SOUR.TEXT`   | A verbatim copy of any description contained within the source. This indicates notes or text that are actually contained in the source document, not the submitter's opinion about the source. | arbitrary-length string | None
`SOUR.DATA.TEXT` | A verbatim copy of any description contained within the source. This indicates notes or text that are actually contained in the source document, not the submitter's opinion about the source. | arbitrary-length string | None


### TIME   {#TIME}

`http://terms.fhiso.org/legacy/longform/TIME`

A time value in a 24-hour clock format, including hours, minutes, and optional seconds, separated by a colon (:). Fractions of seconds are shown in decimal notation.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.DATE.TIME` | The time of a specific event, usually a computer-timed event. | 1--12 characters matching `[0-2][0-9]:[0-5][0-9](:[0-5][0-9](\.[0-9][0-9]?)?)?` | None
`CHAN.DATE.TIME` | The time of a specific event, usually a computer-timed event. | 1--12 characters matching `[0-2][0-9]:[0-5][0-9](:[0-5][0-9](\.[0-9]+)?)?` | None


### TITL   {#TITL}

`http://terms.fhiso.org/legacy/longform/TITLE`

A description of a specific writing or other work, such as the title of a book when used in a source context, or a formal designation used by an individual in connection with positions of royalty or other social status, such as Grand Duke.

Known Context | Meaning | Payload | Supertype | Substructures
--------------|---------|---------|-----------|--------------
`OBJE.TITL` | The title of a work, record, item, or object. | 1--248 characters | None | None
`.SOUR.TITL` | The title of the work, record, or item and, when appropriate, the title of the larger work or series of which it is a part. | arbtrary-length string | None | None
`.INDI.TITL` | The title given to or used by a person, especially of royalty or other noble class within a locality. | 1--120 characters | `[IndividualAttribute]` | [inherited](Event)
`.OBJE.FILE.TITL` | The title of a work, record, item, or object. | 1--248 characters | None | None

{.note} In GEDCOM 5.5, `OBJE.TITL` can appear in both record and substructure `OBJE`; in 5.5.1 it may only appear in substructure `OBJE`.  In GEDCOM 5.5, `.OBJE.FILE.TITL` is not supported; in 5.5.1 it is.



### TRLR   {#TRLR}

`http://terms.fhiso.org/legacy/longform/TRAILER`

At level 0, specifies the end of a GEDCOM transmission.

Contexts
:   `.TRLR`

Description
:   End of dataset

Payload
:   None

Substructures
:   None

When encountering a `TRLR`, applications may cease parsing before even looking for a payload or substructures; it should thus always be the very last record in the dataset.  Additionally, it should never have an ID; some implementations may fail to parse it correctly if it does.


### TYPE   {#TYPE}

`http://terms.fhiso.org/legacy/longform/TYPE`

A further qualification to the meaning of the associated superior tag. The value does not have any computer processing reliability. It is more in the form of a short one or two word note that should be displayed any time the associated data is displayed.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`REFN.TYPE` | A user-defined definition of the `[REFN]`. | 1--40 characters | None
`[EVENT]``.TYPE` | A descriptor that should be used whenever the EVEN tag is used to define the event being cited. The event descriptor should use the same word or phrase and in the same language, when possible, as was used by the recorder of the event. | 1--90 characters | None
`FONE.TYPE` | Indicates the method used in transforming the text to the phonetic variation. | 5--30 characters (may be `hangul`, `kana`, or other) | None
`ROMN.TYPE` | Indicates the method used in transforming the text to a romanized variation. | 5--30 characters (may be `pinyin`, `romanji`, `wadegiles`, or other) | None


### VERS   {#VERS}

`http://terms.fhiso.org/legacy/longform/VERSION`

Indicates which version of a product, item, or publication is being used or referenced.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.HEAD.SOUR.VERS` | An identifier that represents the version level assigned to the associated product. It is defined and changed by the creators of the product. | 1--15 characters | None
`.HEAD.GEDC.VERS` | An identifier that represents the version level assigned to the associated product. It is defined and changed by the creators of the product. | 1--15 characters | None
`.HEAD.CHAR.VERS` | An identifier that represents the version level assigned to the associated product. It is defined and changed by the creators of the product. | 1--15 characters | None

{.note} This draft is based off of a specification for which `.HEAD.GEDC.VERS` would typically be `5.5`, though depending on the specific features used other version strings might be appropriate too.


### WIFE   {#WIFE}

`http://terms.fhiso.org/legacy/longform/WIFE`

An individual in the role as a mother and/or married woman.

Known Context | Meaning | Payload | Substructures
--------------|---------|---------|--------------
`.FAM.WIFE` | | pointer to a `[INDI]` | None
`[FamilyEvent]``.WIFE` | | None | `[AGE]`!



### WILL   {#WILL}

`http://terms.fhiso.org/legacy/longform/WILL`

A legal document treated as an event, by which a person disposes of his or her estate, to take effect after death. The event date is the date the will was signed while the person was alive. (See also `[PROB]`ate.)

Contexts
:   `.INDI.WILL`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### WWW   {#WWW}

`http://terms.fhiso.org/legacy/longform/WEB`

World Wide Web home page.

{.note} This tag was introduced in GEDCOM 5.5.1.

Contexts
:   `ADDR.WWW`

Description
:   The world wide web page address.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None
