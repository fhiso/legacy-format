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

# Notation

This document uses tag names instead of URIs.
A mapping between tag name and URI is expected as part of a separate document or a future extension to the document.

A **structure** is either a *record* or a *substructure*.

**Records** appear at the top-level of an ELF dataset.
Known records types are

-   [HEAD] -- each data set has exactly one of these, at the beginning of the dataset.
-   [TRLR] -- each data set has exactly one of these, at the end of the dataset.
-   [SUBN] -- each data set has zero or one of these; if present, it immediately follows the [HEAD] record.
-    [FAM], [INDI], [OBJE], [NOTE], [REPO], [SOUR], and [SUBM] -- may be omitted or may appear in any number and any order.

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
- `+` means "non-empty set-valued": there must be one and may be more than one of these. In the current spec, `+` only appears for the .[OBJE].[FILE].

# Supertypes

A limited type hierarchy is known to exist:

- [Event]
    -   [IndividualEvent]
        - [ADOP]
        - [BAPM]
        - [BARM]
        - [BASM]
        - [BIRT]
        - [BLES]
        - [BURI]
        - [CENS]
        - [CHR]
        - [CHRA]
        - [CONF]
        - [CREM]
        - [DEAT]
        - [EMIG]
        - [FCOM]
        - [GRAD]
        - [IMMI]
        - [NATU]
        - [ORDN]
        - [PROB]
        - [RETI]
        - [WILL]
        - [EVEN] -- note that there is also an [EVEN] that is a subtype of [FamilyEvent]
    -   [FamilyEvent]
        - [ANUL]
        - [CENS]
        - [DIV]
        - [DIVF]
        - [ENGA]
        - [MARR]
        - [MARB]
        - [MARC]
        - [MARL]
        - [MARS]
        - [EVEN] -- note that there is also an [EVEN] that is a subtype of [IndividualEvent]
    -   [IndividualAttribute]
        - [CAST]
        - [DSCR]
        - [EDUC]
        - [IDNO]
        - [NATI]
        - [NCHI]
        - [NMR]
        - [OCCU]
        - [PROP]
        - [RELI]
        - [RESI]
        - [SSN]
        - [TITL]
        - [FACT] -- note that prior to GEDCOM 5.5.1, [EVEN] was used in place of [FACT]

All other tags in this document have no supertype.

# Known Tags and Contexts

## Abstract Types

The following tags are not encountered directly, but are instead present only as abstract supertypes from which other tags derive.



### Event

The *Event* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Known Subtypes:
:   -   [IndividualEvent]
    -   [FamilyEvent]
    -   [IndividualAttribute]

{.note} GEDCOM 5.5 also includes [AGE] as a substructure of [Event], but GEDCOM 5.5.1 moves that to [IndividualEvent] and [IndividualAttribute] instead.

#### Contexts

.[INDI].([Event])
:   description
    :   Varies by subtype
    
    payload
    :   Varies by subtype
    
    substructures
    :   -   [ADDR]?
        -   [AGNC]?
        -   [CAUS]?
        -   [DATE]?
        -   [NOTE]\*
        -   [OBJE]\*
        -   [PHON]\*
        -   [PLAC]?
        -   [SOUR]\*
        -   [TYPE]?
        -   [RESN]?
        -   [RELI]? 

### IndividualEvent

The *IndividualEvent* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Supertype
:   [Event]

Known Subtypes:
:   - [ADOP]
    - [BAPM]
    - [BARM]
    - [BASM]
    - [BIRT]
    - [BLES]
    - [BURI]
    - [CENS]
    - [CHR]
    - [CHRA]
    - [CONF]
    - [CREM]
    - [DEAT]
    - [EMIG]
    - [FCOM]
    - [GRAD]
    - [IMMI]
    - [NATU]
    - [ORDN]
    - [PROB]
    - [RETI]
    - [WILL]
    - [EVEN]

#### Contexts


.[INDI].([IndividualEvent])
:   description
    :   Varies by subtype
    
    payload
    :   Either the string `Y` or not present.
        If any of the following are true 

        - the payload is `Y` 
        - there is a [DATE] substructure
        - there is a [PLAC] substructure
        - the tag is [EVEN]

        then the IndividualEvent asserts that the event occurred; otherwise it just discusses the idea of the event.
        
        There SHOULD NOT be a payload for the IndividualEvent subclass [EVEN].
    
    substructures
    :   -   Those inherited from [Event]
        -   [AGE]?



### FamilyEvent

The *FamilyEvent* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Supertype
:   [Event]

Known Subtypes:
:   - [ANUL]
    - [CENS]
    - [DIV]
    - [DIVF]
    - [ENGA]
    - [MARR]
    - [MARB]
    - [MARC]
    - [MARL]
    - [MARS]
    - [EVEN]

#### Contexts


.[INDI].([FamilyEvent])
:   description
    :   Varies by subtype
    
    payload
    :   Either the string `Y` or not present.
        If any of the following are true 

        - the payload is `Y` 
        - there is a [DATE] substructure
        - there is a [PLAC] substructure
        - the tag is [EVEN]

        then the FamilyEvent asserts that the event occurred; otherwise it just discusses the idea of the event.
        
        There SHOULD NOT be a payload for the FamilyEvent subclass [EVEN].
    
    substructures
    :   -   Those inherited from [Event]
        -   [HUSB]?
        -   [WIFE]?



### IndividualAttribute

The *IndividualAttribute* type has no GEDCOM tag name and cannot be directly instantiated.
It serves as an abstract supertype for other events and attributes.

Represents attributes or facts are used to describe an individual's actions, physical description, employment, education, places of residence, etc.
These are not generally thought of as events.
However, they are often described like events because they were observed at a particular time and/or place.


{.note} Prior to GEDCOM 5.5.1, there was no [FACT] attribute and [EVEN] was sometimes used in its place.

Supertype
:   [Event]

Known Subtypes:
:   - [CAST]
    - [DSCR]
    - [EDUC]
    - [IDNO]
    - [NATI]
    - [NCHI]
    - [NMR]
    - [OCCU]
    - [PROP]
    - [RELI]
    - [RESI]
    - [SSN]
    - [TITL]
    - [FACT]

#### Contexts

.[INDI].([IndividualAttribute])
:   description
    :   Varies by subtype
    
    payload
    :   Varies by subtype
    
    substructures
    :   -   Those inherited from [Event]
        -   [AGE]?




## Concrete Types


### ABBR

A short name of a title, description, or name.

#### Contexts

.[SOUR].[ABBR]
:   description
    :   A short title used for sorting, filing, and retrieving source records.
    
    payload
    :   1--60 characters
    
    substructures
    :   None


### ADDR

The contemporary place, usually required for postal purposes, of an individual, a submitter of information, a repository, a business, a school, or a company.

#### Contexts

.[HEAD].[SOUR].[CORP].[ADDR]
:   see ..[ADDR]

.[REPO].[ADDR]
:   see ..[ADDR]

.[SUBM].[ADDR]
:   see ..[ADDR]

..[ADDR]
:   description
    :   Address information that, when combined with NAME substructure, meets requirements for sending communications through the mail.
    
    payload
    :   A string. It is RECOMMENDED that implementations support at least three lines of at least 60 characters each.
    
    substructures
    :   -   [ADR1]?
        -   [ADR2]?
        -   [CITY]?
        -   [STAE]?
        -   [POST]?
        -   [CTRY]?
        -   [PHON]\* -- it is RECOMMENDED that at least three `ADDR.PHON` be supported per `ADDR`
        -   [EMAIL]\* -- it is RECOMMENDED that at least three `ADDR.FAX` be supported per `ADDR`
        -   [FAX]\* -- it is RECOMMENDED that at least three `ADDR.EMAIL` be supported per `ADDR`
        -   [WWW]\* -- it is RECOMMENDED that at least three `ADDR.WWW` be supported per `ADDR`

{.note} [EMAIL] was introduced in GEDCOM 5.5.1 with two tag names: both `EMAIL` and `EMAI`.  `EMAIL` was used more consistently and is documented here, but it is RECOMMENDED that implementations treat `ADDR.EMAI` as synonymous with `ADDR.EMAIL`.




### ADR1

The first line of an address.

#### Contexts

[ADDR].[ADR1]
:   description
    :   The first line of the address used for indexing.
        This SHOULD correspond to the first line of the superstructure's payload.
    
    payload
    :   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    substructures
    :   None

### ADR2

The second line of an address.

#### Contexts

[ADDR].[ADR1]
:   description
    :   The second line of the address used for indexing.
        This SHOULD correspond to the second line of the superstructure's payload.
    
    payload
    :   A string. It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    substructures
    :   None


### ADOP

Pertaining to creation of a child-parent relationship that does not exist biologically.

#### Contexts

.[INDI].[ADOP]
:   description
    :   *see [IndividualEvent]*
    
    supertype
    :   [IndividualEvent]
    
    payload
    :   Either `Y` or None, as described under [IndividualEvent]
    
    substructures
    :   [*inherited*](#individualevent),
        [FAMC]?
    
[ADOP].[FAMC].[ADOP]
:   description
    :   Which parent(s) adopted
    
    payload
    :   A string, which SHOULD be from the set {`HUSB`, `WIFE`, `BOTH`}
    
    substructures
    :   None


-----

{.ednote} Continue reformatting from here

-----

### AGE

`http://terms.fhiso.org/legacy/longform/AGE`

The age of the individual at the time an event occurred, or the age listed in the document.

Meaning
:   A number that indicates the age in years, months, and days that the principal was at the time of the associated event.

Payload
:   1--12 characters matching the regular expresison
    `([<>] ?)?(\d+y ?)?(\d+m ?)?(\d+d ?)?|CHILD|INFANT|STILLBORN`
    where
    
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

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.FAM.FamilyEvent.HUSB.AGE` | see above | see above | None
`.FAM.FamilyEvent.WIFE.AGE` | see above | see above | None
`IndividualEvent.AGE` | see above | see above | None
`IndividualAttribute.AGE` | see above | see above | None


### AGNC

`http://terms.fhiso.org/legacy/longform/AGENCY`

The institution or individual having authority and/or responsibility to manage or govern.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SOUR.DATA.AGNC` | The organization, institution, corporation, person, or other entity that has authority or control interests in the associated context. For example, an organization responsible for creating and/or archiving records. | 1--120 characters | None
`Event.AGNC` | The organization, institution, corporation, person, or other entity that has authority or control interests in the associated context. For example, an employer of a person of an associated occupation, or a church that administered rites or event. | 1--120 characters | None

### ALIA

`http://terms.fhiso.org/legacy/longform/ALIAS`

An indicator to link different record descriptions of a person who may be the same person.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.ALIA`  |         | Pointer to an [INDI] | None


### ANCE

`http://terms.fhiso.org/legacy/longform/ANCESTORS`

Pertaining to forbearers of an individual.

See also [DESC]

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SUBN.ANCE` |  The number of generations of ancestors included in this transmission. | 1--4 characters | None

{.ednote} Presumably this is supposed to be a base-10 integer?  Also, the Appendix A text and the Chapter 2 text for this tag don't seem to have much relationship to one another.


### ANCI

`http://terms.fhiso.org/legacy/longform/ANCES_INTEREST`

Indicates an interest in additional research for ancestors of this individual. (See also [DESI].)

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.ANCI`  |         | Pointer to an [SUBM] | None


### ANUL

`http://terms.fhiso.org/legacy/longform/ANNULMENT`

Declaring a marriage void from the beginning (never existed).

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.ANUL`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### ASSO

`http://terms.fhiso.org/legacy/longform/ASSOCIATES`

An indicator to link friends, neighbors, relatives, or associates of an individual.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.ASSO`  |         | Pointer to an [INDI] | [RELA]!, [NOTE]\*, [SOUR]\*


### AUTH

`http://terms.fhiso.org/legacy/longform/AUTHOR`

The name of the individual who created or compiled information.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SOUR.AUTH`  | The person, agency, or entity who created the record. For a published work, this could be the author, compiler, transcriber, abstractor, or editor. For an unpublished source, this may be an individual, a government agency, church organization, or private organization, etc. | String of arbitrary length | None

### BAPM

`http://terms.fhiso.org/legacy/longform/BAPTISM`

The event of baptism, performed in infancy or later.

See also [BAPL] and [CHR]

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.BAPM`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### BARM

`http://terms.fhiso.org/legacy/longform/BAR_MITZVAH`

The ceremonial event held when a Jewish boy reaches age 13.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.BARM`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### BASM

`http://terms.fhiso.org/legacy/longform/BAS_MITZVAH`

The ceremonial event held when a Jewish girl reaches age 13, also known as "Bat Mitzvah."

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.BASM`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### BIRT

`http://terms.fhiso.org/legacy/longform/BIRTH`

The event of entering into life.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.BIRT`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent), [FAMC]?


### BLES

`http://terms.fhiso.org/legacy/longform/BLESSING`

A religious event of bestowing divine care or intercession. Sometimes given in connection with a naming ceremony.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.BLES`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### BLOB

`http://terms.fhiso.org/legacy/longform/BINARY_OBJECT`

A grouping of data used as input to a multimedia system that processes binary data to represent images, sound, and video.

{.note} This tag was present in GEDCOM 5.5 but was **removed** from GEDCOM 5.5.1.

When multiple blobs are present in the same structure, their contents are concatenated in order.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.OBJE.BLOB`  | Encoded binary data | Two or more lines of ELF-style base-64 encoded data | None

The first line of of a blob is always empty.
Each subsequent line is between 4 and 72 characters long, encoded in a base-64 format that differs from other base-64 encodings:
it uses U+00FF (LATIN SMALL LETTER Y WITH DIAERESIS `ÿ`) as padding instead of the more common U+003D (EQUALS SIGN `=`)
and maps bytes as follows:

| Byte range | Code point mapping |
|------------|--------------------|
| 0x00--0x0B | byte + 0x2E        |
| 0x0C--0x25 | byte + 0x35        |
| 0x25--0x3F | byte + 0x3B        |


{.ednote} Probably need more detail than this... also GEDCOM discusses this topic in bytes, not characters, (even though in another place it explicit states it always means character, not bytes) so U+00FF might be wrong.


### BURI

`http://terms.fhiso.org/legacy/longform/BURIAL`

The event of the proper disposing of the mortal remains of a deceased person.

{.ednote} GEDCOM calls burial "proper disposing" sans discussion of method, but cremation "disposal […] by fire" sans proper?  Why?

See also [CREM]

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.BURI`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### CALN

`http://terms.fhiso.org/legacy/longform/CALL_NUMBER`

The number used by a repository to identify the specific items in its collections.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SOUR.REPO.CALN` | An identification or reference description used to file and retrieve items from the holdings of a repository.  | 1--120 characters | [MEDI]?

{.ednote} The example given in the GEDCOM specification is inconsistent with the grammar, putting the `MEDI` coordinate with, instead of subordinate to, `CALN`.


### CAST

`http://terms.fhiso.org/legacy/longform/CASTE`

The name of an individual's rank or status in society, based
on racial or religious differences, or differences in wealth, inherited
rank, profession, occupation, etc.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.CAST`  |  A name assigned to a particular group that this person was associated with, such as a particular racial group, religious group, or a group with an inherited status.  | 1--90 characters | [IndividualAttribute] | [*inherited*](#individualattribute)


### CAUS

`http://terms.fhiso.org/legacy/longform/CAUSE`

A description of the cause of the associated event or fact, such as the cause of death.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
*Event*`.CAUS`  |  Used in special cases to record the reasons which precipitated an event. Normally this will be used subordinate to a death event to show cause of death, such as might be listed on a death certificate. | None


### CENS

`http://terms.fhiso.org/legacy/longform/CENSUS`

The event of the periodic count of the population for a designated locality, such as a national or state Census.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.CENS`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)
`.FAM.CENS`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### CHAN

`http://terms.fhiso.org/legacy/longform/CHANGE`

Indicates a change, correction, or modification. Typically used in connection with a DATE to specify when a change in information occurred.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
*record*`.CHAN`   | The change date is intended to only record the last change to a record. Some systems may want to manage the change process with more detail, but it is sufficient for GEDCOM purposes to indicate the last time that a record was modified. | None | [DATE]!, [NOTE]\*

`CHAN` is known a the substructure of every known record *except* [SUBN], [HEAD], and [TAIL].

### CHAR

`http://terms.fhiso.org/legacy/longform/CHARACTER`

An indicator of the character set used in writing this automated information.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.CHAN`   | A code value that represents the character set to be used to interpret this data. The default character set is ANSEL, which includes ASCII as a subset. UNICODE is not widely supported by most operating systems; therefore, GEDCOM produced using the UNICODE character set will be limited in acceptance for some time. See Chapter 3. ASCII contains the character set from 0x0 to 0x7F.  | one of `ANSEL`, `UNICODE`, or `ASCII` | [VERS]?

### CHIL

`http://terms.fhiso.org/legacy/longform/CHILD`

The natural, adopted, or sealed (LDS) child of a father and a mother.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.FAM.CHIL`   |         | A pointer to an [INDI] | None

{.note} In GEDCOM versions 3 and 4, `FAM.CHIL` had a substructure [ADOP].


### CHR

`http://terms.fhiso.org/legacy/longform/CHRISTENING`

The religious event (not LDS) of baptizing and/or naming a child.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.CHR`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent), [FAMC]?


### CHRA

`http://terms.fhiso.org/legacy/longform/ADULT_CHRISTENING`

The religious event (not LDS) of baptizing and/or naming an adult person.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.CHRA`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### CITY

`http://terms.fhiso.org/legacy/longform/CITY`

A lower level jurisdictional unit. Normally an incorporated municipal unit.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.CITY`   | The name of the city used in the address. Isolated for sorting or indexing. | 1--60 characters | None


### CONF

`http://terms.fhiso.org/legacy/longform/CONFIRMATION`

The religious event (not LDS) of conferring the gift of the Holy Ghost and, among protestants, full church membership.

{.ednote} Is this the right definition of a confirmation?

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.CONF`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### COPR

`http://terms.fhiso.org/legacy/longform/COPYRIGHT`

A statement that accompanies data to protect it from unlawful duplication and distribution.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.COPR`  |  A copyright statement needed to protect the copyrights of the submitter of this GEDCOM file. | 1--90 characters | None
`.HEAD.SOUR.DATA.COPR` |  A copyright statement required by the owner of data from which this information was down- loaded. For example, when a GEDCOM down-load is requested from the Ancestral File, this would be the copyright statement to indicate that the data came from a copyrighted source. | 1--90 characters\* | None

\* The 5.5.1 spec allows `CONC`/`CONT` extension of `.HEAD.SOUR.DATA.COPR` (allowing it to be arbitrarily long) but does not extend the 1--90 character limit (contradicting the other change).

### CORP

`http://terms.fhiso.org/legacy/longform/CORPORATE`

A name of an institution, agency, corporation, or company.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.SOUR.CORP` | Name of the business, corporation, or person that produced or commissioned the product.  | 1--90 characters | [ADDR]?, [PHON]\*

### CREM

`http://terms.fhiso.org/legacy/longform/CREMATION`

Disposal of the remains of a person's body by fire.

See also [BURI]

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.BURI`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)

### CTRY

`http://terms.fhiso.org/legacy/longform/COUNTRY`

The name or code of the country.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.CTRY`   | The name of the country that pertains to the associated address. Isolated by some systems for sorting or indexing. Used in most cases to facilitate automatic sorting of mail. | 1--60 characters | None


### DATA

`http://terms.fhiso.org/legacy/longform/DATA`

Pertaining to stored automated information.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.SOUR.DATA` | The name of the electronic data source that was used to obtain the data in this transmission. For example, the data may have been obtained from a CD-ROM disc that was named "U.S. 1880 CENSUS CD-ROM vol. 13." | 1--90 characters | [DATE]?, [COPR]?
`.SOUR.DATA` | | None | [EVEN]\*, [AGNC]?, [NOTE]\*
`SOUR.DATA`  | | None | [DATE]?, [TEXT]\*

### DATE

`http://terms.fhiso.org/legacy/longform/DATE`

The time of an event in a calendar format.

{.ednote} The date following pseudo-syntax is ugly and leaves out meaning... update it

VALUE ::= DATE | PERIOD | RANGE | APPROXIMATED | `INT` DATE `(` 1--35 characters `)` | `(` 1--35 characters `)`

RANGE ::= `BEF` DATE | `AFT` DATE | `BET` DATE `AND` DATE

PERIOD ::= `FROM` DATE | `TO` DATE | `FROM` DATE `TO` DATE

APPROXIMATED ::= ( `ABT` | `CAL` | `EST` ) DATE

DATE ::= GREG | JULN | HEBR | FREN

GREG ::= escape(`DGREGORIAN`)? `(([0-3]?[0-9])? (JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC))? [0-9]+/([0-9][0-9])?`

EXACT ::= `[0-3]?[0-9] (JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC) [0-9]+`

FREN ::= escape(`DFRENCH R`) `(([0-3]?[0-9])? (VEND|BRUM|FRIM|NIVO|PLUV|VENT|GERM|FLOR|PRAI|MESS|THER|FRUC|COMP))? [0-9]+`

HEBR ::= escape(`DHEBREW`) `(([0-3]?[0-9])? (TSH|CSH|KSL|TVT|SHV|ADR|ADS|NSN|IYR|SVN|TMZ|AAV|ELL))? [0-9]+`

JULN ::= escape(`DJULIAN`) `(([0-3]?[0-9])? (JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC))? [0-9]+`

{.ednote} There is a documented escape(`DROMAN`) and escape(`DUNKNOWN`) but there is no documentation of what, if anything, may follow those escapes.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
[*Event*](#event)`.DATE` | The DATE_VALUE represents the date of an activity, attribute, or event where: INT = Interpreted from knowledge about the associated date phrase included in parentheses. | see VALUE above | None
[*LDS*](#lds)`.DATE` | | see EXACT above | None
`.HEAD.DATE` | The date that this transmission was created. | see EXACT above| [TIME]?
`CHAN.DATE` | The date that this data was changed. | see EXACT above | [TIME]?
`SOUR.DATA.DATE` |  The date that this event data was entered into the original source document. | see VALUE above | None
`.HEAD.SOUR.DATA.DATE` | The date this source was published or created. | see EXACT above | None
`.SOUR.DATA.EVEN.DATE` | | see PERIOD above | None

### DEAT

`http://terms.fhiso.org/legacy/longform/DEATH`

The event when mortal life terminates.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.DEAT`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)

### DESC

`http://terms.fhiso.org/legacy/longform/DESCENDANTS`

Pertaining to offspring of an individual.

See also [ANCE]

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SUBN.DESC` |  The number of generations of descendants included in this transmission. | 1--4 characters | None

{.ednote} Presumably this is supposed to be a base-10 integer?  Also, the Appendix A text and the Chapter 2 text for this tag don't seem to have much relationship to one another.


### DESI

`http://terms.fhiso.org/legacy/longform/DESCENDANT_INT`

Indicates an interest in research to identify additional descendants of this individual. (See also [ANCI].)

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.DESI`  |         | Pointer to an [SUBM] | None


### DEST

`http://terms.fhiso.org/legacy/longform/DESTINATION`

A system receiving data.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.DEST` | The name of the system expected to process the GEDCOM-compatible transmission. | 1--20 characters | None


### DIV

`http://terms.fhiso.org/legacy/longform/DIVORCE`

An event of dissolving a marriage through civil action.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.DIV`    | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### DIVF

`http://terms.fhiso.org/legacy/longform/DIVORCE_FILED`

An event of filing for a divorce by a spouse.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.DIVF`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### DSCR

`http://terms.fhiso.org/legacy/longform/PHY_DESCRIPTION`

The physical characteristics of a person, place, or thing.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.DSCR`  | An unstructured list of the attributes that describe the physical characteristics of a person, place, or object. Commas separate each attribute. | 1--248 characters | [IndividualAttribute] | [*inherited*](#individualattribute)


### EDUC

`http://terms.fhiso.org/legacy/longform/EDUCATION`

Indicator of a level of education attained.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.EDUC`  | A description of a scholastic or educational achievement or pursuit. | 1--248 characters | [IndividualAttribute] | [*inherited*](#individualattribute)


### EMAIL

`http://terms.fhiso.org/legacy/longform/EMAIL`

An electronic mail address.

{.note} This tag was introduced in GEDCOM 5.5.1.  The spec is inconsistent in its short form name: Appendix A lists it as `EMAI` but the grammar uses `EMAIL` instead.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.EMAIL` | An electronic address that can be used for contact such as an email address.  | 5--120 characters | None


### EMIG

`http://terms.fhiso.org/legacy/longform/EMIGRATION`

An event of leaving one's homeland with the intent of residing elsewhere.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.EMIG`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### ENGA

`http://terms.fhiso.org/legacy/longform/ENGAGEMENT`

An event of recording or announcing an agreement between two people to become married.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.ENGA`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### EVEN

`http://terms.fhiso.org/legacy/longform/EVENT`

A noteworthy happening related to an individual, a group, or an organization.

The EVEN tag is for recording general events or attributes that are not other known subtypes of [Event].
The general event or attribute type is declared by using a subordinate [TYPE] tag to show what event or attribute is recorded.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.EVEN`  | see above | None | [IndividualEvent] | [*inherited*](#individualevent), [TYPE]!
`.FAM.EVEN`   | see above | None | [FamilyEvent] | [*inherited*](#familyevent), [TYPE]!
`SOUR.EVEN` |  A code that indicates the type of event which was responsible for the source entry being recorded. For example, if the entry was created to record a birth of a child, then the type would be BIRT regardless of the assertions made from that record, such as the mother's name or mother's birth date. This will allow a prioritized best view choice and a determination of the certainty associated with the source used in asserting the cited fact.  | 1--15 characters; one of {`ANUL`, `CENS`, `DIV`, `DIVF`, `ENGA`, `MARR`, `MARB`, `MARC`, `MARL`, `MARS`, `EVEN`, `ADOP`, `BIRT`, `BAPM`, `BARM`, `BASM`, `BLES`, `BURI`, `CENS`, `CHR`, `CHRA`, `CONF`, `CREM`, `DEAT`, `EMIG`, `FCOM`, `GRAD`, `IMMI`, `NATU`, `ORDN`, `RETI`, `PROB`, `WILL`, `EVEN`, `CAST`, `EDUC`, `NATI`, `OCCU`, `PROP`, `RELI`, `RESI`, `TITL`, `FACT`} | None | [ROLE]
`.SOUR.DATA.EVEN` |  An enumeration of the different kinds of events that were recorded in a particular source. Each enumeration is separated by a comma. Such as a parish register of births, deaths, and marriages would be BIRT, DEAT, MARR. | 1--90 characters forming a comma-separated list of {`ANUL`, `CENS`, `DIV`, `DIVF`, `ENGA`, `MARR`, `MARB`, `MARC`, `MARL`, `MARS`, `EVEN`, `ADOP`, `BIRT`, `BAPM`, `BARM`, `BASM`, `BLES`, `BURI`, `CENS`, `CHR`, `CHRA`, `CONF`, `CREM`, `DEAT`, `EMIG`, `FCOM`, `GRAD`, `IMMI`, `NATU`, `ORDN`, `RETI`, `PROB`, `WILL`, `EVEN`, `CAST`, `EDUC`, `NATI`, `OCCU`, `PROP`, `RELI`, `RESI`, `TITL`, `FACT`} | None | [DATE]?, [PLAC]?

{.note} despite the above text describing the use of EVEN tags for attributes, they are not a known subtype of [IndividualAttribute] and are not known to carry a payload.  Use of EVEN for attributes is thus under-defined.

{.note} Uncharacteristically for enumerated values, `SOUR.EVEN` is listed to be up to 15 characters even though the longest known value is 4 characters long.


### FACT

`http://terms.fhiso.org/legacy/longform/FACT`

Pertaining to a noteworthy attribute or fact concerning an individual, a group, or an organization. A `FACT` structure is usually qualified or classified by a subordinate use of the `TYPE` tag.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.FACT`  | Text describing a particular characteristic or attribute assigned to an individual. | 1--90 characters | [IndividualAttribute] | [*inherited*](#individualattribute), [TYPE]!


### FAM

`http://terms.fhiso.org/legacy/longform/FAMILY`

Identifies a legal, common law, or other customary relationship of man and woman and their children, if any, or a family created by virtue of the birth of a child to its biological father and mother.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.FAM` | The FAM record is used to record marriages, common law marriages, and family unions caused by two people becoming the parents of a child. There can be no more than one HUSB/father and one WIFE/mother listed in each FAM. If, for example, a man participated in more than one family union, then he would appear in more than one FAM. The family record structure assumes that the HUSB/father is male and WIFE/mother is female. | None | [FamilyEvent]\*, [HUSB]? [WIFE]?, [CHIL]\*, [NCHI]?, [SUBM]\*, [SOUR]\*, [OBJE]\*, [NOTE]\*, [REFN]\*, [RIN]?, [CHAN]?, [RESN]?

The preferred order of the [CHIL] substructures within a FAM structure is chronological by birth. 
 
See also [SLGS] in the [LDS-specific tags] section.


### FAMC

`http://terms.fhiso.org/legacy/longform/FAMILY_CHILD`

Identifies the family in which an individual appears as a child.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.FAMC`  | | Pointer to a [FAM] | [PEDI]?, [NOTE]\*, [STAT]?
`BIRT.FAMC`   | | Pointer to a [FAM] | None
`CHR.FAMC`    | | Pointer to a [FAM] | None
`.INDI.ADOP.FAMC` | | Pointer to a [FAM] | [ADOP]

See also [SLGC] in the [LDS-specific tags] section.


### FAMS

`http://terms.fhiso.org/legacy/longform/FAMILY_SPOUSE`

Identifies the family in which an individual appears as a spouse.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.FAMS`  | | Pointer to a [FAM] | [NOTE]\*


### FAX

`http://terms.fhiso.org/legacy/longform/FACIMILIE`

Electronic facimilie transmission.

{.note} This tag was introduced in GEDCOM 5.5.1.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.EMAIL` | A FAX telephone number appropriate for sending data facsimiles.  | 5--60 characters | None


### FCOM

`http://terms.fhiso.org/legacy/longform/FIRST_COMMUNION`

A religious rite, the first act of sharing in the Lord's supper as part of church worship.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.FCOM`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### FILE

`http://terms.fhiso.org/legacy/longform/FILE`

An information storage place that is ordered and arranged for preservation and reference.

#### GEDCOM 5.5-compatible version

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`OBJE.FILE` |  A complete local or remote file reference to the auxiliary data to be linked to the GEDCOM context. Remote reference would include a network address where the multimedia data may be obtained. | 1--30 characters | None

#### GEDCOM 5.5.1-compatible version

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`OBJE.FILE` |  A complete local or remote file reference to the auxiliary data to be linked to the GEDCOM context. Remote reference would include a network address where the multimedia data may be obtained. | 1--30 characters | [FORM]!, [TITL]?

#### Common information

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.FILE` |  The name of the GEDCOM transmission file. If the file name includes a file extension it must be shown in the form (filename.ext). | 1--90 characters | None


### FONE

`http://terms.fhiso.org/legacy/longform/PHONETIC`

A phonetic variation of a superior text string.

{.note} This tag was introduced in GEDCOM 5.5.1.

The method of phonetic writing is indicated by the subordinate `TYPE` value; for example if hiragana was used to provide a reading of a name written in kanji, then the `TYPE` value would indicate `kana`.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.FONE` | The phonetic variation of the name in the same form as the was the name used in the superior primitive.  | 1--120 characters | [TYPE]!, [NPFX]?, [GIVN]?, [NICK]?, [SPFX]?, [SURN]?, [NSFX]?, [NOTE]\*, [SOUR]\*
[Event]`.PLAC.FONE` | The phonetic variation of the place name in the same form as the was the name used in the superior primitive. | [TYPE]!


### FORM

`http://terms.fhiso.org/legacy/longform/FORMAT`

An assigned name given to a consistent format in which information can be conveyed.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.PLAC.FORM` | Implies that all place names follow this jurisdictional format and each jurisdiction is accounted for by a comma, whether the name is known or not. May be overridden by [Event]`.PLAC.FORM`. | 1--120 characters | None
[Event]`.PLAC.FORM` | Jurisdictional entities that are named in a sequence from the lowest to the highest jurisdiction. The jurisdictions are separated by commas, and any jurisdiction's name that is missing is still accounted for by a comma. | 1--120 characters | None
`.HEAD.GEDC.FORM` | The GEDCOM form used to construct this transmission. | 14--20 characters, specifically `LINEAGE_LINKED` | None


#### GEDCOM 5.5-compatible version

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`OBJE.FORM` | Indicates the format of the multimedia data with this tag. | one of {`bmp`, `gif`, `jpeg`, `ole`, `pcx`, `tiff`, `wav`} | None

#### GEDCOM 5.5.1-compatible version

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`FILE.FORM` | Indicates the format of the multimedia data with this tag. | one of {`bmp`, `gif`, `jpeg`, `ole`, `pcx`, `tiff`, `wav`} | None


### GEDC

`http://terms.fhiso.org/legacy/longform/GEDCOM`

Information about the use of GEDCOM in a transmission.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.GEDC`  |         | None    | [VERS]!, [FORM]!


### GIVN

`http://terms.fhiso.org/legacy/longform/GIVEN_NAME`

A given or earned name used for official identification of a person.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.GIVN`   |  Given name or earned name. Different given names are separated by a comma. | 1--120 characters, with 1--90 characters between each comma | None


### GRAD

`http://terms.fhiso.org/legacy/longform/GRADUATION`

An event of awarding educational diplomas or degrees to individuals.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.GRAD`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### HEAD

`http://terms.fhiso.org/legacy/longform/HEADER`

Identifies information pertaining to an entire GEDCOM transmission.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD`       |         | None    | [SOUR]!, [DEST]?, [DATE]?, [SUBM]!, [SUBN]?, [FILE]?, [COPR]?, [GEDC]!, [CHAR]!, [LANG]?, [PLAC]?, [NOTE]?


### HUSB

`http://terms.fhiso.org/legacy/longform/HUSBAND`

An individual in the family role of a married man or father.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.FAM.`[FamilyEvent]`.HUSB` | | None | [AGE]!
`.FAM.HUSB`   |         | pointer to an [INDI] | None


### IDNO

`http://terms.fhiso.org/legacy/longform/IDENT_NUMBER`

A number assigned to identify a person within some significant external system.

See also [SSN]

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.IDNO`  | A nationally-controlled number assigned to an individual. | 1--30 characters | [IndividualAttribute] | [*inherited*](#individualattribute), [TYPE]!

{.note} Although called a "number", the payload of an `IDNO` is *not* restricted to numeric values.


### IMMI

`http://terms.fhiso.org/legacy/longform/IMMIGRATION`

An event of entering into a new locality with the intent of residing there.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.IMMI`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### INDI

`http://terms.fhiso.org/legacy/longform/INDIVIDUAL`

A person.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI` | The individual record is a compilation of facts, known or discovered, about an individual. Sometimes these facts are from different sources. This form allows documentation of the source where each of the facts were discovered. | None | [RESN]?, [NAME]\*, [SEX]?, [IndividualEvent], [IndividualAttribute], [FAMC]\*, [FAMS]\*, [SUBM]\*, [ASSO]\*, [ALIA]\*, [ANCI]\*, [DESI]\*, [SOUR]\*, [OBJE]\*, [NOTE]\*, [RFN]?, [REFN]\*, [RIN]?, [CHAN]?

See also [LDS-specific tags]


### LANG

`http://terms.fhiso.org/legacy/longform/LANGUAGE`

The name of the language used in a communication or transmission of information.

The set of known language payload value is {`Afrikaans`, `Albanian`, `Anglo-Saxon`, `Catalan`, `Catalan_Spn`, `Czech`, `Danish`, `Dutch`, `English`, `Esperanto`, `Estonian`, `Faroese`, `Finnish`, `French`, `German`, `Hawaiian`, `Hungarian`, `Icelandic`, `Indonesian`, `Italian`, `Latvian`, `Lithuanian`, `Navaho`, `Norwegian`, `Polish`, `Portuguese`, `Romanian`, `Serbo_Croa`, `Slovak`, `Slovene`, `Spanish`, `Swedish`, `Turkish`, `Wendic`, `Amharic`, `Arabic`, `Armenian`, `Assamese`, `Belorusian`, `Bengali`, `Braj`, `Bulgarian`, `Burmese`, `Cantonese`, `Church-Slavic`, `Dogri`, `Georgian`, `Greek`, `Gujarati`, `Hebrew`, `Hindi`, `Japanese`, `Kannada`, `Khmer`, `Konkani`, `Korean`, `Lahnda`, `Lao`, `Macedonian`, `Maithili`, `Malayalam`, `Mandrin`, `Manipuri`, `Marathi`, `Mewari`, `Nepali`, `Oriya`, `Pahari`, `Pali`, `Panjabi`, `Persian`, `Prakrit`, `Pusto`, `Rajasthani`, `Russian`, `Sanskrit`, `Serb`, `Tagalog`, `Tamil`, `Telugu`, `Thai`, `Tibetan`, `Ukrainian`, `Urdu`, `Vietnamese`, `Yiddish`}

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.LANG` | The human language in which the data in the transmission is normally read or written. | 1--15 characters; see above | None
`.SUBM.LANG` | The language in which a person prefers to communicate. Multiple language preference is shown by using multiple occurrences in order of priority. | 1--90 characters; see above | None

{.note} There is no obvious reason why the two contexts in which `LANG` appear have the same set of values but different character lengths.

### LEGA

`http://terms.fhiso.org/legacy/longform/LEGATEE`

A role of an individual acting as a person receiving a bequest or legal devise.

This tag does not appear in any known context.


### LATI

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


### LONG

`http://terms.fhiso.org/legacy/longform/LONGITUDE`

A value indicating a coordinate position on a line, plane, or space.

{.note} This tag was introduced in GEDCOM 5.5.1.

For example:  168 degrees, 9 minutes, and 3.4 seconds East would be formatted as `E168.150944`.
Minutes and seconds are converted by dividing the minutes value by 60 and the seconds value by 3600 and adding the results together.
This sum becomes the fractional part of the degree's value.

{.ednote} The example in the GEDCOM spec uses 11 characters, while restricting it to 8.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`MAP.LONG` | The value specifying the longitudinal coordinate of the place name. The longitude coordinate is Degrees and fraction of degrees east or west of the zero or base meridian coordinate. |  5--8 characters | None


### MAP

`http://terms.fhiso.org/legacy/longform/MAP`

Pertains to a representation of measurements usually presented in a graphical form.

{.note} This tag was introduced in GEDCOM 5.5.1.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
[Event]`.PLAC.MAP` | | None | [LATI]!, [LONG]!


### MARB

`http://terms.fhiso.org/legacy/longform/MARRIAGE_BANN`

An event of an official public notice given that two people intend to marry.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.MARB`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### MARC

`http://terms.fhiso.org/legacy/longform/MARR_CONTRACT`

An event of recording a formal agreement of marriage, including the prenuptial agreement in which marriage partners reach agreement about the property rights of one or both, securing property to their children.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.MARC`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### MARL

`http://terms.fhiso.org/legacy/longform/MARR_LICENSE`

An event of obtaining a legal license to marry.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.MARL`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### MARR

`http://terms.fhiso.org/legacy/longform/MARRIAGE`

A legal, common-law, or customary event of creating a family unit of a man and a woman as husband and wife.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.MARR`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### MARS

`http://terms.fhiso.org/legacy/longform/MARR_SETTLEMENT`

An event of creating an agreement between two people contemplating marriage, at which time they agree to release or modify property rights that would otherwise arise from the marriage.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.MARS`   | *see [FamilyEvent]* | Either `Y` or None | [FamilyEvent] | [*inherited*](#familyevent)


### MEDI

`http://terms.fhiso.org/legacy/longform/MEDIA`

Identifies information about the media or having to do with the medium in which information is stored.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SOUR.REPO.CALN.MEDI` |  A code, selected from one of the media classifications choices above, that indicates the type of material in which the referenced source is stored.  | 1--15 characters, one of {`audio`, `book`, `card`, `electronic`, `fiche`, `film`, `magazine`, `manuscript`, `map`, `newspaper`, `photo`, `tombstone`, `video`} | None


### NAME

`http://terms.fhiso.org/legacy/longform/NAME`

A word or combination of words used to help identify an individual, title, or other item. More than one NAME line should be used for people who were known by multiple names.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.SOUR.NAME` | The name of the software product that produced this transmission. | 1--90 characters | None
`.REPO.NAME` | The official name of the archive in which the stated source material is stored. | 1--90 characters | None
`.SUBM.NAME` | The name of the submitter formatted for display and address generation. | 1--60 characters | None
`.INDI.NAME` | The name value is formed in the manner the name is normally spoken, with the given name and family name (surname) separated by slashes `/`. | 1--120 characters, optionally with a substring offset by `/`, optionally with portions elided with `...` | [NPFX]?, [GIVN]?, [NICK]?, [SPFX]?, [SURN]?, [NSFX]?, [SOUR]\*, [NOTE]\*, [FONE]\*, [ROMN]\*


### NATI

`http://terms.fhiso.org/legacy/longform/NATIONALITY`

The national heritage of an individual.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.NATI`  | The person's division of national origin or other folk, house, kindred, lineage, or tribal interest. Examples: Irish, Swede, Egyptian Coptic, Sioux Dakota Rosebud, Apache Chiricawa, Navajo Bitter Water, Eastern Cherokee Taliwa Wolf, and so forth. | 1--120 characters | [IndividualAttribute] | [*inherited*](#individualattribute)


### NATU

`http://terms.fhiso.org/legacy/longform/NATURALIZATION`

The event of obtaining citizenship.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.NATU`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### NCHI

`http://terms.fhiso.org/legacy/longform/CHILDREN_COUNT`

The number of children that this person is known to be the parent of (all marriages) when subordinate to an individual, or that belong to this family when subordinate to a FAM_RECORD.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.NCHI`  | The known number of children of this individual from all marriages. | 1--3 characters | [IndividualAttribute] | [*inherited*](#individualattribute)
`.FAM.NCHI`   | The reported number of children known to belong to this family, regardless of whether the associated children are represented in the corresponding structure. | 1--3 characters | None | None

{.ednote} Presumably this is supposed to be a base-10 integer?


### NICK

`http://terms.fhiso.org/legacy/longform/NICKNAME`

A descriptive or familiar that is used instead of, or in addition to, one's proper name.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.NICK` | A descriptive or familiar name used in connection with one's proper name. | 1--30 characters | None

{.ednote} The `NICK` grammar in GEDCOM is for a comma-separated list, but unlikes other parts of the name there is not descriptive text describing the meaning of the commas.


### NMR

`http://terms.fhiso.org/legacy/longform/MARRIAGE_COUNT`

The number of times this person has participated in a family as a spouse or parent.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.NMR`   | The number of different families that this person was known to have been a member of as a spouse or parent, regardless of whether the associated families are represented in the dataset. | 1--3 characters | [IndividualAttribute] | [*inherited*](#individualattribute)

{.ednote} Presumably this is supposed to be a base-10 integer?


### NOTE

`http://terms.fhiso.org/legacy/longform/NOTE`

Additional information provided by the submitter for understanding the enclosing data.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.NOTE` | Comments or opinions from the submitter. | [SOUR]\*, [REFN]\*, [RIN]?, [CHAN]?
`.HEAD.NOTE` | A note that a user enters to describe the contents of the lineage-linked file in terms of "ancestors or descendants of" so that the person receiving the data knows what genealogical information the transmission contains. | arbitrary-length text | None
`NOTE` | Comments or opinions from the submitter. | pointer to a `NOTE` | [SOUR]\*
`NOTE` | Comments or opinions from the submitter. | arbitrary-length text | [SOUR]\*

Every structures that is known to have substructures is known to admit an arbitrary number of the unanchored [NOTE] substructures, except: 

-   [ADDR]
-   [ADOP].[FAMC]
-   [CALN]
-   [DATE]
-   [FamilyEvent]`.`[HUSB] and [FamilyEvent]`.`[WIFE]
-   .[HEAD] and its nested substructures
-   [NOTE] itself (recursive notes are not known to be permitted)
-   [REFN]
-   [SOUR].[DATA]
-   [SOUR].[DATA].[EVEN]
-   [SOUR].[EVEN]


### NPFX

`http://terms.fhiso.org/legacy/longform/NAME_PREFIX`

Text which appears on a name line before the given and surname parts of a name.
i.e. `Lt. Cmndr. Joseph /Allen/ jr.`
In this example `Lt. Cmndr.` is considered as the name prefix portion.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.NPFX` | Non indexing name piece that appears preceding the given name and surname parts. Different name prefix parts are separated by a comma. | 1--30 characters | None


### NSFX

`http://terms.fhiso.org/legacy/longform/NAME_SUFFIX`

Text which appears on a name line after or behind the given and surname parts of a name.
i.e. `Lt. Cmndr. Joseph /Allen/ jr.`
In this example `jr.` is considered as the name suffix portion.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.NSFX` | Non-indexing name piece that appears after the given name and surname parts. Different name suffix parts are separated by a comma. | 1--30 characters | None


### OBJE

`http://terms.fhiso.org/legacy/longform/OBJECT`

Pertaining to a grouping of attributes used in describing something. Usually referring to the data required to represent a multimedia object, such an audio recording, a photograph of a person, or an image of a document.

The meaning and use of OBJE changed significantly between GEDCOM 5.5 and GEDCOM 5.5.1

#### GEDCOM 5.5-compatible version

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.OBJE` | | None | [FORM]!, [TITL]?, [NOTE]\*, [SOUR]\*, [BLOB]!, [OBJE]?, [REFN]\*, [RIN]?, [CHAN]?
`.OBJE.OBJE` | chain to continued object | pointer to an `.OBJE` | None
`OBJE` | linked form | pointer to an `.OBJE` | None
`OBJE` | embedded form | None | [FORM]!, [TITL]?, [FILE]!, [NOTE]\*

#### GEDCOM 5.5.1-compatible version

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.OBJE` | | None | [FILE]+, [REFN]\*, [RIN]?, [NOTE]\*, [SOUR]\*, [CHAN]?
`OBJE` | linked form | pointer to an `.OBJE` | None
`OBJE` | embedded form | None | [FILE]+, [TITL]?

#### Shared Information

The both unanchored forms of `OBJE` are known to exist in the following contexts:

-   .[FAM]`.OBJE`
-   .[FAM].[SOUR]`.OBJE`
-   .[INDI]`.OBJE`
-   .[SOUR]`.OBJE`
-   .[SUBM]`.OBJE`
-   [Event]`.OBJE`
-   [Event].[SOUR]`.OBJE`
-   [NAME].[SOUR]`.OBJE`
-   [SOUR]`.OBJE` if the payload of the [SOUR] is a pointer, not text 

{.note} The reason for the inconsistent presence/absence of `SOUR.OBJE` is not understood by the author of this document

{.ednote} The "if the payload is a pointer" distinction in where `OBJE` is known to occur is annoying in that the context specifier alone cannot describe this, but is embedded in the table for [SOUR] using just context specifiers so it isn't itself sufficient reason to revise the context specifier language

#### Common 


### OCCU

`http://terms.fhiso.org/legacy/longform/OCCUPATION`

The type of work or profession of an individual.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.OCCU`  | The kind of activity that an individual does for a job, profession, or principal activity. | 1--90 characters | [IndividualAttribute] | [*inherited*](#individualattribute)


### ORDN

`http://terms.fhiso.org/legacy/longform/ORDINATION`

A religious event of receiving authority to act in religious matters.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.ORDN`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### PAGE

`http://terms.fhiso.org/legacy/longform/PAGE`

A number or description to identify where information can be found in a referenced work.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`SOUR.PAGE`   | Specific location with in the information referenced. For a published work, this could include the volume of a multi-volume work and the page number(s). For a periodical, it could include volume, issue, and page numbers. For a newspaper, it could include a column number and page number. For an unpublished source, this could be a sheet number, page number, frame number, etc. A census record might have a line number or dwelling and family numbers in addition to the page number. | 1--248 characters | None


### PEDI

`http://terms.fhiso.org/legacy/longform/PEDIGREE`

Information pertaining to an individual to parent lineage chart.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.FAMC.PEDI` | A code used to indicate the child to family relationship for pedigree navigation purposes. | one of {`adopted`, `birth`, `foster`} | None

`adopted`
:   indicates adoptive parents.

`birth`
:   indicates birth parents.

`foster`
:   indicates child was included in a foster or guardian family.

### PHON

`http://terms.fhiso.org/legacy/longform/PHONE`

A unique number assigned to access a specific telephone.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.SOUR.CORP.PHON` || 1--25 characters | None
`.REPO.PHON`  |         | 1--25 characters | None
`.SUBM.PHON`  |         | 1--25 characters | None
`ADDR.PHON`   |         | 1--25 characters | None


### PLAC

`http://terms.fhiso.org/legacy/longform/PLACE`

A jurisdictional name to identify the place or location of an event.

Places are often represented by a **place hierarchy**.
This is a comma-separated list of place names, each subsumed by the place to its right.
No accommodation for place names that include commas is made by this structure.

See also [FORM] for the interpretation of the elements of a place hierarchy list.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.PLAC`  | see [FORM] | None | [FORM]!
`.SOUR.DATa.EVEN.PLAC` | The name of the lowest jurisdiction that encompasses all lower-level places named in this source. For example, "Oneida, Idaho" would be used as a source jurisdiction place for events occurring in the various towns within Oneida County. "Idaho" would be the source jurisdiction place if the events recorded took place in other counties as well as Oneida County. | 1--120 characters formatted as a *place hierarchy* | None
[Event]`.PLAC` | | 1--120 characters formatted as a *place hierarchy* | [FORM]?, [SOUR]\*, [NOTE]\*


### POST

`http://terms.fhiso.org/legacy/longform/POSTAL_CODE`

A code used by a postal service to identify an area to facilitate mail handling.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.POST`   | The ZIP or postal code used by the various localities in handling of mail. Isolated for sorting or indexing. | 1--10 characters | None


### PROB

`http://terms.fhiso.org/legacy/longform/PROBATE`

An event of judicial determination of the validity of a will. May indicate several related court activities over several dates.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.PROB`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### PROP

`http://terms.fhiso.org/legacy/longform/PROPERTY`

Pertaining to possessions such as real estate or other property of interest.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.ORDN`  | A list of possessions (real estate or other property) belonging to this individual. | 1--248 characters | [IndividualAttribute] | [*inherited*](#individualattribute)


### PUBL

`http://terms.fhiso.org/legacy/longform/PUBLICATION`

Refers to when and/or were a work was published or created.

When and where the record was created. For published works, this includes information such as the city of publication, name of the publisher, and year of publication.

For an unpublished work, it includes the date the record was created and the place where it was created. For example, the county and state of residence of a person making a declaration for a pension or the city and state of residence of the writer of a letter.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SOUR.PUBL` | see above | 248 or more characters | None

{.ednote} It is almost certainly an error in GEDCOM that asserts that every `PUBL` payload must be *at least* 248 characters and *cannot* have a newline in the first 248 characters.


### QUAY

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

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`SOUR.QUAY` | see above | one of {`0`, `1`, `2`, `3`} | None


### REFN

`http://terms.fhiso.org/legacy/longform/REFERENCE`

A description or number used to identify an item for filing, storage, or other reference purposes.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`REFN` | A user-defined number or text that the submitter uses to identify this record. For instance, it may be a record number within the submitter's automated or manual system, or it may be a page and position number on a pedigree chart. | 1--20 characters | [TYPE]?

`REFN` is known to be a substructure of .[FAM], .[INDI], .[NOTE], .[OBJE], .[REPO], and .[SOUR].

### RELA

`http://terms.fhiso.org/legacy/longform/RELATIONSHIP`

A relationship value between the indicated contexts.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ASSO.RELA` | A word or phrase that states object 1's relation is object 2. | 1--25 characters | None

Example:
You would read the following as "Joe Jacob's great grandson is the person described by the record with id "jim":

````gedcom
0 INDI
  1 NAME Joe /Jacob/
  1 ASSO @jim@
    2 RELA great grandson
````

{.ednote} This example, does not have an ID for Joe; that is also true of the example in the GEDCOM specification, despite that specification not allowing `0 INDI` without an ID.


### RELI

`http://terms.fhiso.org/legacy/longform/RELIGION`

A religious denomination to which a person is affiliated or for which a record applies.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.RELI`  | A name of the religion with which this person, event, or record was affiliated. | 1--90 characters | [IndividualAttribute] | [*inherited*](#individualattribute)
[Event]`.RELI` | A name of the religion with which this person, event, or record was affiliated. | 1--90 characters | None | None

{.ednote} Although the text from the GEDCOM specification suggests `RELI` can be a substructure of [SOUR] and [Event], the specification only lists it as an individual attribute.


### REPO

`http://terms.fhiso.org/legacy/longform/REPOSITORY`

An institution or person that has the specified item as part of their collection(s).

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.REPO` | | None | [NAME]?, [ADDR]?, [NOTE]\*, [REFN]\*, [RIN]?, [CHAN]?
`.SOUR.REPO` | This structure is used within a source record to point to a name and address record of the holder of the source document. | pointer to `.REPO` | [NOTE]\*, [CALN]\*


### RESI

`http://terms.fhiso.org/legacy/longform/RESIDENCE`

The act of dwelling at an address for a period of time.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.RESI`  |  | None | [IndividualAttribute] | [*inherited*](#individualattribute)

{.note} The `RESI` is the only known [IndividualAttribute] that does not have a payload.


### RESN

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
[Event]`.RESN` | The restriction notice is defined for Ancestral File usage. Ancestral File download GEDCOM files may contain this data. | one of `confidential`, `locked`, or `privacy` | None

{.ednote} I have not put this in the LDS section because, although it says "Ancestral File", I surmise it might be used for the same purpose by other data providers.


### RETI

`http://terms.fhiso.org/legacy/longform/RETIREMENT`

An event of exiting an occupational relationship with an employer after a qualifying time period.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.RETI`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### RFN

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


### RIN

`http://terms.fhiso.org/legacy/longform/REC_ID_NUMBER`

A number assigned to a record by an originating automated system that can be used by a receiving system to report results pertaining to that record.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`RIN` | A unique record identification number assigned to the record by the source system. This number is intended to serve as a more sure means of identification of a record between two interfacing systems. | 1--12 characters | None

`RIN` is known to be a substructure of all records *except* [HEAD] and [TRLR].


### ROLE

`http://terms.fhiso.org/legacy/longform/ROLE`

A name given to a role played by an individual in connection with an event.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`SOUR.EVEN.ROLE` | Either one of {`CHIL`, `HUSB`, `WIFE`, `MOTH`, `FATH`, `SPOU`}, or parentheses surrounding a word or phrase that identifies a person's role in an event being described---the same word or phrase, and in the same language, that the recorder used to define the role in the actual record. | 1--25 characters | None


### ROMN

`http://terms.fhiso.org/legacy/longform/ROMANIZED`

A romanized variation of a superior text string.

{.note} This tag was introduced in GEDCOM 5.5.1.

The method used to romanize the name is indicated by the subordinate `TYPE`; for example if romaji was used to provide a reading of a name written in kanji, then the `TYPE` subordinate to the `ROMN` tag would indicate `romaji`. 

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.ROMN` | The romanized variation of the name in the same form as the was the name used in the superior primitive.  | 1--120 characters | [TYPE]!, [NPFX]?, [GIVN]?, [NICK]?, [SPFX]?, [SURN]?, [NSFX]?, [NOTE]\*, [SOUR]\*
[Event]`.PLAC.ROMN` | The romanized variation of the place name in the same form as the was the name used in the superior primitive. | [TYPE]!


### SEX

`http://terms.fhiso.org/legacy/longform/SEX`

Indicates the sex of an individual---male or female.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.SEX` | A code that indicates the sex of the individual | 1--7 characters, which is one of {`M`, `F`} | None


### SOUR

`http://terms.fhiso.org/legacy/longform/SOURCE`

The initial or original material from which information was obtained.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.SOUR` |  A system identification name which was obtained through the GEDCOM registration process. This name must be unique from any other product. Spaces within the name must be substituted with a 0x5F (underscore `_`) so as to create one word. | 1--20 characters | [VERS]?, [NAME]?, [CORP]?, [DATA]?
`.SOUR` | Source records are used to provide a bibliographic description of the source cited. | None | [DATA]?, [AUTH]?, [TITL]?, [ABBR]?, [PUBL]?, [TEXT]?, [REPO]?, [OBJE]\*, [NOTE]\*, [REFN]\*, [RIN]?, [CHAN]?
`SOUR` | pointer to source record | pointer to `.SOUR` | [PAGE]?, [EVEN]?, [DATA]?, [QUAY]?, [OBJE]\*, [NOTE]\*
`SOUR` | for systems not using source records | unlimited numbers of characters | [TEXT]\*, [NOTE]\*

The non-pointer unanchored `SOUR` is described as follows:

> A free form text block used to describe the source from which information was obtained. This text block is used by those systems which cannot use a pointer to a source record. It must contain a descriptive title, who created the work, where and when it was created, and where is source data stored. The developer should encourage users to use an appropriate style for forming this free form bibliographic reference. Developers are encouraged to support the `.SOUR` method of reporting bibliographic reference descriptions. 

Both unanchored `SOUR` are known to be substructures of .[FAM], .[INDI], .[OBJE], .[NOTE], [ASSO], [Event], [NAME], and [PLAC].


### SPFX

`http://terms.fhiso.org/legacy/longform/SURN_PREFIX`

A name piece used as a non-indexing pre-part of a surname.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.SPFX` | Surname prefix or article used in a family name. Different surname articles are separated by a comma, for example in the name "de la Cruz", this value would be "de, la". | 1--30 characters | None


### SSN

`http://terms.fhiso.org/legacy/longform/SOC_SEC_NUMBER`

A number assigned by the United States Social Security Administration. Used for tax identification purposes.

See also [IDNO]

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.SSN`  | A number assigned to a person in the United States for identification purposes. | 9--11 characters | [IndividualAttribute] | [*inherited*](#individualattribute)


### STAE

`http://terms.fhiso.org/legacy/longform/STATE`

A geographical division of a larger jurisdictional area, such as a State within the United States of America.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.STAE` | The name of the state used in the address. Isolated for sorting or indexing. | 1--60 characters | None


### STAT

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


Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.FAMC.STAT` | A status code that allows passing on the users opinion of the status of a child to family link. | 1--15 characters; one of `challenged`, `disproven`, or `proven` | None



### SUBM

`http://terms.fhiso.org/legacy/longform/SUBMITTER`

An individual or organization who contributes genealogical data to a file or transfers it to someone else.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SUBM`       | The submitter record identifies an individual or organization that contributed information contained in the dataset. | None | [NAME]!, [ADDR]?, [OBJE]\*, [LANG]\*, [RFN]?, [RIN]?, [CHAN]?
`.HEAD.SUBM`  |         | pointer to `.SUBM` | None
`.FAM.SUBM`   |         | pointer to `.SUBM` | None
`.INDI.SUBM`  |         | pointer to `.SUBM` | None
`.SUBN.SUBM`  |         | pointer to `.SUBM` | None


### SUBN

`http://terms.fhiso.org/legacy/longform/SUBMISSION`

Pertains to a collection of data issued for processing.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SUBN` | The sending system uses a submission record to send instructions and information to the receiving system. | None | [SUBM]?, [ANCE]?, [DESC]?, [RIN]?; also various [LDS-specific tags]
`.HEAD.SUBN` | | pointer to a `.SUBN` | None


### SURN

`http://terms.fhiso.org/legacy/longform/SURNAME`

A family name passed on or used by members of a family.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.NAME.SURN` | Surname or family name. Different surnames are separated by a comma. | 1--120 characters | None


### TEXT

`http://terms.fhiso.org/legacy/longform/TEXT`

The exact wording found in an original source document.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`SOUR.TEXT`   | A verbatim copy of any description contained within the source. This indicates notes or text that are actually contained in the source document, not the submitter's opinion about the source. | arbitrary-length string | None
`SOUR.DATA.TEXT` | A verbatim copy of any description contained within the source. This indicates notes or text that are actually contained in the source document, not the submitter's opinion about the source. | arbitrary-length string | None


### TIME

`http://terms.fhiso.org/legacy/longform/TIME`

A time value in a 24-hour clock format, including hours, minutes, and optional seconds, separated by a colon (:). Fractions of seconds are shown in decimal notation.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.DATE.TIME` | The time of a specific event, usually a computer-timed event. | 1--12 characters matching `[0-2][0-9]:[0-5][0-9](:[0-5][0-9](\.[0-9][0-9]?)?)?` | None
`CHAN.DATE.TIME` | The time of a specific event, usually a computer-timed event. | 1--12 characters matching `[0-2][0-9]:[0-5][0-9](:[0-5][0-9](\.[0-9]+)?)?` | None


### TITL

`http://terms.fhiso.org/legacy/longform/TITLE`

A description of a specific writing or other work, such as the title of a book when used in a source context, or a formal designation used by an individual in connection with positions of royalty or other social status, such as Grand Duke.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`OBJE.TITL` | The title of a work, record, item, or object. | 1--248 characters | None | None
`.SOUR.TITL` | The title of the work, record, or item and, when appropriate, the title of the larger work or series of which it is a part. | arbtrary-length string | None | None
`.INDI.TITL` | The title given to or used by a person, especially of royalty or other noble class within a locality. | 1--120 characters | [IndividualAttribute] | [inherited](Event) 
`.OBJE.FILE.TITL` | The title of a work, record, item, or object. | 1--248 characters | None | None

{.note} In GEDCOM 5.5, `OBJE.TITL` can appear in both record and substructure `OBJE`; in 5.5.1 it may only appear in substructure `OBJE`.  In GEDCOM 5.5, `.OBJE.FILE.TITL` is not supported; in 5.5.1 it is.



### TRLR

`http://terms.fhiso.org/legacy/longform/TRAILER`

At level 0, specifies the end of a GEDCOM transmission.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.TRLR` | End of dataset | None | None

When encountering a `TRLR`, applications may cease parsing before even looking for a payload or substructures; it should thus always be the very last record in the dataset.  Additionally, it should never have an ID; some implementations may fail to parse it correctly if it does.


### TYPE

`http://terms.fhiso.org/legacy/longform/TYPE`

A further qualification to the meaning of the associated superior tag. The value does not have any computer processing reliability. It is more in the form of a short one or two word note that should be displayed any time the associated data is displayed.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`REFN.TYPE` | A user-defined definition of the [REFN]. | 1--40 characters | None 
[EVENT]`.TYPE` | A descriptor that should be used whenever the EVEN tag is used to define the event being cited. The event descriptor should use the same word or phrase and in the same language, when possible, as was used by the recorder of the event. | 1--90 characters | None
`FONE.TYPE` | Indicates the method used in transforming the text to the phonetic variation. | 5--30 characters (may be `hangul`, `kana`, or other) | None
`ROMN.TYPE` | Indicates the method used in transforming the text to a romanized variation. | 5--30 characters (may be `pinyin`, `romanji`, `wadegiles`, or other) | None


### VERS

`http://terms.fhiso.org/legacy/longform/VERSION`

Indicates which version of a product, item, or publication is being used or referenced.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.SOUR.VERS` | An identifier that represents the version level assigned to the associated product. It is defined and changed by the creators of the product. | 1--15 characters | None
`.HEAD.GEDC.VERS` | An identifier that represents the version level assigned to the associated product. It is defined and changed by the creators of the product. | 1--15 characters | None
`.HEAD.CHAR.VERS` | An identifier that represents the version level assigned to the associated product. It is defined and changed by the creators of the product. | 1--15 characters | None

{.note} This draft is based off of a specification for which `.HEAD.GEDC.VERS` would typically be `5.5`, though depending on the specific features used other version strings might be appropriate too.


### WIFE

`http://terms.fhiso.org/legacy/longform/WIFE`

An individual in the role as a mother and/or married woman.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.FAM.WIFE` | | pointer to a [INDI] | None
[FamilyEvent]`.WIFE` | | None | [AGE]!



### WILL

`http://terms.fhiso.org/legacy/longform/WILL`

A legal document treated as an event, by which a person disposes of his or her estate, to take effect after death. The event date is the date the will was signed while the person was alive. (See also [PROB]ate.) 

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.WILL`  | *see [IndividualEvent]* | Either `Y` or None | [IndividualEvent] | [*inherited*](#individualevent)


### WWW

`http://terms.fhiso.org/legacy/longform/WEB`

World Wide Web home page.

{.note} This tag was introduced in GEDCOM 5.5.1.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.WWW` | The world wide web page address. | 5--120 characters | None
