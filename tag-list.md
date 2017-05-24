---
title: "Extended Legacy Format (ELF)"
subtitle: Lineage-Linked Form
date: 18 May 2017
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
-   *context specifier*`.TAGNAME` matches any structure or pointer with that tag, provided it is a substructure of a structure specified by the *context specifier*
-   `(TAGNAME)` refers to any structure with that tag name or any tag inherited from it.

{.ednote} we do not currently use the `..` syntax in this document

{.ednote} We might also want to define `@TAGNAME` to refer to any structure with that tag name that has, as its payload, a pointer; and `#TAGNAME` to refer to any structure with that tag name that does not have, as its payload, a pointer; these might be necessary to discuss different kinds of `OBJE` and `SOUR` tags that cannot be distinguished from context alone.

The **cardinality** of a substructure specifies with what plurality it appears within its containing structure.  Cardinality is specified by adding one of the following suffixes to a tag name:

- `!` means "required": there must be one and only one of these.
- `?` means "optional": there must be either zero or one of these.
- `*` means "list-valued": there may be zero or more of these.  Some implementations may provide a maximum number for some or all set-valued substructures.
- `+` means "non-empty list-valued": there must be one and may be more than one of these. In the current spec, `+` only appears for the .`[OBJE]`.`[FILE]`.

The order of list-valued substructures sharing a single tag is significant; unless otherwise specified, the order is interpreted as the submitter's preference, with the most preferred value first and subsequent elements listed in decreasing order of preference. The exact meaning of "preference" is not defined by this specification.

{.example} A researcher unsure when a person was born would include two `BIRT` substructures within the `INDI`, with the one the researcher deems more probably true first.

{.example} A researcher documenting an individual with several names could include the *researcher-preferred* name first; or could include the name the individual preferred at some point first; etc.

{.example} Although an `INDI` is listed as having substructure (`[IndividualEvent]`)\*, the order of substructure `ADOP` and `CENS` are *not* significant because they differ in tag name.


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

Indicates an interest in additional research for ancestors of this individual. 

See also `[DESI]`

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

Indicates an interest in research to identify additional descendants of this individual.

See also `[ANCI]`

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

Supertype
:   `[Record]`

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


### FILE   {#FILE}

An information storage place that is ordered and arranged for preservation and reference.

Contexts
:   `[OBJE]`.`[FILE]`
:   .`[HEAD]`.`[FILE]`

#### Context `[OBJE]`.`[FILE]`

The substructures of the FILE tag were changed between GEDCOM 5.5 and GEDCOM 5.5.1.
In particular, the substructures listed here were placed under `[FILE]` in 5.5.1;
prior to that they appeared under `[OBJE]` instead.

Implementations SHOULD support importing data using both formats;
it is RECOMMENDED that they export data only in the 5.5.1 format with `[OBJE]`.`[FILE]`.`[FORM]` instead of `[OBJE]`.`[FORM]`.

Description
:   A complete local or remote file reference to the auxiliary data to be linked to the GEDCOM context. Remote reference would include a network address where the multimedia data may be obtained.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   `[FORM]`! -- this MAY be omitted, but if so then it MUST appear in the containing `[OBJE]`
:   `[TITL]`? -- if `[FORM]` is omitted from `[FILE]`, or if there is an `[OBJE]`.`[TITL]` inthe containing `[OBJE]`, then `[TITL]` SHOULD NOT be included as a substructure of `[FILE]`.


#### Context .`[HEAD]`.`[FILE]`

Description
:   The name of the GEDCOM transmission file. If the file name includes a file extension it must be shown in the form (filename.ext).

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Substructures
:   None


### FONE   {#FONE}

A phonetic variation of a superior text string.

{.note} This tag was introduced in GEDCOM 5.5.1.

The method of phonetic writing is indicated by the subordinate `TYPE` value; for example if hiragana was used to provide a reading of a name written in kanji, then the `TYPE` value would indicate `kana`.

Contexts
:   .`[INDI]`.`[NAME]`.`[FONE]`
:   (`[Event]`).`[PLAC]`.`[FONE]`

Description
:   The phonetic variation of the name in the same form as the was the name used in its superstructure

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

#### Context .`[INDI]`.`[NAME]`.`[FONE]`

Substructures
:   `[TYPE]`!
:   `[NPFX]`?
:   `[GIVN]`?
:   `[NICK]`?
:   `[SPFX]`?
:   `[SURN]`?
:   `[NSFX]`?
:   `[NOTE]`\*
:   `[SOUR]`\*

Generally, `[NAME]`.`[FONE]`'s substructures should mirror those of the superstructure, but represent the payloads phonetically 

#### Context (`[Event]`).`[PLAC]`.`[FONE]`

Substructures
:   `[TYPE]`!



### FORM   {#FORM}

An assigned name given to a consistent format in which information can be conveyed.


Contexts
:   .`[HEAD]`.`[GEDC]`.`[FORM]`
:   .`[HEAD]`.`[PLAC]`.`[FORM]`
:   (`[Event]`).`[PLAC]`.`[FORM]`
:   `[OBJE]`.`[FORM]`
:   `[OBJE]`.`[FILE]`.`[FORM]`

#### Context .`[HEAD]`.`[GEDC]`.`[FORM]`

Description
:   The GEDCOM form used to construct this transmission.
    
Payload
:   A string, specifically `LINEAGE_LINKED`.
    Implementations capable of parsing multiple formats may accept other values for this string.

Substructures
:   None


#### Context .`[HEAD]`.`[PLAC]`.`[FORM]`

Description
:   Implies that all place names follow this jurisdictional format and each jurisdiction is accounted for by a comma, whether the name is known or not. May be overridden by (`[Event]`).`[PLAC]`.`[FORM]`.
    
Payload
:   A string containing an ordered list of jurisdictional entities, separated by commas.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None

#### Context (`[Event]`).`[PLAC]`.`[FORM]`

Description
:   Jurisdictional entities that are named in a sequence from the lowest to the highest jurisdiction.
    The jurisdictions are separated by commas, and any jurisdiction's name that is missing is still accounted for by a comma.
    
Payload
:   A string containing an ordered list of jurisdictional entities, separated by commas.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None


#### Contexts `[OBJE]`.`[FORM]` and `[OBJE]`.`[FILE]`.`[FORM]`

Description
:   Indicates the format of the multimedia data with this tag.

Payload
:   one of {`bmp`, `gif`, `jpeg`, `ole`, `pcx`, `tiff`, `wav`}

Substructures
:   None

{.note} These two contexts differ primarily in the GEDCOM version in which they were introduced.  For more, see the discussion under `[OBJE]`


### GEDC   {#GEDC}

Information about the use of GEDCOM in a transmission.

Contexts
:   .`[HEAD]`.`[GEDC]`  

Payload
:    None    

Substructures
:   `[VERS]`!
:   `[FORM]`!


### GIVN   {#GIVN}

A given or earned name used for official identification of a person.

Contexts
:   .`[INDI]`.`[NAME]`.`[GIVN]`   

Description
:     Given name or earned name.

Payload
:   A string containing a comma-separated list of names.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    It is RECOMMENDED that implementations support at least 90 characters between each comma.

Substructures
:    None


### GRAD   {#GRAD}

An event of awarding educational diplomas or degrees to individuals.

Contexts
:   .`[INDI]`.`[GRAD]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### HEAD   {#HEAD}

Identifies information pertaining to an entire GEDCOM transmission.

Contexts
:   .`[HEAD]`

Payload
:    None    

Supertype
:   `[TopLevel]`

Substructures
:   `[SOUR]`!
:   `[DEST]`?
:   `[DATE]`?
:   `[SUBM]`!
:   `[SUBN]`?
:   `[FILE]`?
:   `[COPR]`?
:   `[GEDC]`!
:   `[CHAR]`!
:   `[LANG]`?
:   `[PLAC]`?
:   `[NOTE]`?


### HUSB   {#HUSB}

An individual in the family role of a married man or father.

Contexts
:   .`[FAM]`.(`[FamilyEvent]`).`[HUSB]`
:   .`[FAM]`.`[HUSB]`

#### Context .`[FAM]`.(`[FamilyEvent]`).`[HUSB]`

Payload
:   None

Substructures
:   `[AGE]`!

#### Context .`[FAM]`.`[HUSB]`

Payload
:   pointer to an `[INDI]`

Substructures
:   None


### IDNO   {#IDNO}

A number assigned to identify a person within some significant external system.

{.note} Although called a "number", the payload of an `[IDNO]` is *not* restricted to numeric values.

See also `[SSN]`

Contexts
:   .`[INDI]`.`[IDNO]`

Description
:   A nationally-controlled number assigned to an individual.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)
:   `[TYPE]`!


### IMMI   {#IMMI}

An event of entering into a new locality with the intent of residing there.

Contexts
:   .`[INDI]`.`[IMMI]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None, as described under `[IndividualEvent]`

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### INDI   {#INDI}

A person.

Contexts
:   `.INDI`

Description
:   The individual record is a compilation of facts, known or discovered, about an individual. Sometimes these facts are from different sources. This form allows documentation of the source where each of the facts were discovered.

Payload
:   None

Substructures
:   `[RESN]`?
:   `[NAME]`\*
:   `[SEX]`?
:   (`[IndividualEvent]`)\*, 
:   (`[IndividualAttribute]`)\*
:   `[FAMC]`\*
:   `[FAMS]`\*
:   `[SUBM]`\*
:   `[ASSO]`\*
:   `[ALIA]`\*
:   `[ANCI]`\*
:   `[DESI]`\*
:   `[SOUR]`\*
:   `[OBJE]`\*
:   `[NOTE]`\*
:   `[RFN]`?
:   `[REFN]`\*
:   `[RIN]`?
:   `[CHAN]`?


### LANG   {#LANG}

The name of the language used in a communication or transmission of information.

Contexts
:   .`[HEAD]`.`[LANG]`
:   .`[SUBM]`.`[LANG]`

Description
:   Refers to a human language.
    
    Within a `[HEAD]`, indicates the human language in which the data in the transmission is normally read or written.
    
    Within a `[SUBM]`, indicates a language in which a person prefers to communicate.
    Multiple language preference is shown by using multiple occurrences in order of priority.

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 15 characters.
    It is RECOMMENDED that implementations use one of the following known language payloads:
    {`Afrikaans`, `Albanian`, `Amharic`, `Anglo-Saxon`, `Arabic`, `Armenian`, `Assamese`, `Belorusian`, `Bengali`, `Braj`, `Bulgarian`, `Burmese`, `Cantonese`, `Catalan`, `Catalan_Spn`, `Church-Slavic`, `Czech`, `Danish`, `Dogri`, `Dutch`, `English`, `Esperanto`, `Estonian`, `Faroese`, `Finnish`, `French`, `Georgian`, `German`, `Greek`, `Gujarati`, `Hawaiian`, `Hebrew`, `Hindi`, `Hungarian`, `Icelandic`, `Indonesian`, `Italian`, `Japanese`, `Kannada`, `Khmer`, `Konkani`, `Korean`, `Lahnda`, `Lao`, `Latvian`, `Lithuanian`, `Macedonian`, `Maithili`, `Malayalam`, `Mandrin`, `Manipuri`, `Marathi`, `Mewari`, `Navaho`, `Nepali`, `Norwegian`, `Oriya`, `Pahari`, `Pali`, `Panjabi`, `Persian`, `Polish`, `Portuguese`, `Prakrit`, `Pusto`, `Rajasthani`, `Romanian`, `Russian`, `Sanskrit`, `Serb`, `Serbo_Croa`, `Slovak`, `Slovene`, `Spanish`, `Swedish`, `Tagalog`, `Tamil`, `Telugu`, `Thai`, `Tibetan`, `Turkish`, `Ukrainian`, `Urdu`, `Vietnamese`, `Wendic`, `Yiddish`}

Substructures
:   None



### LEGA   {#LEGA}

A role of an individual acting as a person receiving a bequest or legal devise.

In the GEDCOM standard, this tag is documented as existing but does not appear in any known context.

{.ednote} Should we guess that this is an [IndividualEvent] or remove it from our documentation?



### LATI   {#LATI}

A value indicating a coordinate position on a line, plane, or space.

{.note} This tag was introduced in GEDCOM 5.5.1.

For example:  18 degrees, 9 minutes, and 3.4 seconds North would be formatted as `N18.150944`.
Minutes and seconds are converted by dividing the minutes value by 60 and the seconds value by 3600 and adding the results together.
This sum becomes the fractional part of the degree's value.


Contexts
:   `[MAP]`.`[LATI]`

Description
:   The value specifying the latitudinal coordinate of the place name. The latitude coordinate is the direction North or South from the equator in degrees and fraction of degrees carried out to give the desired accuracy.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 10 characters.

    The string should match the regular expression 

    > `[NS]\d+(\.\d+)?`

{.ednote} In the GEDCOM 5.5.1 specification, the payload is specified as being no more than 8 characters but the example given therein 10 characters.

Substructures
:   None



### LONG   {#LONG}

A value indicating a coordinate position on a line, plane, or space.

{.note} This tag was introduced in GEDCOM 5.5.1.

For example:  168 degrees, 9 minutes, and 3.4 seconds East would be formatted as `E168.150944`.
Minutes and seconds are converted by dividing the minutes value by 60 and the seconds value by 3600 and adding the results together.
This sum becomes the fractional part of the degree's value.

{.ednote} The example in the GEDCOM spec uses 11 characters, while restricting it to 8.

Contexts
:   `[MAP].[LONG]`

Description
:   The value specifying the longitudinal coordinate of the place name. The longitude coordinate is Degrees and fraction of degrees east or west of the zero or base meridian coordinate.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 11 characters.

    The string should match the regular expression 

    > `[EW]\d+(\.\d+)?`

{.ednote} In the GEDCOM 5.5.1 specification, the payload is specified as being no more than 8 characters but the example given therein 11 characters.

Substructures
:   None



### MAP   {#MAP}

Pertains to a representation of measurements usually presented in a graphical form.

{.note} This tag was introduced in GEDCOM 5.5.1.

Contexts
:   (`[Event]`).`[MAP]`

Payload
:   None

Substructures
:   `[LATI]`!
:   `[LONG]`!



### MARB   {#MARB}

An event of an official public notice given that two people intend to marry.

Contexts
:   .`[FAM]`.`[MARB]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARC   {#MARC}

An event of recording a formal agreement of marriage, including the prenuptial agreement in which marriage partners reach agreement about the property rights of one or both, securing property to their children.

Contexts
:   .`[FAM]`.`[MARC]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARL   {#MARL}

An event of obtaining a legal license to marry.

Contexts
:   .`[FAM]`.`[MARL]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARR   {#MARR}

A legal, common-law, or customary event of creating a family unit of a man and a woman as husband and wife.

Contexts
:   .`[FAM]`.`[MARR]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)


### MARS   {#MARS}

An event of creating an agreement between two people contemplating marriage, at which time they agree to release or modify property rights that would otherwise arise from the marriage.

Contexts
:   .`[FAM]`.`[MARS]`

Description
:   *see `[FamilyEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[FamilyEvent]`

Substructures
:   [*inherited*](#FamilyEvent)




### MEDI   {#MEDI}

Identifies information about the media or having to do with the medium in which information is stored.

Contexts
:   .`[SOUR]`.`[REPO]`.`[CALN]`.`[MEDI]`

{.note} Due to an example in the GEDCOM specification that is inconsistent with the grammar, it is RECOMMENDED that implementations parse a .`[SOUR]`.`[REPO]`.`[MEDI]` (i.e., coordinate with instead of subordinate to `CALN`) as if they were .`[SOUR]`.`[REPO]`.`[CALN]`.`[MEDI]`.

Description
:   A code, selected from one of the media classifications choices listed under *Payload*, that indicates the type of material in which the referenced source is stored.

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 15 characters.
    
    The following are known values for the payload: {`audio`, `book`, `card`, `electronic`, `fiche`, `film`, `magazine`, `manuscript`, `map`, `newspaper`, `photo`, `tombstone`, `video`}

Substructures
:   None


### NAME   {#NAME}

A word or combination of words used to help identify an individual, title, or other item. More than one NAME line should be used for people who were known by multiple names.


Contexts
:   .`[HEAD]`.`[SOUR]`.`[NAME]`
:   .`[REPO]`.`[NAME]`
:   .`[SUBM]`.`[NAME]`
:   .`[INDI]`.`[NAME]`

#### Context .`[HEAD]`.`[SOUR]`.`[NAME]`

Description
:   The name of the software product that produced this transmission.

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
Substructures
:   None

#### Context .`[REPO]`.`[NAME]`

Description
:   The official name of the archive in which the stated source material is stored.

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
Substructures
:   None

#### Context .`[SUBM]`.`[NAME]`

Description
:   The name of the submitter formatted for display and address generation.

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
Substructures
:   None


#### Context .`[INDI]`.`[NAME]`

Description
:   The name value is formed in the manner the name is normally spoken.

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    The family name (surname) SHOULD be surrounded by U+002F SOLIDUS `/`.
    If the family name is unknown, a pair of soliduses with nothing between them SHOULD be included.
    If the given name is unknown, the entire payload should be surrounded by soliduses.
    
    Portions of the name may be elided and replaced by three  U+002E FULL STOP `...`.
    
Substructures
:   `[NPFX]`?
:   `[GIVN]`?
:   `[NICK]`?
:   `[SPFX]`?
:   `[SURN]`?
:   `[NSFX]`?
:   `[SOUR]`\*
:   `[NOTE]`\*
:   `[FONE]`\*
:   `[ROMN]`\*


### NATI   {#NATI}

The national heritage of an individual.

Contexts
:   .`[INDI]`.`[NATI]`

Description
:   The person's division of national origin or other folk, house, kindred, lineage, or tribal interest. Examples: Irish, Swede, Egyptian Coptic, Sioux Dakota Rosebud, Apache Chiricawa, Navajo Bitter Water, Eastern Cherokee Taliwa Wolf, and so forth.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### NATU   {#NATU}

The event of obtaining citizenship.

Contexts
:   .`[INDI]`.`[NATU]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### NCHI   {#NCHI}

The number of children that this person is known to be the parent of (all marriages) when specified for an `[INDI]`, or that belong to this family when specified for an `[FAM]`.


Contexts
:   .`[INDI]`.`[NCHI]`
:   .`[FAM]`.`[NCHI]`

Payload
:   A string.
    It is RECOMMENDED that implementations support payloads of at least 3 characters.
    
{.ednote} Although not specified in GEDCOM, this string should represent a base-10 integer.

#### Context .`[INDI]`.`[NCHI]`

Description
:   The known number of children of this individual from all marriages.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


#### Context .`[FAM]`.`[NCHI]`

Description
:   The reported number of children known to belong to this family, regardless of whether the associated children are represented in the corresponding structure.

Supertype
:   None

Substructures
:   None


### NICK   {#NICK}

A descriptive or familiar that is used instead of, or in addition to, one's proper name.

Contexts
:   .`[INDI]`.`[NAME]`.`[NICK]`

Description
:   A descriptive or familiar name used in connection with one's proper name.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

{.ednote} The `NICK` grammar in GEDCOM is for a comma-separated list, but unlikes other parts of the name there is no descriptive text specifying the meaning of the commas.

Substructures
:   None


### NMR   {#NMR}

The number of times this person has participated in a family as a spouse or parent.

Contexts
:   .`[INDI]`.`[NMR]`

Description
:   The number of different families that this person was known to have been a member of as a spouse or parent, regardless of whether the associated families are represented in the dataset.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 3 characters.

{.ednote} Although not specified in GEDCOM, this string should represent a base-10 integer.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)

{.ednote} Presumably this is supposed to be a base-10 integer?


### NOTE   {#NOTE}

Additional information provided by the submitter for understanding the enclosing data.

Contexts
:   .`[NOTE]`
:   .`[HEAD]`.`[NOTE]`
:   `[NOTE]`

#### Context .`[NOTE]`

Description
:   Comments or opinions from the submitter.

Payload
:   None

Substructures
:   `[SOUR]`\*
:   `[REFN]`\*
:   `[RIN]`?
:   `[CHAN]`?

#### Context .`[HEAD]`.`[NOTE]`

Description
:   A note that a user enters to describe the contents of the lineage-linked file in terms of "ancestors or descendants of" so that the person receiving the data knows what genealogical information the transmission contains.

Payload
:   String of arbitrary length.

Substructures
:   None

#### Context `[NOTE]`

Every structures that is known to have any substructures is known to admit an arbitrary number of the unanchored `[NOTE]` substructures *except*:

-   `[ADDR]`
-   `[ADOP]`.`[FAMC]`
-   `[CALN]`
-   `[DATE]`
-   (`[FamilyEvent]`).`[HUSB]` and (`[FamilyEvent]`).`[WIFE]`
-   .`[HEAD]` and its nested substructures
-   `[NOTE]` itself (recursive notes are not known to be permitted)
-   `[REFN]`
-   `[SOUR]`.`[DATA]`, `[SOUR]`.`[DATA]`.`[EVEN]`, and `[SOUR]`.`[EVEN]`

Description
:   Comments or opinions from the submitter.

Payload
:   *Either* String of arbitrary length *or* pointer to a `[NOTE]`.

Substructures
:   `[SOUR]`\*


### NPFX   {#NPFX}

Text which appears on a name line before the given and surname parts of a name.
i.e. `Lt. Cmndr. Joseph /Allen/ jr.`
In this example `Lt. Cmndr.` is considered as the name prefix portion.

Contexts
:   .`[INDI]`.`[NAME]`.`[NPFX]`

Description
:   Non indexing name piece that appears preceding the given name and surname parts. Different name prefix parts are separated by a comma.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   None


### NSFX   {#NSFX}

Text which appears on a name line after or behind the given and surname parts of a name.
i.e. `Lt. Cmndr. Joseph /Allen/ jr.`
In this example `jr.` is considered as the name suffix portion.

Contexts
:   .`[INDI]`.`[NAME]`.`[NSFX]`

Description
:   Non-indexing name piece that appears after the given name and surname parts. Different name suffix parts are separated by a comma.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

Substructures
:   None


### OBJE   {#OBJE}

Pertaining to a grouping of attributes used in describing something. Usually referring to the data required to represent a multimedia object, such an audio recording, a photograph of a person, or an image of a document.

The meaning and use of OBJE changed significantly between GEDCOM 5.5 and GEDCOM 5.5.1; it is RECOMMEND that implementations be able to handle parsing both.

Contexts
:   .`[OBJE]`
:   .`[OBJE]`.`[OBJE]` -- GEDCOM 5.5 only (not 5.5.1)
:   `[OBJE]`

    In particular, the unanchored `[OBJE]` appears in the following contexts:
    
    -   (`[Event]`).`[OBJE]`
    -   (`[Event]`).`[SOUR]`.`[OBJE]`
    -   .`[FAM]`.`[OBJE]`
    -   .`[FAM]`.`[SOUR]`.`[OBJE]`
    -   .`[INDI]`.`[OBJE]`
    -   `[NAME]`.`[SOUR]`.`[OBJE]`
    -   .`[SOUR]`.`[OBJE]`
    -   `[SOUR]`.`[OBJE]` (but only if the payload of the `[SOUR]` is a pointer, not a string)
    -   .`[SUBM]`.`[OBJE]`

{.note} The reason for the inconsistent presence/absence of `SOUR.OBJE` is not understood by the author of this document

#### Context .`[OBJE]`

Payload
:   None

Substructures
:   `[NOTE]`\*
:   `[SOUR]`\*
:   `[REFN]`\*
:   `[RIN]`?
:   `[CHAN]`?
:   `[FORM]`! -- GEDCOM 5.5 only
:   `[TITL]`? -- GEDCOM 5.5 only
:   `[BLOB]`! -- GEDCOM 5.5 only
:   `[OBJE]`? -- GEDCOM 5.5 only
:   `[FILE]`+ -- GEDCOM 5.5.1 only

#### Context .`[OBJE]`.`[OBJE]`

GEDCOM 5.5 only

Description
:   chain to continued object

Payload
:   Pointer to an .`[OBJE]`

Substructures
:   None

#### Context `[OBJE]`

Payload
:   *Either* pointer to an .`[OBJE]` *or* None

Substructures
:   *Either* None (if payload is a pointer) *or*
:   `[FORM]`! -- GEDCOM 5.5 only (not present in GEDCOM 5.5.1)
:   `[TITL]`?
:   `[FILE]`! (GEDCOM 5.5) or `[FILE]`+ (GEDCOM 5.5.1)
:   `[NOTE]`\* -- GEDCOM 5.5 only (not present in GEDCOM 5.5.1)


### OCCU   {#OCCU}


The type of work or profession of an individual.

Contexts
:   .`[INDI]`.`[OCCU]`

Description
:   The kind of activity that an individual does for a job, profession, or principal activity.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### ORDN   {#ORDN}

A religious event of receiving authority to act in religious matters.

Contexts
:   .`[INDI]`.`[ORDN]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### PAGE   {#PAGE}

A number or description to identify where information can be found in a referenced work.

Contexts
:   `[SOUR]`.`[PAGE]`

Description
:   Specific location with in the information referenced. For a published work, this could include the volume of a multi-volume work and the page number(s). For a periodical, it could include volume, issue, and page numbers. For a newspaper, it could include a column number and page number. For an unpublished source, this could be a sheet number, page number, frame number, etc. A census record might have a line number or dwelling and family numbers in addition to the page number.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 248 characters.

Substructures
:   None


### PEDI   {#PEDI}

Information pertaining to an individual to parent lineage chart.

Contexts
:   .`[INDI]`.`[FAMC]`.`[PEDI]`

Description
:   A code used to indicate the child to family relationship for pedigree navigation purposes.

Payload
:   one of {`adopted`, `birth`, `foster`}

    `adopted`
    :   indicates adoptive parents.

    `birth`
    :   indicates birth parents.

    `foster`
    :   indicates child was included in a foster or guardian family.

Substructures
:   None


### PHON   {#PHON}

A unique number assigned to access a specific telephone.

Contexts
:   .`[HEAD]`.`[SOUR]`.`[CORP]`.`[PHON]`
:   .`[REPO]`.`[PHON]`
:   .`[SUBM]`.`[PHON]`
:   `[ADDR]`.`[PHON]`

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 25 characters.

Substructures
:   None


### PLAC   {#PLAC}

A jurisdictional name to identify the place or location of an event.

Places are often represented by a **place hierarchy**.
This is a comma-separated list of place names, each subsumed by the place to its right.
No accommodation for place names that themselves include commas is made by this structure.

See also `[FORM]` for the interpretation of the elements of a place hierarchy list.

Contexts
:   .`[HEAD]`.`[PLAC]`
:   .`[SOUR]`.`[DATA]`.`[EVEN]`.`[PLAC]`
:   (`[Event]`).`[PLAC]`

#### Context .`[HEAD]`.`[PLAC]`

Description
:   see `[FORM]`

Payload
:   None

Substructures
:   `[FORM]`!

#### Context .`[SOUR]`.`[DATA]`.`[EVEN]`.`[PLAC]`

Description
:   The name of the lowest jurisdiction that encompasses all lower-level places named in this source. For example, "Oneida, Idaho" would be used as a source jurisdiction place for events occurring in the various towns within Oneida County. "Idaho" would be the source jurisdiction place if the events recorded took place in other counties as well as Oneida County.

Payload
:   A string formatted as a *place hierarchy*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None

#### Context (`[Event]`).`[PLAC]`

Payload
:   A string formatted as a *place hierarchy*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   `[FORM]`?
:   `[SOUR]`\*
:   `[NOTE]`\*


### POST   {#POST}

A code used by a postal service to identify an area to facilitate mail handling.

Contexts
:   `[ADDR]`.`[POST]`

Description
:   The ZIP or postal code used by the various localities in handling of mail. Isolated for sorting or indexing.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 10 characters.

Substructures
:   None


### PROB   {#PROB}

An event of judicial determination of the validity of a will. May indicate several related court activities over several dates.

Contexts
:   .`[INDI]`.`[PROB]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### PROP   {#PROP}

Pertaining to possessions such as real estate or other property of interest.

Contexts
:   .`[INDI]`.`[ORDN]`

Description
:   A list of possessions (real estate or other property) belonging to this individual.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 248 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### PUBL   {#PUBL}

Refers to when and/or were a work was published or created.

Contexts
:   .`[SOUR]`.`[PUBL]`

Description
:   When and where the record was created.
    
    For published works, this includes information such as the city of publication, name of the publisher, and year of publication.

    For an unpublished work, it includes the date the record was created and the place where it was created. For example, the county and state of residence of a person making a declaration for a pension or the city and state of residence of the writer of a letter.

Payload
:   A string of arbitrary length.

Substructures
:   None

{.ednote} GEDCOM asserts that every `PUBL` payload must be *at least* 248 characters and *cannot* have a newline in the first 248 characters; this is almost certainly an error and not reflected in this specification.


### QUAY   {#QUAY}

An assessment of the certainty of the evidence to support the conclusion drawn from evidence.

Contexts
:   `[SOUR]`.`[QUAY]`

Description
:   The QUAY tag's value conveys the submitter's quantitative evaluation of the credibility of a piece of information, based upon its supporting evidence. Some systems use this feature to rank multiple conflicting opinions for display of most likely information first. It is not intended to eliminate the receiver's need to evaluate the evidence for themselves.

Payload
:   one of {`0`, `1`, `2`, `3`}

    0
    :   Unreliable evidence or estimated data

    1
    :   Questionable reliability of evidence (interviews, census, oral genealogies, or potential for bias for example, an autobiography)

    2
    :   Secondary evidence, data officially recorded sometime after event

    3
    :   Direct and primary evidence used, or by dominance of the evidence


Substructures
:   None


### REFN   {#REFN}

A description or number used to identify an item for filing, storage, or other reference purposes.

Contexts
:   .`[FAM]`.`[REFN]`
:   .`[INDI]`.`[REFN]`
:   .`[NOTE]`.`[REFN]`
:   .`[OBJE]`.`[REFN]`
:   .`[REPO]`.`[REFN]`
:   .`[SOUR]`.`[REFN]`

Description
:   A user-defined number or text that the submitter uses to identify this record. For instance, it may be a record number within the submitter's automated or manual system, or it may be a page and position number on a pedigree chart.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 20 characters.

Substructures
:   `[TYPE]`?


### RELA   {#RELA}

A relationship value between the indicated contexts.

Contexts
:   `[ASSO]`.`[RELA]`

Description
:   A word or phrase that states object 1's relation is object 2.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 25 characters.

Substructures
:   None

**Example:**
You would read the following as "Joe Jacob's great grandson is the person described by the record with id "jim":

````gedcom
0 @joe@ INDI
  1 NAME Joe /Jacob/
  1 ASSO @jim@
    2 RELA great grandson
````


### RELI   {#RELI}

A religious denomination to which a person is affiliated or for which a record applies.

Contexts
:   .`[INDI]`.`[RELI]`
:   (`[Event]`).`[RELI]`

{.ednote} Although the text from the GEDCOM specification suggests `RELI` can be a substructure of `[SOUR]`, the specification does not list it in that context.

#### Context .`[INDI]`.`[RELI]`

Description
:   A name of the religion with which this person, event, or record was affiliated.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)

#### Context (`[Event]`).`[RELI]`

Description
:   A name of the religion with which this person, event, or record was affiliated.

Payload
:   None

Substructures
:   None


### REPO   {#REPO}

An institution or person that has the specified item as part of their collection(s).

Contexts
:   .`[REPO]`
:   .`[SOUR]`.`[REPO]`

#### Context .`[REPO]`

Payload
:   None

Substructures
:   `[NAME]`?
:   `[ADDR]`?
:   `[NOTE]`\*
:   `[REFN]`\*
:   `[RIN]`?
:   `[CHAN]`?

#### Context .`[SOUR]`.`[REPO]`

Description
:   This structure is used within a source record to point to a name and address record of the holder of the source document.

Payload
:   Pointer to a .`[REPO]`

Substructures
:   `[NOTE]`\*
:   `[CALN]`\*


### RESI   {#RESI}

The act of dwelling at an address for a period of time.

Contexts
:   .`[INDI]`.`[RESI]`

Payload
:   None

{.note} The `RESI` is the only known `[IndividualAttribute]` that does not have a payload.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### RESN   {#RESN}

A processing indicator signifying access to information has been denied or otherwise restricted.

Contexts
:   .`[INDI]`.`[RESN]`
:   .`[FAM]`.`[RESN]`
:   .(`[Event]`).`[RESN]`

Description
:   The restriction notice is defined for Ancestral File usage. Ancestral File download GEDCOM files may contain this data.

Payload
:   one of `confidential`, `locked`, or `privacy` (`confidential` was added in GEDCOM 5.5.1).

    `confidential`
    :   This data was marked as confidential by the user.  In some systems data marked as confidential will be treated differently, for example, there might be an option that would stop confidential data from appearing on printed reports or would prevent that information from being exported.

    `locked`
    :   Some records in Ancestral File have been satisfactorily proven by evidence, but because of source conflicts or incorrect traditions, there are repeated attempts to change this record. By arrangement, the Ancestral File Custodian can lock a record so that it cannot be changed without an agreement from the person assigned as the steward of such a record. The assigned steward is either the submitter listed for the record or Family History Support when no submitter is listed.

    `privacy`
    :   Information concerning this record is not present due to rights of or an approved request for privacy.

Substructures
:   None

{.ednote} Although the GEDCOM spec says "Ancestral File", we included this assuming it might be used for the same purpose by other data providers.


### RETI   {#RETI}

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

A permanent number assigned to a record that uniquely identifies it within a known file.

Contexts
:   .`[INDI]`.`[RFN]`
:   .`[SUBM]`.`[RFN]`

Description
:   A .`[SUBM]`.`[RFN]` is a registered number of a submitter of Ancestral File data. This number is used in subsequent submissions or inquiries by the submitter for identification purposes.
    
    An .`[INDI]`.`[RFN]`

    -   uniquely identifies this record within a registered network resource.
    -   is usable as a cross-reference pointer.
    -   may contain a colon (`:`) in which case the portion preceding the colon is an identifier assigned to a resource database that is available through access to a network and the portion following the colon is an an identification number assigned to each record within a specific database; or it may omit a colon and refer to a record within the current dataset.
    -   marked in GEDCOM 5.5 as being "for future use".

{.ednote} It is not clear to me if either RFN is used or ought to be part of ELF.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.

Substructures
:   None


### RIN   {#RIN}

A number assigned to a record by an originating automated system that can be used by a receiving system to report results pertaining to that record.

Contexts
:   .(`[Record]`).`[RIN]` -- but *not* .`[HEAD]`.`[RIN]` or .`[TRLR]`.`[RIN]`.

Description
:   A unique record identification number assigned to the record by the source system. This number is intended to serve as a more sure means of identification of a record between two interfacing systems.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 12 characters.

Substructures
:   None


### ROLE   {#ROLE}

A name given to a role played by an individual in connection with an event.

Contexts
:   `[SOUR]`.`[EVEN]`.`[ROLE]`


Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 25 characters.

    The payload SHOULD be either one of {`CHIL`, `HUSB`, `WIFE`, `MOTH`, `FATH`, `SPOU`}, or parentheses surrounding a word or phrase that identifies a person's role in an event being described---the same word or phrase, and in the same language, that the recorder used to define the role in the actual record.

Substructures
:   None


### ROMN   {#ROMN}

{.note} This tag was introduced in GEDCOM 5.5.1.

A romanized variation of a superior text string.

The method used to romanize the name is indicated by the subordinate `TYPE`; for example if romaji was used to provide a reading of a name written in kanji, then the `TYPE` subordinate to the `ROMN` tag would indicate `romaji`.

Contexts
:   .`[INDI]`.`[NAME]`.`[ROMN]`
:   (`[Event]`).`[PLAC]`.`[ROMN]`

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

#### Context .`[INDI]`.`[NAME]`.`[ROMN]`

Description
:   The romanized variation of the name in the same form as the was the name used in the superior primitive.

Substructures
:   `[TYPE]`!
:   `[NPFX]`?
:   `[GIVN]`?
:   `[NICK]`?
:   `[SPFX]`?
:   `[SURN]`?
:   `[NSFX]`?
:   `[NOTE]`\*
:   `[SOUR]`\*

#### Context (`[Event]`).`[PLAC]`.`[ROMN]`

Description
:   The romanized variation of the place name in the same form as the was the name used in the superior primitive.

Substructures
:   `[TYPE]`!


### SEX   {#SEX}

Indicates the sex of an individual---male or female.

Contexts
:   .`[INDI]`.`[SEX]`

Description
:   A code that indicates the sex of the individual

Payload
:   one of {`M`, `F`}

Substructures
:   None


### SOUR   {#SOUR}

The initial or original material from which information was obtained.

Contexts
:   .`[HEAD]`.`[SOUR]`
:   .`[SOUR]`
:   `[SOUR]`

    In particular, the unanchored `[SOUR]` appears in the following contexts:
    
    -   (`[Event]`).`[SOUR]`
    -   .`[FAM]`.`[SOUR]`
    -   .`[INDI]`.`[SOUR]`
    -   .`[NOTE]`.`[SOUR]`
    -   `[ASSO]`.`[SOUR]`
    -   `[NAME]`.`[SOUR]`
    -   `[PLAC]`.`[SOUR]`

#### Context .`[HEAD]`.`[SOUR]`

Description
:    A system identification name which was obtained through the GEDCOM registration process. This name must be unique from any other product. Spaces within the name must be substituted with a U+005F (underscore `_`) so as to create one word.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 20 characters.

Substructures
:   `[VERS]`?
:   `[NAME]`?
:   `[CORP]`?
:   `[DATA]`?

#### Context .`[SOUR]`

Description
:   Source records are used to provide a bibliographic description of the source cited.

Payload
:   None

Substructures
:   `[DATA]`?
:   `[AUTH]`?
:   `[TITL]`?
:   `[ABBR]`?
:   `[PUBL]`?
:   `[TEXT]`?
:   `[REPO]`?
:   `[OBJE]`\*
:   `[NOTE]`\*
:   `[REFN]`\*
:   `[RIN]`?
:   `[CHAN]`?

#### Context `[SOUR]`, pointer version

Payload
:   Pointer to a .`[SOUR]`

Substructures
:   `[PAGE]`?
:   `[EVEN]`?
:   `[DATA]`?
:   `[QUAY]`?
:   `[OBJE]`\*
:   `[NOTE]`\*

#### Context `[SOUR]`, text block version

It is RECOMMENDED that this version not be generated by software.

Description
:   A free form text block used to describe the source from which information was obtained. This text block is used by those systems which cannot use a pointer to a source record. It must contain a descriptive title, who created the work, where and when it was created, and where is source data stored. The developer should encourage users to use an appropriate style for forming this free form bibliographic reference.

Payload
:   A string of arbitrary length

Substructures
:   `[TEXT]`\*
:   `[NOTE]`\*


### SPFX   {#SPFX}

A name piece used as a non-indexing pre-part of a surname.

Contexts
:   .`[INDI]`.`[NAME]`.`[SPFX]`

Description
:   Surname prefix or article used in a family name.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.

    Different surname articles are separated by a comma, for example in the name "de la Cruz", this value would be "de, la".

Substructures
:   None


### SSN   {#SSN}

A number assigned by the United States Social Security Administration. Used for tax identification purposes.

See also `[IDNO]`

Contexts
:   .`[INDI]`.`[SSN]`

Description
:   A number assigned to a person in the United States for identification purposes.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 11 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)


### STAE   {#STAE}

A geographical division of a larger jurisdictional area, such as a State within the United States of America.

Contexts
:   `[ADDR].[STAE]`

Description
:   The name of the state used in the address. Isolated for sorting or indexing.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.

Substructures
:   None


### STAT   {#STAT}

An assessment of the state or condition of something.

{.note} Most uses of this tag in GEDCOM are LDS-specific and excluded from this specification.  The one documented here was introduced in GEDCOM 5.5.1.

Contexts
:   `.INDI.FAMC.STAT`

Description
:   A status code that allows passing on the users opinion of the status of a child to family link.

Payload
:   1--15 characters; one of `challenged`, `disproven`, or `proven`

    challenged
    :   Linking this child to this family is suspect, but the linkage has been neither proven nor disproven.

    disproven
    :   There has been a claim by some that this child belongs to this family, but the linkage has been disproven.

    proven
    :   There has been a claim by some that this child does not belongs to this family, but the linkage has been proven.

Substructures
:   None


### SUBM   {#SUBM}

An individual or organization who contributes genealogical data to a file or transfers it to someone else.

Contexts
:   .`[SUBM]`
:   .`[HEAD]`.`[SUBM]`
:   .`[FAM]`.`[SUBM]`
:   .`[INDI]`.`[SUBM]`
:   .`[SUBN]`.`[SUBM]`

Description
:   The submitter record identifies an individual or organization that contributed information contained in the dataset.

#### Context .`[SUBM]`

Payload
:   None

Substructures
:   `[NAME]`!
:   `[ADDR]`?
:   `[OBJE]`\*
:   `[LANG]`\*
:   `[RFN]`?
:   `[RIN]`?
:   `[CHAN]`?

#### Context (`[Record]`).`[SUBM]`

Payload
:   pointer to .`[SUBM]`

Substructures
:   None


### SUBN   {#SUBN}

Pertains to a collection of data issued for processing.

Contexts
:   .`[SUBN]`
:   .`[HEAD]`.`[SUBN]`

#### Context .`[SUBN]`

Description
:   The sending system uses a submission record to send instructions and information to the receiving system.

Payload
:   None

Substructures
:   `[SUBM]`?
:   `[ANCE]`?
:   `[DESC]`?
:   `[RIN]`?

#### Context .`[HEAD]`.`[SUBN]`

Payload
:   Pointer to a .`[SUBN]`

Substructures
:   None


### SURN   {#SURN}

A family name passed on or used by members of a family.

Contexts
:   .`[INDI]`.`[NAME]`.`[SURN]`

Description
:   Surname or family name. Different surnames are separated by a comma.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None


### TEXT   {#TEXT}

The exact wording found in an original source document.

Contexts
:   `[SOUR]`.`[TEXT]`
:   `[SOUR]`.`[DATA]`.`[TEXT]`

Description
:   A verbatim copy of any description contained within the source. This indicates notes or text that are actually contained in the source document, not the submitter's opinion about the source.

Payload
:   A string of arbitrary length

Substructures
:   None


### TIME   {#TIME}

A time value in a 24-hour clock format, including hours, minutes, and optional seconds, separated by a colon (`:`). Fractions of seconds are shown in decimal notation.

Contexts
:   .`[HEAD]`.`[DATE]`.`[TIME]`
:   .`[CHAN]`.`[DATE]`.`[TIME]`

Description
:   The time of a specific event, usually a computer-timed event.

Payload
:   A string.  It is RECOMMENDED that implementations support payloads of at least 12 characters.
    
    The string should match the regular expression 

    > `[0-2][0-9]:[0-5][0-9](:[0-5][0-9](\.[0-9][0-9]?)?)?`

Substructures
:   None


### TITL   {#TITL}

A description of a specific writing or other work, such as the title of a book when used in a source context, or a formal designation used by an individual in connection with positions of royalty or other social status, such as Grand Duke.

Contexts
:   .`[INDI]`.`[TITL]`
:   .`[OBJE]`.`[FILE]`.`[TITL]` -- in GEDCOM 5.5 but not 5.5.1
:   .`[OBJE]`.`[TITL]` -- in GEDCOM 5.5 but not 5.5.1
:   `[OBJE]`.`[TITL]`
:   .`[SOUR]`.`[TITL]`


#### Context .`[INDI]`.`[TITL]`

Description
:   The title given to or used by a person, especially of royalty or other noble class within a locality.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Supertype
:   `[IndividualAttribute]`

Substructures
:   [*inherited*](#IndividualAttribute)

#### Contexts .`[OBJE]`.`[FILE]`.`[TITL]` and `[OBJE]`.`[TITL]`

Description
:   The title of a work, record, item, or object.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 248 characters.

Substructures
:   None


#### Context .`[SOUR]`.`[TITL]`

Description
:   The title of the work, record, or item and, when appropriate, the title of the larger work or series of which it is a part.

Payload
:   A string of arbitrary length.

Substructures
:   None


### TRLR   {#TRLR}

Specifies the end of a GEDCOM transmission.

When encountering a `TRLR`, applications may cease parsing before even looking for a payload or substructures; it should thus always be the very last record in the dataset.  Additionally, it should never have an ID; some implementations may fail to parse it correctly if it does.

Contexts
:   .`[TRLR]`

Description
:   End of dataset

Payload
:   None

Supertype
:   `[TopLevel]`

Substructures
:   None


### TYPE   {#TYPE}

A further qualification to the meaning of the associated superior tag. The value does not have any computer processing reliability. It is more in the form of a short one or two word note that should be displayed any time the associated data is displayed.

Context
:   `[REFN]`.`[TYPE]`
:   (`[Event]`).`[TYPE]`
:   `[FONE]`.`[TYPE]`
:   `[ROMN]`.`[TYPE]`

Substructures
:   None

#### Context `[REFN]`.`[TYPE]`

Description
:   A user-defined definition of the `[REFN]`.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 40 characters.


#### Context (`[Event]`).`[TYPE]`

Description
:   A descriptor that should be used whenever the `[EVEN]` tag is used to define the event being cited. The event descriptor should use the same word or phrase and in the same language, when possible, as was used by the recorder of the event.

{.ednote} The description text from GEDCOM suggests `[EVEN]`.`[TYPE]`, but it is a known to appear as a substructure of all (`[Event]`)s.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 90 characters.


#### Context `[FONE]`.`[TYPE]`

Description
:   Indicates the method used in transforming the text to the phonetic variation.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.
    Known values include, but are not limited to, `hangul` and `kana`.


#### Context `[ROMN]`.`[TYPE]`

Description
:   Indicates the method used in transforming the text to the romanized variation.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 30 characters.
    Known values include, but are not limited to, `pinyin`, `romanji`, and `wadegiles`.


### VERS   {#VERS}

Indicates which version of a product, item, or publication is being used or referenced.

Contexts
:   .`[HEAD]`.`[SOUR]`.`[VERS]`
:   .`[HEAD]`.`[GEDC]`.`[VERS]`
:   .`[HEAD]`.`[CHAR]`.`[VERS]`

Description
:   An identifier that represents the version level assigned to the associated product. It is defined and changed by the creators of the product.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 15 characters.
    
{.note} This draft is based off of a specification for which `.HEAD.GEDC.VERS` would typically be `5.5.1`, though depending on the specific features used other version strings might be appropriate too.

Substructures
:   None


### WIFE   {#WIFE}

An individual in the role as a mother and/or married woman.

Contexts
:   .`[FAM]`.(`[FamilyEvent]`).`[WIFE]`
:   .`[FAM]`.`[WIFE]`

#### Context .`[FAM]`.(`[FamilyEvent]`).`[WIFE]`

Payload
:   None

Substructures
:   `[AGE]`!

#### Context .`[FAM]`.`[WIFE]`

Payload
:   pointer to an .`[INDI]`

Substructures
:   None


### WILL   {#WILL}

A legal document treated as an event, by which a person disposes of his or her estate, to take effect after death. The event date is the date the will was signed while the person was alive. 

See also `[PROB]`

Contexts
:   .`[INDI]`.`[WILL]`

Description
:   *see `[IndividualEvent]`*

Payload
:   Either `Y` or None

Supertype
:   `[IndividualEvent]`

Substructures
:   [*inherited*](#IndividualEvent)


### WWW   {#WWW}

World Wide Web home page.

{.note} This tag was introduced in GEDCOM 5.5.1.

Contexts
:   `[ADDR]`.`[WWW]`

Description
:   The world wide web page address.

Payload
:   A string. It is RECOMMENDED that implementations support payloads of at least 120 characters.

Substructures
:   None
