This is an early exploratory draft of a proposed FHISO standard that is fully compatible with GEDCOM 5.5.
Goals include 

-   Clarifying ambiguities
-   Updating presentation
-   Providing extension mechanisms suitable for anticipated FHISO extensions
-   Creating a archival standard, as GEDCOM 5.5 is no longer available from its authoring corporation.

The working title of this draft proposed standard is the FHISO Extended Legacy Format, or ELF.

This draft is intended to provide enough context for meaningful discussion of the value and desirable content of a potential future standard.
It will likely change significantly prior to being released.

----

# The organization of ELF data

## Structures

Each structure consists of the following parts:

Name       | Required? | Notes
-----------|-----------|------
Tag        | Required  | short identifiers describing structure meaning, of three kinds
Identifier | Required for *records*, optional otherwise | short identifiers allowing pointers to point to structure
Payload    | Required, Optional, or Forbidden (varies by Tag) | EITHER string content OR pointer (but never both)
Substructures | Required or Optional (varies by Tag) | structured content


A structure is called a *record* if it is not contained within another structure.
If a structure is contained within another structure, it is called a *substructure* and its containing structure is called its *superstructure*.
The *root* of a record is itself; the *root* of a substructure is the root of its parent.
The set of substructures sharing a common root are called the *nested substructures* of that root.

{.ednote} About the 32K limit: the OBJE tag allows exceeding this limit by letting one OBJE structure contain an OBJE pointer to the continuation of the structure.  I see no reason why this cannot be extended generally: a structure of any type may contain a single pointer to the same type, that being a continuation of the structure.  I thus believe we can leave the limit out of this discussion and leave it for the serialization discussion instead.

### Tags

A tag is a string of between 2 and 31 characters from the class `[A-Za-z0-9_]`.
No two distinct tags within a data set may share the same initial 15 characters.
It may contain at most one underscore, and maps to an IRI as follows:

-   If there is no underscore, the IRI of the tag is `http://fhiso.org/legacy/` followed by the tag.
    The set of no-underscore tags is limited to those defined in this document, has not changed since 1995, and will not be expanded in future documents.
    
    Tags of this kind are referred to in this document as "[legacy tags](#legacy-tags)".

-   If there is a leading underscore, the IRI of the tag is either undefined or is specified via the `ELF_SCEHMA` substructure of the `HEAD` record.

    Tags of this kind are referred to in this document as "old-style extension tags"

-   If there is an underscore but not in the leading position than the IRI of the tag is the IRI (specified in the `ELF_SCHEMA` substructure of the `HEAD` record) for the portion of the tag preceding the underscore, followed by the the portion of the tag following the underscore.

    Tags of this kind are referred to in this document as "new-style extension tags"

{.ednote} The above suggests we could write `ELF_SCHEMA` rather than `_ELF_SCHEMA`.  The latter is more fully GEDCOM-5.5 compliant.

### Identifiers

An identifier is between 1 and 22 characters.
The first character must be from the class `[A-Za-z0-9_]`;
the remaining characters MUST NOT be U+0040 (COMMERCIAL AT `@`)
or control characters (U+0000 through U+001F or U+007F).
Each record must have a unique identifier among the records of its containing dataset.
Each substructure that has an identifier must have a unique identifier among the nested substructures of its root.

### String content payloads

{.ednote} The GEDCOM 5.5 standard uses both the word "primitive" and the term "line item" instead of "payload".  Primitive I reject due to its overload with other computing contexts, and "line item" is defined at the lexer level and excludes `CONC`/`CONT`-extended payloads; hence my use of the work "payload" herein.

A payload is a sequence of zero or more payload elements, where each payload element is one of the following:

-   A unicode code-point, excluding the control characters (U+0000 through U+001F or U+007F)
-   A newline
-   An escape string, consisting of no more than 244 characters which may not include control characters (U+0000 through U+001F or U+007F).  Each COMMERCIAL AT character (U+0040, `@`) counts as two characters in computing lengths.

{.note} Both U+000A (LINE FEED) and U+000D (CARRIAGE RETURN) are control characters and thus may not appear in payloads.
Newlines are transmitted in structurally in a way that does not translate directly to those characters.
Implementations are welcome to use either U+000A or U+000D or both to represent newlines internally, but during encoding and transmission the newline representation chosen by the implementation will be lost.

Additionally, payloads have the following limitations

-   A payload MAY NOT begin or end with a space (U+0020)
-   A newline MAY NOT be preceded or followed by a space (U+0020)
-   A run of spaces (containing *e* escape strings) may not exceed 248 (&minus; 4*e*) characters

The **length** of a payload is only defined if it does not contain newlines.
When defined, it is the sum of the number of unicode code points, plus four times the number of escape strings, plus the total number of characters in all escape strings.
When a constraint is made on the length of a payload, implementations

-   MAY silently treat some or all payloads as if they were truncated to the given maximum length.
-   SHOULD handle the longer payload if they can.
-   MUST NOT cease functioning when encountering excess payload.

## Pointers

A pointer (also called a cross-reference pointer) is only encountered as a substructure.
Each pointer is exactly one of the following:

-   a string matching the cross-reference identifier of a record in the dataset.
    That record is called the *associated structure* of the pointer.

-   a string consisting of a U+0021 (EXCLAMATION MARK `!`) 
    followed by a string matching the identifier of a substructure contained within the record in which this cross-reference pointer appears.
    That substructure is called the *associated structure* of the pointer.

-   a string consisting of two parts, separated by a U+0021 (EXCLAMATION MARK `!`).
    This first part matches the cross-reference identifier of a record in the dataset.
    The second part matches the cross-reference identifier of a substructure of that record.
    That substructure is called the *associated structure* of the pointer.

-   a string containing U+003A (COLON `:`).
    The meaning of such a cross-reference pointer is not defined by this document,
    and new data sets SHOULD NOT include such pointers.
    
    {.ednote} We should probably give some suggested behavior for implementations encountering a `:` in a pointer.

    {.ednote} We could easily define the network reference syntax: something like `resolves to URL of a .ged` `:` `pointer without a ':' in it`; but I'm not sure of two things: either (1) that there is any value in doing so or (2) that some existing implementations have not already defined their own meaning for these pointers.

Some substructures are required to contain pointers; others may have payloads or pointers.
Some substructures always have pointers to records of the same type as the substructure that contains the pointer; others point to records of other types.
This specification does not mention any application for the substructure-pointer syntax (using `!`): all pointers listed here are to records.

### Contextualized Meaning

The meaning, structure, and constraints of a tag are sometimes dependent on the context in which the tag appears.
For example, a `NAME` inside a `SOUR` inside a `HEAD` identifies the title or name of a software product creating a data set, while a `NAME` inside an `INDI` identifies a personal name of a historical person. 

We use the following notation for **context specifiers** to identify the context of a structure.
The options are listed from least to most specific; when more than one context matches a structure, the most specific takes precedence.

-   `TAGNAME` matches any structure or pointer with that tag, anywhere it appears
-   `.TAGNAME` matches any record with that tag, but not substructures
-   *context specifier*`..TAGNAME` matches any structure or pointer with that tag, provided it is nested under a structure specified by the *context specifier*
-   *context specifier*`.TAGNAME` matches any structure or pointer with that tag, provided it is a substructure of a structure specified by the *context specifier*

{.ednote} Do we need this much generality?  Is this the right levels of specificity?  Either way, probably needs to be re-written for improved clarity.



# Legacy Types

The following outlines all of the type that have legacy tags associated with them.

When listing known substructures, each is followed by a single character:

- `!` means "required": there must be one and only one of these
- `?` means "optional": there may be one and no more than one of these
- `*` means "set-valued": there may be zero or more of these

{.ednote} this section is far from complete...

## Supertypes

Some tags in some contexts are logically collected into a type hierarchy.

### Event

`http://fihso.org/legacy/supertype/Event`

Any Event subtype is known to have the following substructures:

[ADDR](#addr)?
[AGE](#age)?
[AGNC](#agnc)?
[CAUS](#caus)?
[DATE](#date)?
[NOTE](#note)\*
[OBJE](#obje)\*
[PHON](#phon)\*
[PLAC](#plac)?
[SOUR](#sour)\*
[TYPE](#type)?

### IndividualEvent

`http://fihso.org/legacy/supertype/IndividualEvent`

A subtype of [Event](#event), representing an event that appertained to a particular individual.

### FamilyEvent

`http://fihso.org/legacy/supertype/FamilyEvent`

A subtype of [Event](#event), representing an event that appertained to a particular couple or nuclear family.

### IndividualAttribute

`http://fihso.org/legacy/supertype/IndividualAttribute`

A subtype of [Event](#event), representing attributes or facts are used to describe an individual's actions, physical description, employment, education, places of residence, etc.
These are not generally thought of as events.
However, they are often described like events because they were observed at a particular time and/or place.


## Tags

### ABBR

`http://fihso.org/legacy/longform/ABBREVIATION`

A short name of a title, description, or name.

Known Context | Meaning| Payload | Substructures 
--------------|--------|---------|--------------
`.SOUR.ABBR`  | A short title used for sorting, filing, and retrieving source records. | 1--60 characters | None


### ADDR

`http://fihso.org/legacy/longform/ADDRESS`

The contemporary place, usually required for postal purposes, of an individual, a submitter of information, a repository, a business, a school, or a company.

Payload
:   Address information that, when combined with NAME substructure, meets requirements for sending communications through the mail. 
    
    The first line of the payload SHOULD contain between 1 and 60 characters.
    Implementations MAY ignore excess characters on the first line of the payload.


Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.HEAD.SOUR.CORP.ADDR` || see above | [ADR1](#adr1)?, [ADR2](#adr2)?, [CITY](#city)?, [STAE](#stae)?, [POST](#post)?, [CTRY](#ctry)?
`.REPO.ADDR`  |         | see above | [ADR1](#adr1)?, [ADR2](#adr2)?, [CITY](#city)?, [STAE](#stae)?, [POST](#post)?, [CTRY](#ctry)?
`.SUBM.ADDR`  |         | see above | [ADR1](#adr1)?, [ADR2](#adr2)?, [CITY](#city)?, [STAE](#stae)?, [POST](#post)?, [CTRY](#ctry)?



### ADR1

`http://fihso.org/legacy/longform/ADDRESS1`

The first line of an address.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.ADR1` | The first line of the address used for indexing. This SHOULD correspond to the first line of the superstructure's payload. | 1--60 characters | None


### ADR2

`http://fihso.org/legacy/longform/ADDRESS2`

The second line of an address.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`ADDR.ADR2` |  The second line of the address used for indexing. This SHOULD correspond to the second line of the superstructure's payload. | 1--60 characters | None


### ADOP

`http://fihso.org/legacy/longform/ADOPTION`

Pertaining to creation of a child-parent relationship that does not exist biologically.

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.INDI.ADOP`  | If the payload is `Y` or there is [DATE](#date) or [PLAC](#plac) substructure, asserts that an adoption occurred; otherwise just discusses the idea of an adoption | Either `Y` or None | [IndividualEvent](#individualevent) | [FAMC](#famc)?
`ADOP.FAMC.ADOP` | Which parent(s) adopted | `HUSB`, `WIFE`, or `BOTH` | None | None


### AFN

`http://fihso.org/legacy/longform/AFN`

A unique permanent record file number of an individual record stored in Ancestral File.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.AFN`   | A unique permanent record number of an individual record contained in the Family History Department's Ancestral File.| 1--12 characters | None


### AGE

`http://fihso.org/legacy/longform/AGE`

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
`Event.AGE` | see above | see above | None


### AGNC

`http://fihso.org/legacy/longform/AGENCY`

The institution or individual having authority and/or responsibility to manage or govern.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SOUR.DATA.AGNC` | The organization, institution, corporation, person, or other entity that has authority or control interests in the associated context. For example, an organization responsible for creating and/or archiving records. | 1--120 characters | None
`Event.AGNC` | The organization, institution, corporation, person, or other entity that has authority or control interests in the associated context. For example, an employer of a person of an associated occupation, or a church that administered rites or event. | 1--120 characters | None

### ALIA

`http://fihso.org/legacy/longform/ALIAS`

An indicator to link different record descriptions of a person who may be the same person.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.ALIA`  |         | Pointer to an [INDI](#indi) | None


### ANCE

`http://fihso.org/legacy/longform/ANCESTORS`

Pertaining to forbearers of an individual.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SUBN`  |  The number of generations of ancestors included in this transmission. | 1--4 characters | None

{.ednote} Presumably this is supposed to be a base-10 integer?  Also, the Appendix A text and the Chapter 2 text for this tag don't seem to have much relationship to one another.


### ANCI

`http://fihso.org/legacy/longform/ANCES_INTEREST`

Indicates an interest in additional research for ancestors of this individual. (See also [DESI](#desi).)

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.ANCI`  |         | Pointer to an [SUBM](#subm) | None


### ANUL

`http://fihso.org/legacy/longform/ANNULMENT`

Declaring a marriage void from the beginning (never existed).

Known Context | Meaning | Payload | Supertype | Substructures 
--------------|---------|---------|-----------|--------------
`.FAM.ANUL`  | If the payload is `Y` or there is [DATE](#date) or [PLAC](#plac) substructure, asserts that an annulment occurred; otherwise just discusses the idea of an annulment | Either `Y` or None | [FamilyEvent](#familyevent) | None


### ASSO

`http://fihso.org/legacy/longform/ASSOCIATES`

An indicator to link friends, neighbors, relatives, or associates of an individual.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.INDI.ASSO`  |         | Pointer to an [INDI](#indi) | [RELA](#rela)!, [NOTE](#note)\*, [SOUR](#sour)\*


### AUTH

`http://fihso.org/legacy/longform/AUTHOR`

The name of the individual who created or compiled information.

Known Context | Meaning | Payload | Substructures 
--------------|---------|---------|--------------
`.SOUR.AUTH`  | The person, agency, or entity who created the record. For a published work, this could be the author, compiler, transcriber, abstractor, or editor. For an unpublished source, this may be an individual, a government agency, church organization, or private organization, etc. | String of arbitrary length | None

----

{.ednote) continue expanding this list

----

### BAPL

`http://fihso.org/legacy/longform/BAPTISM-LDS`

The event of baptism performed at age eight or later by priesthood authority of the LDS Church. (See also BAPM)

### BAPM

`http://fihso.org/legacy/longform/BAPTISM`

The event of baptism (not LDS), performed in infancy or later. (See also BAPL, and CHR.)

### BARM

`http://fihso.org/legacy/longform/BAR_MITZVAH`

The ceremonial event held when a Jewish boy reaches age 13.

### BASM

`http://fihso.org/legacy/longform/BAS_MITZVAH`

The ceremonial event held when a Jewish girl reaches age 13, also known as "Bat Mitzvah."

### BIRT

`http://fihso.org/legacy/longform/BIRTH`

The event of entering into life.

### BLES

`http://fihso.org/legacy/longform/BLESSING`

A religious event of bestowing divine care or intercession. Sometimes given in connection with a naming ceremony.

### BLOB

`http://fihso.org/legacy/longform/BINARY_OBJECT`

A grouping of data used as input to a multimedia system that processes binary data to represent images, sound, and video.

### BURI

`http://fihso.org/legacy/longform/BURIAL`

The event of the proper disposing of the mortal remains of a deceased person.

### CALN

`http://fihso.org/legacy/longform/CALL_NUMBER`

The number used by a repository to identify the specific items in its collections.

### CAST

`http://fihso.org/legacy/longform/CASTE`

The name of an individual's rank or status in society, based
on racial or religious differences, or differences in wealth, inherited
rank, profession, occupation, etc.

### CAUS

`http://fihso.org/legacy/longform/CAUSE`

A description of the cause of the associated event or fact, such as the cause of death.

### CENS

`http://fihso.org/legacy/longform/CENSUS`

The event of the periodic count of the population for a designated locality, such as a national or state Census.

### CHAN

`http://fihso.org/legacy/longform/CHANGE`

Indicates a change, correction, or modification. Typically used in connection with a DATE to specify when a change in information occurred.

### CHAR

`http://fihso.org/legacy/longform/CHARACTER`

An indicator of the character set used in writing this automated information.

### CHIL

`http://fihso.org/legacy/longform/CHILD`

The natural, adopted, or sealed (LDS) child of a father and a mother.

### CHR

`http://fihso.org/legacy/longform/CHRISTENING`

The religious event (not LDS) of baptizing and/or naming a child.

### CHRA

`http://fihso.org/legacy/longform/ADULT_CHRISTENING`

The religious event (not LDS) of baptizing and/or naming an adult person.

### CITY

`http://fihso.org/legacy/longform/CITY`

A lower level jurisdictional unit. Normally an incorporated municipal unit.

### CONC

`http://fihso.org/legacy/longform/CONCATENATION`

An indicator that additional data belongs to the superior value. The information from the CONC value is to be connected to the value of the superior preceding line without a space and without a carriage return and/or new line character. Values that are split for a CONC tag must always be split at a non-space. If the value is split on a space the space will be lost when concatenation takes place. This is because of the treatment that spaces get as a GEDCOM delimiter, many GEDCOM values are trimmed of trailing spaces and some systems look for the first non-space starting after the tag to determine the beginning of the value.

### CONF

`http://fihso.org/legacy/longform/CONFIRMATION`

The religious event (not LDS) of conferring the gift of the Holy Ghost and, among protestants, full church membership.

### CONL

`http://fihso.org/legacy/longform/CONFIRMATION_L`

The religious event by which a person receives membership in the LDS Church.

### CONT

`http://fihso.org/legacy/longform/CONTINUED`

An indicator that additional data belongs to the superior value. The information from the CONT value is to be connected to the value of the superior preceding line with a carriage return and/or new line character. Leading spaces could be important to the formatting of the resultant text. When importing values from CONT lines the reader should assume only one delimiter character following the CONT tag. Assume that the rest of the leading spaces are to be a part of the value.

### COPR

`http://fihso.org/legacy/longform/COPYRIGHT`

A statement that accompanies data to protect it from unlawful duplication and distribution.

### CORP

`http://fihso.org/legacy/longform/CORPORATE`

A name of an institution, agency, corporation, or company.

### CREM

`http://fihso.org/legacy/longform/CREMATION`

Disposal of the remains of a person's body by fire.

### CTRY

`http://fihso.org/legacy/longform/COUNTRY`

The name or code of the country.

### DATA

`http://fihso.org/legacy/longform/DATA`

Pertaining to stored automated information.

### DATE

`http://fihso.org/legacy/longform/DATE`

The time of an event in a calendar format.

### DEAT

`http://fihso.org/legacy/longform/DEATH`

The event when mortal life terminates.

### DESC

`http://fihso.org/legacy/longform/DESCENDANTS`

Pertaining to offspring of an individual.
### DESI

`http://fihso.org/legacy/longform/DESCENDANT_INT`

Indicates an interest in research to identify additional descendants of this individual. (See also ANCI.)

### DEST

`http://fihso.org/legacy/longform/DESTINATION`

A system receiving data.

### DIV

`http://fihso.org/legacy/longform/DIVORCE`

An event of dissolving a marriage through civil action.

### DIVF

`http://fihso.org/legacy/longform/DIVORCE_FILED`

An event of filing for a divorce by a spouse.

### DSCR

`http://fihso.org/legacy/longform/PHY_DESCRIPTION`

The physical characteristics of a person, place, or thing.

### EDUC

`http://fihso.org/legacy/longform/EDUCATION`

Indicator of a level of education attained.

### EMIG

`http://fihso.org/legacy/longform/EMIGRATION`

An event of leaving one's homeland with the intent of residing elsewhere.

### ENDL

`http://fihso.org/legacy/longform/ENDOWMENT`

A religious event where an endowment ordinance for an individual was performed by priesthood authority in an LDS temple.

### ENGA

`http://fihso.org/legacy/longform/ENGAGEMENT`

An event of recording or announcing an agreement between two people to become married.

### EVEN

`http://fihso.org/legacy/longform/EVENT`

A noteworthy happening related to an individual, a group, or an organization.

### FAM

`http://fihso.org/legacy/longform/FAMILY`

Identifies a legal, common law, or other customary relationship of man and woman and their children, if any, or a family created by virtue of the birth of a child to its biological father and mother.

### FAMC

`http://fihso.org/legacy/longform/FAMILY_CHILD`

Identifies the family in which an individual appears as a child.

### FAMF

`http://fihso.org/legacy/longform/FAMILY_FILE`

Pertaining to, or the name of, a family file. Names stored in a file that are assigned to a family for doing temple ordinance work.

### FAMS

`http://fihso.org/legacy/longform/FAMILY_SPOUSE`

Identifies the family in which an individual appears as a spouse.

### FCOM

`http://fihso.org/legacy/longform/FIRST_COMMUNION`

A religious rite, the first act of sharing in the Lord's supper as part of church worship.

### FILE

`http://fihso.org/legacy/longform/FILE`

An information storage place that is ordered and arranged for preservation and reference.

### FORM

`http://fihso.org/legacy/longform/FORMAT`

An assigned name given to a consistent format in which information can be conveyed.

### GEDC

`http://fihso.org/legacy/longform/GEDCOM`

Information about the use of GEDCOM in a transmission.

GIVN {GIVEN_NAME}
A given or earned name used for official identification of a person.

### GRAD

`http://fihso.org/legacy/longform/GRADUATION`

An event of awarding educational diplomas or degrees to individuals.

### HEAD

`http://fihso.org/legacy/longform/HEADER`

Identifies information pertaining to an entire GEDCOM transmission.

### HUSB

`http://fihso.org/legacy/longform/HUSBAND`

An individual in the family role of a married man or father.

### IDNO

`http://fihso.org/legacy/longform/IDENT_NUMBER`

A number assigned to identify a person within some significant external system.

### IMMI

`http://fihso.org/legacy/longform/IMMIGRATION`

An event of entering into a new locality with the intent of residing there.

### INDI

`http://fihso.org/legacy/longform/INDIVIDUAL`

A person.

### LANG

`http://fihso.org/legacy/longform/LANGUAGE`

The name of the language used in a communication or transmission of information.

### LEGA

`http://fihso.org/legacy/longform/LEGATEE`

A role of an individual acting as a person receiving a bequest or legal devise.

### MARB

`http://fihso.org/legacy/longform/MARRIAGE_BANN`

An event of an official public notice given that two people intend to marry.

### MARC

`http://fihso.org/legacy/longform/MARR_CONTRACT`

An event of recording a formal agreement of marriage, including the prenuptial agreement in which marriage partners reach agreement about the property rights of one or both, securing property to their children.

### MARL

`http://fihso.org/legacy/longform/MARR_LICENSE`

An event of obtaining a legal license to marry.

### MARR

`http://fihso.org/legacy/longform/MARRIAGE`

A legal, common-law, or customary event of creating a family unit of a man and a woman as husband and wife.

### MARS

`http://fihso.org/legacy/longform/MARR_SETTLEMENT`

An event of creating an agreement between two people contemplating marriage, at which time they agree to release or modify property rights that would otherwise arise from the marriage.

### MEDI

`http://fihso.org/legacy/longform/MEDIA`

Identifies information about the media or having to do with the medium in which information is stored.

### NAME

`http://fihso.org/legacy/longform/NAME`

A word or combination of words used to help identify an individual, title, or other item. More than one NAME line should be used for people who were known by multiple names.

### NATI

`http://fihso.org/legacy/longform/NATIONALITY`

The national heritage of an individual.

### NATU

`http://fihso.org/legacy/longform/NATURALIZATION`

The event of obtaining citizenship.

### NCHI

`http://fihso.org/legacy/longform/CHILDREN_COUNT`

The number of children that this person is known to be the parent of (all marriages) when subordinate to an individual, or that belong to this family when subordinate to a FAM_RECORD.

### NICK

`http://fihso.org/legacy/longform/NICKNAME`

A descriptive or familiar that is used instead of, or in addition to, one's proper name.

### NMR

`http://fihso.org/legacy/longform/MARRIAGE_COUNT`

The number of times this person has participated in a family as a spouse or parent.

### NOTE

`http://fihso.org/legacy/longform/NOTE`

Additional information provided by the submitter for understanding the enclosing data.

### NPFX

`http://fihso.org/legacy/longform/NAME_PREFIX`

Text which appears on a name line before the given and surname parts of a name.
i.e. ( Lt. Cmndr. ) Joseph /Allen/ jr.
In this example Lt. Cmndr. is considered as the name prefix portion.

### NSFX

`http://fihso.org/legacy/longform/NAME_SUFFIX`

Text which appears on a name line after or behind the given and surname parts of a name.
i.e. Lt. Cmndr. Joseph /Allen/ ( jr. )
In this example jr. is considered as the name suffix portion.

### OBJE

`http://fihso.org/legacy/longform/OBJECT`

Pertaining to a grouping of attributes used in describing something. Usually referring to the data required to represent a multimedia object, such an audio recording, a photograph of a person, or an image of a document.

### OCCU

`http://fihso.org/legacy/longform/OCCUPATION`

The type of work or profession of an individual.

### ORDI

`http://fihso.org/legacy/longform/ORDINANCE`

Pertaining to a religious ordinance in general.

### ORDN

`http://fihso.org/legacy/longform/ORDINATION`

A religious event of receiving authority to act in religious matters.

### PAGE

`http://fihso.org/legacy/longform/PAGE`

A number or description to identify where information can be found in a referenced work.

### PEDI

`http://fihso.org/legacy/longform/PEDIGREE`

Information pertaining to an individual to parent lineage chart.

### PHON

`http://fihso.org/legacy/longform/PHONE`

A unique number assigned to access a specific telephone.

### PLAC

`http://fihso.org/legacy/longform/PLACE`

A jurisdictional name to identify the place or location of an event.

### POST

`http://fihso.org/legacy/longform/POSTAL_CODE`

A code used by a postal service to identify an area to facilitate mail handling.

### PROB

`http://fihso.org/legacy/longform/PROBATE`

An event of judicial determination of the validity of a will. May indicate several related court activities over several dates.

### PROP

`http://fihso.org/legacy/longform/PROPERTY`

Pertaining to possessions such as real estate or other property of interest.

### PUBL

`http://fihso.org/legacy/longform/PUBLICATION`

Refers to when and/or were a work was published or created.

### QUAY

`http://fihso.org/legacy/longform/QUALITY_OF_DATA`

An assessment of the certainty of the evidence to support the conclusion drawn from evidence.

### REFN

`http://fihso.org/legacy/longform/REFERENCE`

A description or number used to identify an item for filing, storage, or other reference purposes.

### RELA

`http://fihso.org/legacy/longform/RELATIONSHIP`

A relationship value between the indicated contexts.

### RELI

`http://fihso.org/legacy/longform/RELIGION`

A religious denomination to which a person is affiliated or for which a record applies.

### REPO

`http://fihso.org/legacy/longform/REPOSITORY`

An institution or person that has the specified item as part of their collection(s).

### RESI

`http://fihso.org/legacy/longform/RESIDENCE`

The act of dwelling at an address for a period of time.

### RESN

`http://fihso.org/legacy/longform/RESTRICTION`

A processing indicator signifying access to information has been denied or otherwise restricted.

### RETI

`http://fihso.org/legacy/longform/RETIREMENT`

An event of exiting an occupational relationship with an employer after a qualifying time period.

### RFN

`http://fihso.org/legacy/longform/REC_FILE_NUMBER`

A permanent number assigned to a record that uniquely identifies it within a known file.

### RIN

`http://fihso.org/legacy/longform/REC_ID_NUMBER`

A number assigned to a record by an originating automated system that can be used by a receiving system to report results pertaining to that record.

### ROLE

`http://fihso.org/legacy/longform/ROLE`

A name given to a role played by an individual in connection with an event.

### SEX

`http://fihso.org/legacy/longform/SEX`

Indicates the sex of an individual--male or female.

### SLGC

`http://fihso.org/legacy/longform/SEALING_CHILD`

A religious event pertaining to the sealing of a child to his or her parents in an LDS temple ceremony.

### SLGS

`http://fihso.org/legacy/longform/SEALING_SPOUSE`

A religious event pertaining to the sealing of a husband and wife in an LDS temple ceremony.

### SOUR

`http://fihso.org/legacy/longform/SOURCE`

The initial or original material from which information was obtained.

### SPFX

`http://fihso.org/legacy/longform/SURN_PREFIX`

A name piece used as a non-indexing pre-part of a surname.

### SSN

`http://fihso.org/legacy/longform/SOC_SEC_NUMBER`

A number assigned by the United States Social Security Administration. Used for tax identification purposes.

### STAE

`http://fihso.org/legacy/longform/STATE`

A geographical division of a larger jurisdictional area, such as a State within the United States of America.

### STAT

`http://fihso.org/legacy/longform/STATUS`

An assessment of the state or condition of something.

### SUBM

`http://fihso.org/legacy/longform/SUBMITTER`

An individual or organization who contributes genealogical data to a file or transfers it to someone else.

### SUBN

`http://fihso.org/legacy/longform/SUBMISSION`

Pertains to a collection of data issued for processing.

### SURN

`http://fihso.org/legacy/longform/SURNAME`

A family name passed on or used by members of a family.

### TEMP

`http://fihso.org/legacy/longform/TEMPLE`

The name or code that represents the name a temple of the LDS Church.

### TEXT

`http://fihso.org/legacy/longform/TEXT`

The exact wording found in an original source document.

### TIME

`http://fihso.org/legacy/longform/TIME`

A time value in a 24-hour clock format, including hours, minutes, and optional seconds, separated by a colon (:). Fractions of seconds are shown in decimal notation.

### TITL

`http://fihso.org/legacy/longform/TITLE`

A description of a specific writing or other work, such as the title of a book when used in a source context, or a formal designation used by an individual in connection with positions of royalty or other social status, such as Grand Duke.

### TRLR

`http://fihso.org/legacy/longform/TRAILER`

At level 0, specifies the end of a GEDCOM transmission.

### TYPE

`http://fihso.org/legacy/longform/TYPE`

A further qualification to the meaning of the associated superior tag. The value does not have any computer processing reliability. It is more in the form of a short one or two word note that should be displayed any time the associated data is displayed.

### VERS

`http://fihso.org/legacy/longform/VERSION`

Indicates which version of a product, item, or publication is being used or referenced.

### WIFE

`http://fihso.org/legacy/longform/WIFE`

An individual in the role as a mother and/or married woman.

### WILL

`http://fihso.org/legacy/longform/WILL`

A legal document treated as an event, by which a person disposes of his or her estate, to take effect after death. The event date is the date the will was signed while the person was alive. (See also [PROB](#prob)ate.) 
