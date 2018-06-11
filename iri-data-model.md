{.ednote ...} This is an early WIP complete re-write of `data-model.md`. Goals:

- Avoid quoting GEDCOM, and where necessary attribute the quote.
- Use IRI = meaning instead of tag-hierarchy = meaning.
{/}


## Microformats

Several microformats are used in payloads of various structures below.

### Personal name format {#personal-name)

A full name, presented in the order usually spoken and with the capitalization typical of the culture of the named individual.

The text *should not* include commas or digits.

It *should* include exactly two U+002F SOLIDUS `/` characters, one on each side of the family name or surname if present, or adjacent to one another if no family name or surname name is known.

Portions of the name *may* be elided and replaced by three U+002E FULL STOP `...`.
This *should* only be done if part of a name is known to exist but its content is not known.

### Language tag format {#language-tag}

{.ednote} TO DO: fill this section using the "either GEDCOM or IANA" concept outlined in `languages.tsv`.

### Age format {#age}

Principally to describe an age, but may also be used for any other quantity of time.

Matches the production `Age`:

    Age  ::= [<>]? ([0-9]+ "y")? ([0-9]+ "m")? ([0-9]+ "d")? 
            | CHILD | INFANT | STILLBORN

The component pieces of the microformat have the following meanings:

| Symbol       | Meaning                                |
|--------------|----------------------------------------|
| `>`          | greater than indicated age             |
| `<`          | less than indicated age                |
| `[0-9]+ "y"` | a number of years                      |
| `[0-9]+ "m"` | a number of months                     |
| `[0-9]+ "d"` | a number of days                       |
| `CHILD`      | `<8y`                                  |
| `INFANT`     | `<1y`                                  |
| `STILLBORN`  | just prior, at, or near birth; or `0y` |

Additional white space may be inserted between tokens without changing meaning.

{.note} GEDCOM did not define if a missing component should be taken to mean 0 or unknown. Common practice appears to be to assume that larger-resolution missing components are 0, but smaller are unknown. Thus, `2m` would be taken to mean  between `0y 2m 0d` and `0y 2m 30d`. However, this practice is not explicit in GEDCOM and data that violates it are likely present in the wild.

### Date formats

Dates are represented using a somewhat involved syntax with three entry points, documented below.

{.ednote ...} The following is the EBNF for date payloads, but it lacks semantics

    Date    ::= greg | juln | hebr | fren | future

    Exact   ::= [1-9] [0-9]? month year_g

    Value   ::= Date | Period | range | approx
                | ( "INT" Date )? "(" String ")"

    Period  ::= "FROM" Date ( "TO" Date )? | "TO" Date

    approx  ::= ( "ABT" | "CAL" | "EST" ) Date
    range   ::= ( "BEF" | "AFT" ) Date | "BET" Date "AND" Date
    
    greg    ::= ("@#DGREGORIAN@" #x20)? d_greg
    juln    ::= "@#DJULIAN@" #x20 d_juln
    hebr    ::= "@#DHEBREW@" #x20 d_hebr
    fren    ::= "@#DFRENCH R@" #x20 d_fren
    future  ::= "@#D" ( "ROMAN" | "UNKNOWN" ) "@" #x20 String
    
    d_fren  ::= ( ( [1-9] [0-9]? )? month_f )? year
    d_greg  ::= ( ( [1-9] [0-9]? )? month )? year_g
    d_hebr  ::= ( ( [1-9] [0-9]? )? month_h )? year
    d_juln  ::= ( ( [1-9] [0-9]? )? month )? year
    
    year_g  ::= [1-9] [0-9]* ( "/" [0-9] [0-9] )? "(B.C.)"?
    year    ::= [1-9] [0-9]* "(B.C.)"?
    
    month   ::= "JAN" | "FEB" | "MAR" | "APR" | "M‌AY" | "JUN"
                | "JUL" | "AUG" | "SEP" | "OCT" | "NOV" | "DEC"

    month_f ::= "VEND" | "BRUM" | "FRIM" | "NIVO" | "PLUV" | "VENT" | "GERM"
                | "FLOR" | "PRAI" | "MESS" | "THER" | "FRUC" | "COMP"

    month_h ::= "TSH" | "CSH" | "KSL" | "TVT" | "SHV" | "ADR" | "ADS"
                | "NSN" | "IYR" | "SVN" | "TMZ" | "AAV" | "ELL"
    
{/}

#### Date {#date-format}

At the core of the date syntax is a calendared date.
This consists of an optional *calender escape* followed by the content of the date.

The *calender escape* is a substring beginning `@#D` and ending `@`, between which is a calender identifier; known calender identifiers are `GREGORIAN`, `FRENCH R`, `HEBREW`, `JULIAN`, `ROMAN`, and `UNKNOWN`.
If no calender escape is given, `GREGORIAN` is assumed.

{.ednote} Should we move the escape syntax to [ELF-Serialization] and change the above to describe an abstract notion of "an escape"?

{.note} Some dates (in particular the [Period](#date-period) and [Value](#date-value) productions) may have multiple [Date](#date-format) values; it is not known if current implementations can handle situations where the dates are from different calenders, nor if they assume an uncalendered date paired with a calendered date is `GREGORIAN` or the same as the other date provided.  It is RECOMMENDED that the same calender be used for both Dates in such payloads.

The `ROMAN` and `UNKNOWN` calenders's date formats are not defined in this specification.

`GREGORIAN`, `FRENCH R`, `HEBREW`, and `JULIAN` dates all have the format "day month year", separated by spaces; the day may be omitted; if the day is omitted, the month may be omitted as well.
The three pieces are formatted as follows:
    
day
:   A decimal number of one or two digits.
    This SHOULD NOT be zero or greater than the number of days in the appropriate month.
    This specification does not specify whether single-digit days should begin with a zero or not.

{.ednote} Should we specify leading 0s are preferred?

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
        Shows the possible date alternatives brought about when the beginning of the year changed from `MAR` to `JAN`: for example, `15 APR 1699/00`.
        
        The `/` MUST NOT have a space on either side.
    
    BCE
    :   Represented as `(B.C.)`, which SHOULD be preceded by a space.
        Indicates a date before the birth of Christ.
    
    If both suffixes are present, `(B.C.)` comes last.

#### Exact Date {#exact-date}

An *exact date* is a `GREGORIAN` [Date](#date-format)s with the following additional constraints:

-   They MUST NOT include a *calender escape*
-   They MUST include the day and month
-   They MUST NOT have either year suffix

#### Date Period {#date-period}

A *date period* is one of the following three forms:

-   `FROM` [Date](#date-format)
-   `TO` [Date](#date-format)
-   `FROM` [Date](#date-format) `TO` [Date](#date-format)

#### Date Value {#date-value}

A *date value* may have any of a variety of formats:

| Format               | Meaning                                                           |
|----------------------|-------------------------------------------------------------------|
| [Date](#date-format) | |
| [Date Period](#date-period) | |
| `BEF` [Date](#date-format) | before the given date |
| `AFT` [Date](#date-format) | after the given date |
| `BET` [Date](#date-format) `AND` [Date](#date-format) | between the given dates; the first date SHOULD be earlier than the second date |
| `ABT` [Date](#date-format) | about; the given date is not exact |
| `CAL` [Date](#date-format) | calculated mathematically, for example, from an event date and age |
| `EST` [Date](#date-format) | estimated based on some other event date |
| `INT` [Date](#date-format) `(`arbitrary text`)` | interpreted from knowledge about the associated date phrase included in parentheses |
| `(`arbitrary text`)` | information about when an event occurred that is not recognizable to a date parser |

{.ednote} Is the above table or the below list more understandable? We definitely don't need both...

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


## Structure types

### `elf:Record`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

`elf:Record` types always contain cross-reference IDs.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:AUTOMATED_RECORD_ID]` ?
:   `[elf:CHANGE_DATE]` ?
:   `[elf:NOTE_STRUCTURE]` \*
:   `[elf:USER_REFERENCE_NUMBER]` \*

Subtypes
:   `[elf:FAM_RECORD]`
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elf:MULTIMEDIA_RECORD]`
:   `[elf:NOTE_RECORD]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SOURCE_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

### `elf:SUBMITTER_POINTER`

{.ednote} GEDCOM limits these to at most one per HEAD, FAM, and INDI. This seems odd; surely a source, note, etc., can also have a submitter, and there can be more than one contributing submitter per record...

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FAM_RECORD]`
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elfm:HEADER]`

Payload
:   A pointer to an `[elf:SUBMITTER_RECORD]`

    Indicates that the pointed-to `[elf:SUBMITTER_RECORD]` describes a contributor of information to the containing structure.

Default tag
:   `SUBM`

### `elf:USER_REFERENCE_NUMBER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Record]`

Substructures
:   `[elf:USER_REFERENCE_TYPE]` ?

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.

    A user-defined identifier (textual or numberic) of this record.
    In GEDCOM, the examples suggests this was to allow brief links to another record keeping system, though it's non-multi-values character limits that functionality.

Default tag
:   `REFN`


### `elf:USER_REFERENCE_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:USER_REFERENCE_NUMBER]`

Payload
:   A *line string*. It is RECOMMENDED that implementations support payloads of at least 40 characters.

    A user-defined definition of the superstructure.

Default tag
:   `TYPE`

### `elf:AUTOMATED_RECORD_ID`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Record]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 12 characters.

    A record identifier (numeric or textual) that is supposed to be unique within an originating system.
    Since there is no indication of which system, nor a mechanism for spanning systems,
    this has little value when communicating between systems.

Default tag
:   `RIN`


### `elf:FAM_RECORD`

Used to record couple and parent/child relationships.

Because of the social context in which GEDCOM was first created and because `elf:FAM_RECORD`s are used in some software applications to present binary ancestry trees and *n*-arry descendancy trees, each `elf:FAM_RECORD` is limited to having at most one "first-position" parent; at most one "second-position" parent; and any number of ordered children. GEDCOM explicitly stated that the first-position parent was male and the second-position parent was female; that is not true of how GEDCOM has been used in practice and MUST NOT be assumed by any conformant ELF implementation.

Supertype
:   `[elf:Record]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:RESTRICTION_NOTICE]` ?
:   `[elf:FamilyEvent]` \*
:   `[elf:PARENT1_POINTER]` ?
:   `[elf:PARENT2_POINTER]` ?
:   `[elf:CHILD_POINTER]` \*
:   `[elf:COUNT_OF_CHILDREN#Family]` ?
:   `[elf:MULTIMEDIA_LINK]` \*
:   `[elf:SOURCE_CITATION]` \*
:   `[elf:SUBMITTER_POINTER]` \*

Payload
:   None

Default tag
:   `FAM`

### `elf:ParentPointer`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

{.ednote} This supertype is added primarily as a place-holder for potential future extensions to support more general models of families.

Supertype
:   `[elf:Structure]`

Subtypes
:   `[elf:PARENT1_POINTER]`
:   `[elf:PARENT2_POINTER]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`


### `elf:PARENT1_POINTER`

A pointer to the spouse or parent traditionally presented on the left fork of a vertical family tree or on the upper fork of a horizontal family tree. In a heterosexual pair union, this is traditionally the husband or father.

Supertype
:   `[elf:ParentPointer]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`

Default tag
:   `HSUB`


### `elf:PARENT2_POINTER`

A pointer to the spouse or parent traditionally presented on the right fork of a vertical family tree or on the bottom fork of a horizontal family tree. In heterosexual pair unions, this is traditionally the wife or mother.

Supertype
:   `[elf:ParentPointer]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`

Default tag
:   `WIFE`

### `elf:CHILD_POINTER`

A pointer one of the children in a family.

The preferred order of the children pointers within a family structure is chronological by birth. 

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`

    It *must* be the case that the pointed-to `[elf:INDIVIDUAL_RECORD]` contains a `[elf:CHILD_TO_FAMILY_LINK]` pointing to the superstructure of this structure.

Default tag
:   `CHIL`

### `elf:INDIVIDUAL_RECORD`

A representation of a historical individual, together with the facts and events believed to apply to that individual and the sources of those data.

Supertype
:   `[elf:Record]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:RESTRICTION_NOTICE]` ?
:   `[elf:PERSONAL_NAME_STRUCTURE]` \*
:   `[elf:SEX_VALUE]` ?
:   `[elf:IndividualEvent]` \*
:   `[elf:IndividualAttribute]` \*
:   `[elf:CHILD_TO_FAMILY_LINK]` \*
:   `[elf:SPOUSE_TO_FAMILY_LINK]` \*
:   `[elf:ASSOCIATION_STRUCTURE]` \*
:   `[elf:ALIAS_POINTER]` \*
:   `[elf:ANCESTOR_INTEREST_POINTER]` \*
:   `[elf:DESCENDANT_INTEREST_POINTER]` \*
:   `[elf:MULTIMEDIA_LINK]` \*
:   `[elf:SOURCE_CITATION]` \*
:   `[elf:SUBMITTER_POINTER]` \*

{.note} GEDCOM permitted a `PERMANENT_RECORD_FILE_NUMBER` with tag `RFN`, the value of which was under-defined and not included in this one.

Payload
:   None

Default tag
:   `INDI`


### `elf:ALIAS_POINTER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`
    
    Points to a different `[elf:INDIVIDUAL_RECORD]` that may describe the same historical individual as the superstructure.

Default tag
:   `ALIA`

### `elf:ANCESTOR_INTEREST_POINTER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Payload
:   A pointer to an `[elf:SUBMITTER_RECORD]`

    Indicates that the pointed-to `[elf:SUBMITTER_RECORD]` describes someone interested in the ancestors of the individual described by the superstructure.

Default tag
:   `ANCI`


### `elf:DESCENDANT_INTEREST_POINTER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Payload
:   A pointer to an `[elf:SUBMITTER_RECORD]`

    Indicates that the pointed-to `[elf:SUBMITTER_RECORD]` describes someone interested in the descendants of the individual described by the superstructure.

Default tag
:   `DESI`


### `elf:MULTIMEDIA_RECORD`

The form of this record was changed between GEDCOM 5.5 and GEDCOM 5.5.1. Implementations should accept both formats but export only 5.5.1 format.

Supertype
:   `[elf:Record]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:MULTIMEDIA_FILE_REFERENCE]` \* -- GEDCOM 5.5.1
:   `[elf:MULTIMEDIA_FORMAT]` ! -- GEDCOM 5.5
:   `[elf:DESCRIPTIVE_TITLE]` ? -- GEDCOM 5.5
:   `[elf:CONTINUED_BINARY_OBJECT]` ? -- GEDCOM 5.5
:   `[elf:BINARY_OBJECT]` ! -- GEDCOM 5.5

Payload
:   None

Default tag
:   `OBJE`


### `elf:MULTIMEDIA_FILE_REFERENCE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]`
:   `[elf:MULTIMEDIA_LINK]`

Substructures
:   `[elf:MULTIMEDIA_FORMAT]` !
:   `[elf:DESCRIPTIVE_TITLE]` ?

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.

    Contains locator information (such as a file path or IRL) for a file containing auxiliary data.

{.ednote} GEDCOM is vague on payload format. Presumably there exist a variety of formats in the wild. Should we perform a survey and see if we can provide guidance on payload format?

Default tag
:   `FILE`


### `elf:MULTIMEDIA_FORMAT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]` -- GEDCOM 5.5
:   `[elf:MULTIMEDIA_FILE_REFERENCE]` -- GEDCOM 5.5.1
:   `[elf:MULTIMEDIA_LINK]`

Substructures
:   `[elf:SOURCE_MEDIA_TYPE]` ?

Payload
:   A *line string*. Known values include {`bmp`, `gif`, `jpg`, `ole`, `pcx`, `tif`, `wav`}.

    Indicates the format of the multimedia data associated with the superstructure of the `elf:MULTIMEDIA_FORMAT` structure.

{.ednote} Should we expand this to allow MIME-type as well as the seven known Windows-style file endings?

Default tag
:   `FORM`

### `elf:DESCRIPTIVE_TITLE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]` -- GEDCOM 5.5
:   `[elf:MULTIMEDIA_FILE_REFERENCE]` -- GEDCOM 5.5.1
:   `[elf:MULTIMEDIA_LINK]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 248 characters.

    The title of a work, record, item, or object.

Default tag
:   `TITL`


### `elf:NOTE_RECORD`

Supertype
:   `[elf:Record]`

Superstructures
:   `[elfm:Document]`

Payload
:   A *block string* of arbitrary length.

Default tag
:   `NOTE`

{.note} GEDCOM did not `[elf:NOTE_STRUCTURE]` as a substructure of `elf:NOTE_RECORD`, but they do appear in the wild and have valid semantics (notes about the note itself) so `elf:NOTE_RECORD` inherits the `[elf:NOTE_STRUCTURE]` substructure from `[elf:Record]` in this specification.


### `elf:REPOSITORY_RECORD`

A representation of where a source or set of sources is located. May be formal, like a library, or informal, like the owner of a family bible.

Supertype
:   `[elf:Record]`
:   `[elf:Agent]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:NAME_OF_REPOSITORY]` !

Payload
:   None

Default tag
:   `REPO`

### `elf:SOURCE_RECORD`

Supertype
:   `[elf:Record]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:SOURCE_RECORD_DATA]` ?
:   `[elf:SOURCE_ORIGINATOR]` ?
:   `[elf:SOURCE_DESCRIPTIVE_TITLE]` ?
:   `[elf:SOURCE_FILED_BY_ENTRY]` ?
:   `[elf:SOURCE_PUBLICATION_FACTS]` \*
:   `[elf:TEXT_FROM_SOURCE]` ?
:   `[elf:SOURCE_REPOSITORY_CITATION]` \*
:   `[elf:MULTIMEDIA_LINK]` \*

Payload
:   None

Default tag
:   `SOUR`

### `elf:SOURCE_RECORD_DATA`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD]`

Substructures
:   `[elf:EVENTS_RECORDED]` \*
:   `[elf:RESPONSIBLE_AGENCY]` ?
:   `[elf:NOTE_STRUCTURE]` \*

Payload
:   None

Default tag
:   `DATA`

### `elf:SUBMITTER_RECORD`

Supertype
:   `[elf:Record]`
:   `[elf:Agent]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:SUBMITTER_NAME]` !
:   `[elf:MULTIMEDIA_LINK]` \*
:   `[elf:LANGUAGE_PREFERENCE]` \*
:   *should not* contain a `[elf:USER_REFERENCE_NUMBER]` even though it is an `[elf:Record]`

Payload
:   None

{.note} GEDCOM permitted a `SUBMITTER_REGISTERED_RFN` with tag `RFN`, the value of which needed to to be preregistered with Ancestral File, a service that is no longer available. It has thus been removed from this specification, making it an extension tag.

Default tag
:   `SUBM`






### `elf:Agent`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

An `elf:Agent` structure represents an entity that may be contacted, such as a person, corporation, or archive.

Supertype
:   `[elf:Structure]`

Substructures
:   `[elf:ADDRESS]` ?
:   `[elf:ADDRESS_WEB_PAGE]` \*
:   `[elf:ADDRESS_FAX]` \*
:   `[elf:ADDRESS_EMAIL]` \*
:   `[elf:PHONE_NUMBER]` \*

Subtypes
:   `[elf:NAME_OF_BUSINESS]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

### `elf:ADDRESS`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`
:   `[elf:Agent]`

Substructures
:   `[elf:ADDRESS_LINE1]`
:   `[elf:ADDRESS_LINE2]`
:   `[elf:ADDRESS_LINE3]`
:   `[elf:ADDRESS_CITY]`
:   `[elf:ADDRESS_STATE]`
:   `[elf:ADDRESS_POSTAL_CODE]`
:   `[elf:ADDRESS_COUNTRY]`

Payload
:   A *block string* of arbitrary length.

    The fully-formatted address, as it would appear for shipment labels.
    This *should not* be omitted even if all of its information is contained in substructures.

Default tag
:   `ADDR`

### `elf:PHONE_NUMBER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Agent]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.
    
    This contains a telephone number.
    It is *recommended* that this be an international telephone number.

{.ednote} Add appropriate reference to ITU-T E.123 and E.164 (I think; I'm not up on these standards)

Default tag
:   `PHON`

### `elf:ADDRESS_EMAIL`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Agent]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    This contains an email address.
    It is *recommended* that this match production `addr-spec` of [RFC 5322].

{.ednote} Look up RFC 5322 and make sure addr-spec is what we want

Default tag
:   `EMAIL`
:   `EMAI`

### `elf:ADDRESS_FAX`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Agent]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    This contains a telephone number that will connect to a fax machine.
    It is *recommended* that this be an international telephone number.

{.ednote} Add appropriate reference to ITU-T T.4 (I think; I'm not up on these standards)

Default tag
:   `FAX`

### `elf:ADDRESS_WEB_PAGE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Agent]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    This *should* contain a single IRL, as defined in [RFC 1736].

{.ednote} Look up RFC 1736 and make sure the above is correct.

Default tag
:   `WWW`


### `elf:ASSOCIATION_STRUCTURE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   `[elf:RELATION_IS_DESCRIPTOR]` !
:   `[elf:SOURCE_CITATION]` \*
:   `[elf:NOTE_STRUCTURE]` \*

Payload
:   A *pointer* to a `[elf:INDIVIDUAL_RECORD]`

{.ednote} While GEDCOM unambiguously stated this was a pointer to an INDIVIDUAL_RECORD, it also contained an example (under the definition of RELATION_IS_DESCRIPTOR) where it was a pointer to a SUBMITTER_RECORD instead.

Default tag
:   `ASSO`


### `elf:CHANGE_DATE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Record]`

Substructures
:   `[elf:CHANGE_DATE_DATE]` !
:   `[elf:NOTE_STRUCTURE]` \*

Payload
:   None

Default tag
:   `CHAN`

{.ednote} GEDCOM uses the token CHANGE_DATE in two ways. Page 31 defines what we call `[elf:CHANGE_DATE]`, a structure containing a date and an arbitrary number of notes; page 44 defines what we call `elf:CHANGE_DATE_DATE`, a payload-only format structure. 

### `elf:CHANGE_DATE_DATE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:CHANGE_DATE]`

Substructures
:   `[elf:TIME_VALUE]` ?

Payload
:   A string matching the [Exact Date](#exact-date) syntax.
    
    Indicates the last change to the containing structure.

Default tag
:   `DATE`

### `elf:TIME_VALUE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:CHANGE_DATE_DATE]`
:   `[elf:TRANSMISSION_DATE]`

Substructures
:   None

:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 12 characters.
    
    The string *should* match the `Time` production:
    
        Time    ::= hour ":" minute ( ":" second )?
        hour    ::= [01] [0-9] | "2" [0-3]
        minute  ::= [0-5] [0-9]
        second  ::=  [0-5] [0-9] ( "." [0-9]+ )?

Default tag
:   `TIME`


### `elf:CHILD_TO_FAMILY_LINK`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   `[elf:PEDIGREE_LINKAGE_TYPE]` ?
:   `[elf:CHILD_LINKAGE_STATUS]` ?
:   `[elf:NOTE_STRUCTURE]` \*

Payload
:   A *pointer* to a `[elf:FAM_RECORD]`

    It *must* be the case that the pointed-to `[elf:FAM_RECORD]` contains a `[elf:CHILD_POINTER]` pointing to the superstructure of this structure.

Default tag
:   `FAMC`

### `elf:PEDIGREE_LINKAGE_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:CHILD_TO_FAMILY_LINK]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 7 characters.

    Contains a description of how this child is related to the superstructure's pointed-to `[elf:FAM_RECORD]`.
    Known values include {`adopted`, `birth`, `foster`}.

Default tag
:   `PEDI`


### `elf:CHILD_LINKAGE_STATUS`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:CHILD_TO_FAMILY_LINK]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 15 characters.

    Contains a description of the confidence that this relationship exists.
    Known values include {`challenged`, `disproven`, `proven`}.
    
    No matter the contents, there *should* be a `[elf:NOTE_STRUCTURE]` within this structure's superstructure that describes the the proof or challenge.

Default tag
:   `STAT`


### `elf:Event`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

Supertype
:   `[elf:Structure]`

Superstructures
:   None

Substructures
:   `[elf:EVENT_OR_FACT_CLASSIFICATION]` ?
:   `[elf:DATE_VALUE]` ?
:   `[elf:PLACE_STRUCTURE]` ?
:   `[elf:ADDRESS]` ?
:   `[elf:RESPONSIBLE_AGENCY]` ?
:   `[elf:RELIGIOUS_AFFILIATION]` ?
:   `[elf:CAUSE_OF_EVENT]` ?
:   `[elf:RESTRICTION_NOTICE]` ?
:   `[elf:NOTE_STRUCTURE]` \*
:   `[elf:SOURCE_CITATION]` \*
:   `[elf:MULTIMEDIA_LINK]` \*

Subtypes
:   `[elf:FamilyEvent]`
:   `[elf:IndividualAttribute]`
:   `[elf:IndividualEvent]`

{.note} GEDCOM suggested that `elf:Event` was a subtype of `[elf:Agent]` and thus could have `[elf:ADDRESS_WEB_PAGE]`, etc, inside; this appears to be a mistake as almost no historical event has any of that information.

### `elf:FamilyEvent`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

Supertype
:   `[elf:Event]`

Superstructures
:   `[elf:FAM_RECORD]`

Substructures
:   `[elf:Parent1Age]` ?
:   `[elf:Parent2Age]` ?

Subtypes
:   `[elf:ANNULMENT]`
:   `[elf:CENSUS#Family]`
:   `[elf:DIVORCE]`
:   `[elf:DIVORCE_FILED]`
:   `[elf:ENGAGEMENT]`
:   `[elf:MARRIAGE_BANN]`
:   `[elf:MARRIAGE_CONTRACT]`
:   `[elf:MARRIAGE]`
:   `[elf:MARRIAGE_LICENSE]`
:   `[elf:MARRIAGE_SETTLEMENT]`
:   `[elf:RESIDENCE]`
:   `[elf:EVENT#Family]`

Payload
:   A *string*, which may be limited by subtypes.
    
    The special value `Y` indicates an assertion that the event in question did occur,
    even if it has no subordinate date or place.

### `elf:Parent1Age`

An intermediate structure to indicate the age of a spouse or parent at the time of an event.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FamilyEvent]`

Substructures
:   `[elf:AGE_AT_EVENT]`

Payload
:   None

Default tag
:   `HUSB`

### `elf:Parent2Age`

An intermediate structure to indicate the age of a spouse or parent at the time of an event.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FamilyEvent]`

Substructures
:   `[elf:AGE_AT_EVENT]`

Payload
:   None

Default tag
:   `WIFE`


### `elf:IndividualAttribute`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

Supertype
:   `[elf:Event]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   *inherited*

Subtypes
:   `[elf:CASTE_NAME]`
:   `[elf:PHYSICAL_DESCRIPTION]`
:   `[elf:SCHOLASTIC_ACHIEVEMENT]`
:   `[elf:NATIONAL_ID_NUMBER]`
:   `[elf:NATIONAL_OR_TRIBAL_ORIGIN]`
:   `[elf:COUNT_OF_CHILDREN#Individual]`
:   `[elf:COUNT_OF_MARRIAGES]`
:   `[elf:OCCUPATION]`
:   `[elf:POSSESSIONS]`
:   `[elf:RELIGIOUS_AFFILIATION#Individual]`
:   `[elf:RESIDES_AT]`
:   `[elf:SOCIAL_SECURITY_NUMBER]`
:   `[elf:NOBILITY_TYPE_TITLE]`
:   `[elf:ATTRIBUTE_DESCRIPTOR]`

### `elf:IndividualEvent`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

Supertype
:   `[elf:Event]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   `[elf:AGE_AT_EVENT]`

Subtypes
:   `[elf:BIRTH]`
:   `[elf:CHRISTENING]`
:   `[elf:DEATH]`
:   `[elf:BURIAL]`
:   `[elf:CREMATION]`
:   `[elf:ADOPTION]`
:   `[elf:BAPTISM]`
:   `[elf:BAR_MITZVAH]`
:   `[elf:BAS_MITZVAH]`
:   `[elf:BLESSING]`
:   `[elf:ADULT_CHRISTENING]`
:   `[elf:CONFIRMATION]`
:   `[elf:FIRST_COMMUNION]`
:   `[elf:ORDINATION]`
:   `[elf:NATURALIZATION]`
:   `[elf:EMIGRATION]`
:   `[elf:IMMIGRATION]`
:   `[elf:CENSUS#Individual]`
:   `[elf:PROBATE]`
:   `[elf:WILL]`
:   `[elf:GRADUATION]`
:   `[elf:RETIREMENT]`
:   `[elf:EVENT#Individual]`

Payload
:   A *string*, which may be limited by subtypes.

    The special value `Y` indicates an assertion that the event in question did occur,
    even if it has no subordinate date or place.

### `elf:MULTIMEDIA_LINK`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_CITATION]`
:   `[elf:FAM_RECORD]`
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elf:SOURCE_RECORD]`
:   `[elf:SUBMITTER_RECORD]`
:   `[elf:Event]`

Substructures
:   `[elf:MULTIMEDIA_FILE_REFERENCE]` \*
:   `[elf:DESCRIPTIVE_TITLE]` ?
:   `[elf:MULTIMEDIA_FORMAT]` ? -- GEDCOM 5.5

Payload
:   Either a *pointer* to a `[elf:MULTIMEDIA_RECORD]` or none.

    If the payload is a pointer, it *should not* contain substructures.

Default tag
:   `OBJE`

{.ednote} TO DO: review GEDCOM 5.5 to make sure this is right

### `elf:NOTE_STRUCTURE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Record]`
:   `[elf:SOURCE_RECORD_DATA]`
:   `[elf:ASSOCIATION_STRUCTURE]`
:   `[elf:CHILD_TO_FAMILY_LINK]`
:   `[elf:Event]`
:   `[elf:PersonalName]`
:   `[elf:SOURCE_CITATION]`
:   `[elf:SOURCE_REPOSITORY_CITATION]`
:   `[elf:SPOUSE_TO_FAMILY_LINK]`
:   `[elf:CHANGE_DATE]`
:   `[elf:PLACE_STRUCTURE]`

Substructures
:   None

Payload
:   Either a *pointer* to a `[elf:NOTE_RECORD]` or a *block string* of arbitrary length.

Default tag
:   `NOTE`

### `elf:PersonalName`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

Supertype
:   `[elf:Structure]`

Substructures
:   `[elf:NAME_PIECE_PREFIX]` ?
:   `[elf:NAME_PIECE_GIVEN]` ?
:   `[elf:NAME_PIECE_NICKNAME]` ?
:   `[elf:NAME_PIECE_SURNAME_PREFIX]` ?
:   `[elf:NAME_PIECE_SURNAME]` ?
:   `[elf:NAME_PIECE_SUFFIX]` ?
:   `[elf:NOTE_STRUCTURE]` \*
:   `[elf:SOURCE_CITATION]` \*

Subtypes
:   `[elf:PERSONAL_NAME_STRUCTURE]`
:   `[elf:NAME_PHONETIC_VARIATION]`
:   `[elf:NAME_ROMANIZED_VARIATION]`

### `elf:PERSONAL_NAME_STRUCTURE`

Supertype
:   `[elf:PersonalName]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   `[elf:NAME_TYPE]`
:   `[elf:NAME_PHONETIC_VARIATION]`
:   `[elf:NAME_ROMANIZED_VARIATION]`

Payload
:   A *line string* matching the [Language Tag](#language-tag) microformat.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    In the event that this payload disagrees with the substructures of this structure, the payload *should* be taken as more correct.

Default tag
:   `NAME`

### `elf:PLACE_STRUCTURE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`

Substructures
:   `[elf:PLACE_HIERARCHY]` ?
:   `[elf:PLACE_PHONETIC_VARIATION]` \*
:   `[elf:PLACE_ROMANIZED_VARIATION]` \*
:   `[elf:MAP_COORDINATES]` ?
:   `[elf:NOTE_STRUCTURE]` \*


Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A comma-separated list of names of regions, where each element of the list is subsumed within all subsequent elements.
    
    If this structure has a `[elf:PLACE_HIERARCHY]` substructure or there is a default `[elf:PLACE_HIERARCHY]` defined for the dataset, then this payload *should* contain one name for each jurisdictional elements in that `[elf:PLACE_HIERARCHY]`, using empty strings in place of any unknown or non-present elements.

Default tag
:   `PLAC`

{.note} If an individual region name contains a comma, that comma cannot be represented in the place structure format. As there is no escaping mechanism provided, it must either be omitted or replaced with a substitute marking.

### `elf:MAP_COORDINATES`

Contains the location of a place in a global coordinate system.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PLACE_STRUCTURE]`

Substructures
:   `[elf:PLACE_LATITUDE]` !
:   `[elf:PLACE_LONGITUDE]` !


Payload
:   None

Default tag
:   `MAP`

### `elf:PLACE_LONGITUDE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MAP_COORDINATES]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 12 characters.

    Decimal degrees from the prime meridian.
    Either the letter `E` (for east) or `W` (for west), followed (without a space) by a decimal number between 0 and 180.

{.note} Only decimal degrees supported; degree-minute-second representations *must not* appear in this payload.

Default tag
:   `LONG`

### `elf:PLACE_LATITUDE`

Degrees north or south of the equator

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MAP_COORDINATES]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 12 characters.
    
    Decimal degrees from the equator.
    Either the letter `N` (for north) or `S` (for south), followed (without a space) by a decimal number between 0 and 90.

{.note} Only decimal degrees supported; degree-minute-second representations *must not* appear in this payload.

Default tag
:   `LATI`


### `elf:DEFAULT_PLACE_FORMAT`

Contains the default `[elf:PLACE_HIERARCHY]` for the full document stream.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   `[elf:PLACE_HIERARCHY]` ?

Payload
:   None

Default tag
:   `PLAC`

### `elf:PLACE_HIERARCHY`

{.ednote} This feels like a strange way of serializing an ordered map, and thus perhaps better defined as a pseudostructure?

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PLACE_STRUCTURE]`
:   `[elf:DEFAULT_PLACE_FORMAT]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A comma-separated list of names of what the specific components of `[elf:PLACE_STRUCTURE]` represent.

Default tag
:   `FORM`

### `elf:SOURCE_CITATION`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elf:FAM_RECORD]`
:   `[elf:Event]`
:   `[elf:ASSOCIATION_STRUCTURE]`
:   `[elf:PersonalName]`

Substructures
:   `[elf:WHERE_WITHIN_SOURCE]` ?
:   `[elf:EVENT_TYPE_CITED_FROM]` ?
:   `[elf:SOURCE_CITATION_DATA]` ?
:   `[elf:TEXT_FROM_SOURCE]` \*
:   `[elf:NOTE_STRUCTURE]` \*
:   `[elf:MULTIMEDIA_LINK]` \*
:   `[elf:CERTAINTY_ASSESSMENT]` ?

Payload
:   Either a *pointer* to a `[elf:SOURCE_RECORD]` or a *block string* of arbitrary length.

    If the payload is a pointer, then the structure *should not* contain any
    `[elf:TEXT_FROM_SOURCE]` substructures.
    
    If the payload is a block string, then the structure *should not* contain any 
    `[elf:WHERE_WITHIN_SOURCE]`, `[elf:EVENT_TYPE_CITED_FROM]`, or `[elf:SOURCE_CITATION_DATA]` substructures.

    It is *recommended* that only the pointer payload version be created.

Default tag
:   `SOUR`

{.note} The text-payload version has significantly less internal structure than does the pointer version. Also note that the text-payload and pointer-payload versions may both contain `[elf:TEXT_FROM_SOURCE]`, but while the text-payload version has it as a direct substructure, the pointer-payload version has it both through the pointed-to structure and nested inside its `[elf:SOURCE_CITATION_DATA]` substructure.

### `elf:SOURCE_CITATION_DATA`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_CITATION]`

Substructures
:   `[elf:ENTRY_RECORDING_DATE]` ?
:   `[elf:TEXT_FROM_SOURCE]` \*

Payload
:   None

Default tag
:   `DATA`

### `elf:SOURCE_REPOSITORY_CITATION`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD]`

Substructures
:   `[elf:SOURCE_CALL_NUMBER]` \*
:   `[elf:NOTE_STRUCTURE]` \*

Payload
:   Either a *pointer* to a `[elf:REPOSITORY_RECORD]` or none.

    If the payload is none, there *should* be a `[elf:NOTE_STRUCTURE]` describing where the information described by the containing structure can be found.

Default tag
:   `SOUR`

### `elf:SPOUSE_TO_FAMILY_LINK`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   `[elf:NOTE_STRUCTURE]` \*

Payload
:   A *pointer* to a `[elf:FAM_RECORD]`

    It *must* be the case that the pointed-to `[elf:FAM_RECORD]` contains a `[elf:ParentPointer]` pointing to the superstructure of this structure.

Default tag
:   `FAMS`







### `elf:CENSUS#Family`

An inventory of persons or households in a population.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `CENS`

### `elf:EVENT#Family`

A generic event, the type of which which *should* be more fully described in a `[elf:EVENT_OR_FACT_CLASSIFICATION]` substructure.

Supertype
:   `[elf:FamilyEvent]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    A textual description of the specific event; typically more specific than the generic event type classification of the `[elf:EVENT_OR_FACT_CLASSIFICATION]` substructure.
    
    Unlike other `[elf:FamilyEvent]`s, `Y` is not a special value.
    `elf:EVENT#Family` events are always assertions that the event occurred.

Default tag
:   `EVEN`


### `elf:ANNULMENT`

Declaring a marriage to be invalid, as though it had never occurred.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `ANUL`

### `elf:DIVORCE`

The ending of a marriage between still-living individuals.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `DIV`

### `elf:DIVORCE_FILED`

The legal action expressing intent to divorce.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `DIVF`

### `elf:ENGAGEMENT`

The agreement of a couple to enter into a marriage in the future.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `ENGA`

### `elf:MARRIAGE_BANN`

A public notice of an intent to marry.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `MARB`

### `elf:MARRIAGE_CONTRACT`

A formal contractual agreement to marry.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `MARC`

### `elf:MARRIAGE_LICENSE`

Optaining a legal license to marry.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `MARL`

### `elf:MARRIAGE`

The creation of a family unit (via a legal, religious, customary, common-law, or other form of union).

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `MARR`

### `elf:MARRIAGE_SETTLEMENT`

A legal arrangement to modify property rights upon marriage.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `MARS`

### `elf:RESIDENCE`

Residence: either the fact of residing at, or the event of moving to, a particular location.

Supertype
:   `[elf:FamilyEvent]`

Default tag
:   `RESI`





### `elf:CASTE_NAME`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.

    The social, religious, or racial caste tow which an individual belongs.

Default tag
:   `CAST`

### `elf:PHYSICAL_DESCRIPTION`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *block string* of arbitrary length.

    Appearance and/or other physical characteristics.

Default tag
:   `DSCR`

### `elf:SCHOLASTIC_ACHIEVEMENT`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 248 characters.
    
    An educational degree or attainment.

Default tag
:   `EDUC`

### `elf:NATIONAL_ID_NUMBER`

Supertype
:   `[elf:IndividualAttribute]`

Substructures
:   `[elf:EVENT_OR_FACT_CLASSIFICATION]` !

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.
    
    An identifier used by a nation to a particular individual.
    If an appropriate nation-specific alternative is present, it *should* be used.

Default tag
:   `IDNO`

### `elf:NATIONAL_OR_TRIBAL_ORIGIN`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    A group to which a person is associated, typically by birth.

Default tag
:   `NATI`

### `elf:COUNT_OF_CHILDREN#Individual`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string* taking the form of a decimal number.
    It is RECOMMENDED that implementations support payloads of at least 3 characters.
    
    The total number of children this person ever had.
    This does not need to match the number of children individually identified in the dataset.

Default tag
:   `NCHI`


### `elf:COUNT_OF_MARRIAGES`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string* taking the form of a decimal number.
    It is RECOMMENDED that implementations support payloads of at least 3 characters.
    
    The total number of marriages this person ever had.
    This does not need to match the number of marriages individually identified in the dataset.

Default tag
:   `NMR`


### `elf:OCCUPATION`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    What this person does as a livelihood.

Default tag
:   `OCCU`


### `elf:POSSESSIONS`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string* of arbitrary length.

    A list of objects or land owned by the person.

Default tag
:   `PROP`


### `elf:RELIGIOUS_AFFILIATION#Individual`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    The name of a religion with which the person affiliates.

Default tag
:   `RELI`


### `elf:RESIDES_AT`

Indicates that the person resided at the location indicated by the `[elf:ADDRESS]` substructure.

Supertype
:   `[elf:IndividualAttribute]`

Substructures
:   `[elf:ADDRESS]` !

Payload
:   None

Default tag
:   `RESI`


### `elf:SOCIAL_SECURITY_NUMBER`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 11 characters.
    
    A variant of `[elf:NATIONAL_ID_NUMBER]` assigned by the United States of America.

Default tag
:   `SSN`

{.ednote} I have not made this a subtype of IDNO because IDNO has a required TYPE where SSN does not.

### `elf:NOBILITY_TYPE_TITLE`

Supertype
:   `[elf:IndividualAttribute]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A title given a person associated with a local or national notion of nobility or royalty.

Default tag
:   `TITL`


### `elf:ATTRIBUTE_DESCRIPTOR`

A generic attribute, the type of which which *must* be more fully described in a `[elf:EVENT_OR_FACT_CLASSIFICATION]` substructure.

Supertype
:   `[elf:IndividualAttribute]`

Substructures
:   `[elf:EVENT_OR_FACT_CLASSIFICATION]` !

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    A textual description of the specific attribute; typically more specific than the generic attribute type classification of the `[elf:EVENT_OR_FACT_CLASSIFICATION]` substructure.

Default tag
:   `FACT`








### `elf:ADOPTION`

The creation of a parent-child relationship not associated with birth.

Supertype
:   `[elf:IndividualEvent]`

Substructures
:   `[elf:ADOPTIVE_FAMILY]` ?

Default tag
:   `ADOP`

### `elf:BIRTH`

The exiting of the womb.

Supertype
:   `[elf:IndividualEvent]`

Substructures
:   `[elf:WITHIN_FAMILY]` ?

Default tag
:   `BIRT`

### `elf:CHRISTENING`

A religious rite occurring at or near birth.

Supertype
:   `[elf:IndividualEvent]`

Substructures
:   `[elf:WITHIN_FAMILY]` ?
    
Default tag
:   `CHR`

### `elf:DEATH`

The end of life.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `DEAT`

### `elf:BURIAL`

The depositing of the body (in whole or in part) of the deceased.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `BRI`

### `elf:CREMATION`

The burning of the body (in whole or in part) of the deceased.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `CREM`

### `elf:BAPTISM`

Baptism, a common Christian rite, typically involving water and indicating entry into a particular faith or denomination, performed at different ages in different denominations.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `BAPM`

### `elf:BAR_MITZVAH`

Bar Mitzvah, a Jewish rite (typically for 13-year-old boys).

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `BARM`

### `elf:BAS_MITZVAH`

Bas Mitzvah, a Jewish rite (typically for 13-year-old girls).

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `BASM`

### `elf:BLESSING`

A religious rite invoking divine favor on an individual.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `BLES`

### `elf:ADULT_CHRISTENING`

Adult christening, a religious rite in some Christian denominations typically performed when converting to the religion.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `CHRA`

### `elf:CONFIRMATION`

Confirmation, a religious rite in some Christian denominations associated with gaining full fellowship in the religion and/or receiving the Holy Ghost.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `CONF`

### `elf:FIRST_COMMUNION`

First communion, a religious rite in many Christian denominations associated with first partaking of the communion of the Lord's Supper.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `FCOM`

### `elf:ORDINATION`

The bestowal of religious authority or office.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `ORDN`

### `elf:NATURALIZATION`

{.ednote} GEDCOM used American spelling; should we change it to British?

The gaining of citizenship in a new nation or land.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `NATU`

### `elf:EMIGRATION`

The departure from the nation or land in which one has nativity or citizenship.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `EMIG`

### `elf:IMMIGRATION`

The entering of a nation or land in which one does not have nativity or citizenship.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `IMMI`

### `elf:CENSUS#Individual`

An inventory of persons or households in a population.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `CENS`

### `elf:PROBATE`

The judicial actions associated with the disposition of the estate of the deceased.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `PROB`

### `elf:WILL`

The creation of a legal document regarding the disposition of a person's estate upon death.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `WILL`

### `elf:GRADUATION`

The conclusion of formal education.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `GRAD`

### `elf:RETIREMENT`

The cessation of gainful employment, typically because sufficient wealth has been accumulated to no longer necessitate such.

Supertype
:   `[elf:IndividualEvent]`

Default tag
:   `RETI`

### `elf:EVENT#Individual`

A generic event, the type of which which *should* be more fully described in a `[elf:EVENT_OR_FACT_CLASSIFICATION]` substructure.

Supertype
:   `[elf:IndividualEvent]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    A textual description of the specific event; typically more specific than the generic event type classification of the `[elf:EVENT_OR_FACT_CLASSIFICATION]` substructure.
    
    Unlike other `[elf:FamilyEvent]`s, `Y` is not a special value.
    `elf:EVENT#Family` events are always assertions that the event occurred.

{.ednote} GEDCOM does not list this payload for individual EVEN, only family EVEN, but other text in GEDCOM suggests that this was an oversight, not an intentional omission.

Default tag
:   `EVEN`








### `elf:ADDRESS_CITY`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADDRESS]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    The city, town, or similar name in an address.

Default tag
:   `CITY`

### `elf:ADDRESS_COUNTRY`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADDRESS]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    The nation, country, or similar name in an address.

Default tag
:   `CTRY`

### `elf:ADDRESS_LINE1`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADDRESS]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    The first line of the address, preceding the city.

Default tag
:   `ADR1`


### `elf:ADDRESS_LINE2`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADDRESS]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    The second line of the address, preceding the city.

Default tag
:   `ADR2`


### `elf:ADDRESS_LINE3`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADDRESS]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    The third line of the address, preceding the city.

Default tag
:   `ADR3`


### `elf:ADDRESS_POSTAL_CODE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADDRESS]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 10 characters.
    
    The postal code of this address, as defined and used by the postal system in the area.

Default tag
:   `POST`

### `elf:ADDRESS_STATE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADDRESS]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    The state, province, or similar name in the area.

Default tag
:   `STAE`



### `elf:ADOPTIVE_FAMILY`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADOPTION]`

Substructures
:   `[elf:ADOPTED_BY_WHICH_PARENT]`

Payload
:   A pointer to a `[elf:FAM_RECORD]`.

    The pointed-to record describes the family unit into which the individual was adopted.

Default tag
:   `FAMC`


### `elf:ADOPTED_BY_WHICH_PARENT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ADOPTIVE_FAMILY]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 4 characters.
    
    Known values include {`HUSB`, `WIFE`, `BOTH`}.
    `HUSB` means the adoption was to the individual indicated by the `[elf:PARENT1_POINTER]` of the `[elf:FAM_RECORD]` pointed to by the payload of the containing superstructure;
    `WIFE` means the adoption was to the individual indicated by the `[elf:PARENT2_POINTER]`pointed to by the payload of the containing superstructure;
    and `BOTH` means both of those individuals were part of the adoption.

Default tag
:   `ADOP`


### `elf:WITHIN_FAMILY`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:BIRTH]`
:   `[elf:CHRISTENING]`

Payload
:   A pointer to a `[elf:FAM_RECORD]`.

    The pointed-to record describes the family unit associated with the individual event described by the superstructure.

Default tag
:   `FAMC`

### `elf:AGE_AT_EVENT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:IndividualEvent]`
:   `[elf:Parent1Age]`
:   `[elf:Parent2Age]`

Payload
:   A *line string* matching the [Age](#age) microformat.

Default tag
:   `AGE`

### `elf:CAUSE_OF_EVENT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    Introduced to record the cause of death as a substructure to a `[elf:DEATH]` structure, but permitted under any event in case a cause of the event is known.

Default tag
:   `CAUS`

### `elf:CERTAINTY_ASSESSMENT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_CITATION]`

Payload
:   A *line string* containing a numeric value.
    Known values include {`0`, `1`, `2`, and `3`}.
    
    A ranking hint for display: if two `[elf:SOURCE_CITATION]`s contain `elf:CERTAINTY_ASSESSMENT`s with different payloads, the numerically larger `elf:CERTAINTY_ASSESSMENT` may be displayed as being inside a more reliable `[elf:SOURCE_CITATION]` than is the numerically smaller payload.
    
    GEDCOM defined the four specific values as having the following meanings
    
    > 0 = Unreliable evidence or estimated data
    >
    > 1 = Questionable reliability of evidence (interviews, census, oral genealogies, or potential for bias for example, an autobiography)
    >
    > 2 = Secondary evidence, data officially recorded sometime after event
    >
    > 3 = Direct and primary evidence used, or by dominance of the evidence
    
{.note} It is unclear that GEDCOM's four categories have the relative reliability their ordering suggests, nor that `elf:CERTAINTY_ASSESSMENT`s in extant files contain meaningful information. It is not difficult to find example GEDCOM where all `[elf:SOURCE_CITATION]`s have a `elf:CERTAINTY_ASSESSMENT` with payload `3` even when some clearly cite sources providing secondary evidence of the facts containing the citation. 

Default tag
:   `QUAY`

### `elf:COUNT_OF_CHILDREN#Family`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A *line string* taking the form of a decimal number.
    It is RECOMMENDED that implementations support payloads of at least 3 characters.
    
    The total number of children this family unit had,
    either at some (unspecified) point in time or in total its entire existence.
    This does not need to match the number of children identified through `[elf:CHILD_POINTER]` substructures of the containing superstructure.

Default tag
:   `NCHI`

{.ednote} It seems odd to me that `elf:COUNT_OF_CHILDREN#Family` is not a `elf:FamilyEvent` (or `elf:FamilyAttribute`, though no such supertype currently exists) as surely the number of children of a family would need sourcing and an as-of date? Should we leave it as a stand-alone structure, or boost it to event status?

### `elf:DATE_VALUE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`

Payload
:   A *line string* matching the [Date Value](#date-value) microformat.
    
    Indicates when the event or attribute described by the containing structure occurred or was witnessed.
    
Default tag
:   `DATE`

### `elf:DATE_PERIOD`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:EVENTS_RECORDED]`

Payload
:   A *line string* matching the [Date Period](#date-period) microformat.
    
    Indicates the period during which the source recorded events.
    
Default tag
:   `DATE`

### `elf:ENTRY_RECORDING_DATE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_CITATION_DATA]`

Payload
:   A *line string* matching the [Date Value](#date-value) microformat.
    
    Indicates when the portion of the source being cited was entered into the source.
    
Default tag
:   `DATE`

### `elf:EVENT_OR_FACT_CLASSIFICATION`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    A classification for the superstructure's category, more precise than its type alone provides but generic enough to be anticipated to be re-used.
    
Default tag
:   `TYPE`

### `elf:EVENT_TYPE_CITED_FROM`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_CITATION]`

Substructures
:   `[elf:ROLE_IN_EVENT]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 15 characters.
    
    Known values include 
    {`CAST`, `EDUC`, `NATI`, `OCCU`, `PROP`, `RELI`, `RESI`, `TITL`, `FACT`,
    `ANUL`, `CENS`, `DIV`, `DIVF`, `ENGA`, `MARR`, `MARB`, `MARC`, `MARL`, `MARS`,
    `ADOP`, `BIRT`, `BAPM`, `BARM`, `BASM`, `BLES`, `BURI`, `CENS`, `CHR`,
    `CHRA`, `CONF`, `CREM`, `DEAT`, `EMIG`, `FCOM`, `GRAD`, `IMMI`, `NATU`,
    `ORDN`, `RETI`, `PROB`, `WILL`,
    `EVEN`}.
    Indicates that the cited source was created to document the event or attribute
    described by the subtype of `elf:Event` whose default tag is the provided value.

{.example ...}
A marriage certificate may document the spouses' birth dates;
however, it's `elf:EVENT_TYPE_CITED_FROM`'s payload should be `MARR`, not `BIRT`. 
{/}

Default tag
:   `EVEN`

### `elf:EVENTS_RECORDED`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD_DATA]`

Substructures
:   `[elf:DATE_PERIOD]`
:   `[elf:SOURCE_JURISDICTION_PLACE]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    A comma-separated list of valid payload values of `[elf:EVENT_TYPE_CITED_FROM]`.
    Indicates that the source includes documentation of these events or attributes.

Default tag
:   `EVEN`

### `elf:FILE_NAME`

{.ednote} What is the purpose of this structure? Clearly it cannot always match the name of the physical file, which can be renamed without editing; nor are there any limitations given on it in GEDCOM besides that it include an extension *if* the file containing it has an extension in its name. Without knowing its purpose, I don't know how to document this structure.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    The base name (i.e., not a full path) of a file.

Default tag
:   `EVEN`

### `elf:GEDCOM_CONTENT_DESCRIPTION`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   None

Payload
:   A *block string* of arbitrary length.
    
    A description of the intended scope of the contents of the dataset.

Default tag
:   `NOTE`

### `elf:GEDCOM_FORM`

{.ednote} GEDCOM_FORM seems like a pseudostructure?

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:GEDCOM_FORMAT]`

Substructures
:   None

Payload
:   The exact string `LINEAGE-LINKED`.

Default tag
:   `FORM`

### `elf:GEDCOM_FORMAT`

{.ednote} GEDCOM_FORMAT seems like a pseudostructure?

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   `[elf:GEDCOM_FORM]`
:   `[elf:VERSION_NUMBER]`

Payload
:   None

Default tag
:   `GEDC`

### `elf:LANGUAGE_OF_TEXT`

{.ednote} Should this really be a pseudostructure? If we re-work this as having language-tagged strings as payloads, then it is; but if we leave the strings in this document as non-language-tagged then it is data instead.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   None

Payload
:   A *line string* matching the [Language Tag](#language-tag) microformat.
    
    Indicates the default language of the free-text payloads in the dataset.

Default tag
:   `LANG`


### `elf:LANGUAGE_PREFERENCE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SUBMITTER_RECORD]`

Substructures
:   None

Payload
:   A *line string* matching the [Language Tag](#language-tag) microformat.
    
    Indicates a language in which the person described by the superstructure prefers to communicate.

Default tag
:   `LANG`

### `elf:NAME_OF_BUSINESS`

Supertype
:   `[elf:Agent]`

Superstructures
:   `[elf:DOCUMENT_SOURCE]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    The name of the entity that produced the product described by the superstructure.

Default tag
:   `CORP`

### `elf:NAME_OF_PRODUCT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:DOCUMENT_SOURCE]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    The name of the product described by the superstructure.

Default tag
:   `NAME`

### `elf:NAME_OF_REPOSITORY`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:REPOSITORY_RECORD]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    The name of the repository described by the superstructure.

Default tag
:   `NAME`

### `elf:NAME_OF_SOURCE_DATA`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:DOCUMENT_SOURCE]`

Substructures
:   `[elf:PUBLICATION_DATE]`
:   `[elf:COPYRIGHT_SOURCE_DATA]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    The name of an electronic data source from which this dataset was extracted.

Default tag
:   `DATA`

### `elf:NAME_PHONETIC_VARIATION`

Supertype
:   `[elf:PersonalName]`

Superstructures
:   `[elf:PERSONAL_NAME_STRUCTURE]`

Substructures
:   `[elf:PHONETIC_TYPE]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    Contains a phonetic presentation of the same name as its superstructure.

Default tag
:   `FONE`

### `elf:PHONETIC_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:NAME_PHONETIC_VARIATION]`
:   `[elf:PLACE_PHONETIC_VARIATION]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.
    
    Identifies the phonetic scheme used in the superstructure.
    Known values include {`hangul`, `kana`}.

{.ednote} Should we add others, like `ipa`?

Default tag
:   `TYPE`

### `elf:NAME_ROMANIZED_VARIATION`

Supertype
:   `[elf:PersonalName]`

Superstructures
:   `[elf:PERSONAL_NAME_STRUCTURE]`

Substructures
:   `[elf:ROMANIZED_TYPE]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    Contains the same name as its superstructure, but presented using ASCII letters.

Default tag
:   `ROMN`

### `elf:ROMANIZED_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:NAME_ROMANIZED_VARIATION]`
:   `[elf:PLACE_ROMANIZED_VARIATION]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.
    
    Identifies the romanization scheme used in the superstructure.
    Known values include {`pinyin`, `romanji`, `wadegiles`}.

Default tag
:   `TYPE`

### `elf:NAME_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PERSONAL_NAME_STRUCTURE]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.
    
    The kind of name the superstructure contains.
    Known values include {`aka`, `birth`, `immigration`, `maiden`, `married`}; additional values are encouraged as appropriate.
    
    | known value | meaning                                 |
    |:------------|:----------------------------------------|
    | aka         | also known as: an unofficial pseudonym  |
    | birth       | name given at or near birth             |
    | immigrant   | name assumed when immigrating           |
    | maiden      | name used prior to marriage             |
    | married     | name assumed at marriage                |
    
{.note} GEDCOM's definition of the `married` payload was "name was persons previous married name," suggestion it was only to be used after the married name was no longer used; this nuanced definition does not appear to have been used in practice.

Default tag
:   `TYPE`



### `elf:PLACE_PHONETIC_VARIATION`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PLACE_STRUCTURE]`

Substructures
:   `[elf:PHONETIC_TYPE]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    Contains a phonetic presentation of the same place, in the same format, as its superstructure.

Default tag
:   `FONE`

### `elf:PLACE_ROMANIZED_VARIATION`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PLACE_STRUCTURE]`

Substructures
:   `[elf:ROMANIZED_TYPE]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    Contains an ASCII letter presentation of the same place, in the same format, as its superstructure.

Default tag
:   `ROMN`

### `elf:PUBLICATION_DATE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:NAME_OF_SOURCE_DATA]`

Substructures
:   None

Payload
:   A *line string* matching the [Exact Date](#exact-date) microformat.
        
    Contains the date the source dataset (described by the superstructure) was published or created.

Default tag
:   `DATE`


### `elf:COPYRIGHT_SOURCE_DATA`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:NAME_OF_SOURCE_DATA]`

Substructures
:   None

Payload
:   A *block string* of arbitrary length.
    
    Contains a copyright statement for the source dataset described by the superstructure.

Default tag
:   `COPR`

### `elf:COPYRIGHT_GEDCOM_FILE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    Contains a copyright statement for the entire dataset.

Default tag
:   `COPR`

### `elf:RELATION_IS_DESCRIPTOR`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:ASSOCIATION_STRUCTURE]`

Substructures
:   None

Payload
:   A *line string* of arbitrary length.
    It is RECOMMENDED that implementations support payloads of at least 25 characters.
    
    Describes the nature of the association described by the superstructure.
    This is a directed relationship.
    If the payload text is *X*,
    the person described by the record pointed to by the payload of the superstructure is *Y*, and 
    the person described by the superstructure of the superstructure is *Z*
    then this payload means "*Y* is *X*'s *Z*".

{.example ...}
The following ELF fragment records that Galahad was employed by Arthur:

````gedcom
0 @arthur@ INDI
1 NAME Arthur //
1 ASSO @galahad@
2 RELA employee
0 @galahad@ INDI
1 NAME Galahad //
1 ASSO @arthur@
2 RELA employer
````
{/}

Default tag
:   `RELA`

### `elf:RELIGIOUS_AFFILIATION`

Supertype
:   `[elf:Structure]`

Superstructure
:   `[elf:Event]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 90 characters.
    
    The name of a religion with which the event was affiliated.

Default tag
:   `RELI`

### `elf:RESPONSIBLE_AGENCY`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD_DATA]`
:   `[elf:Event]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    The group or entity that was responsible for this event or data.

Default tag
:   `ROMN`

### `elf:RESTRICTION_NOTICE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elf:FAM_RECORD]`
:   `[elf:Event]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 12 characters.
    
    Specifies how the superstructure should be treated.
    Known values and their meaning are listed in the following table:
    
    Known value     Meaning
    --------------  ------------------------------------------------------------
    confidential    should not be distributed or exported
    locked          should not be edited
    privacy         has had information omitted to maintain confidentiality
    
Default tag
:   `RESN`

### `elf:ROLE_IN_EVENT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:EVENT_TYPE_CITED_FROM]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 25 characters.
    
    When contained within an `[elf:INDIVIDUAL_RECORD]`, indicates what role that individual played in the event described by this structure's superstructure.
    It has no defined meaning (and thus *should not* be used) outside of that context.
    
    Known values and their meanings are listed in the following table.
    It is expected that additional values are also used when these are insufficient:
    
    Known value     Role the individual played in the event
    -------------   ------------------------------------------------------------
    CHIL            child
    FATH            father
    HUSB            husband
    MOTH            mother
    SPOU            spouse
    WIFE            wife
    

Default tag
:   `ROLE`

### `elf:SEX_VALUE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 7 characters.
    
    The sex of the individual.
    Known values and their meanings are listed in the following table:
    
    Known value     Sex
    -------------   ------------------------------------------------------------
    F               female
    M               male
    U               not knowable from available records
    

{.note} GEDCOM was silent on if this was to be interpreted as biological sex or gender identity, and it is likely that data exists with both intended meanings.

{.ednote} A revision of or extension to this structure has been discussed by FHISO and is anticipated in a future release of this standard.


Default tag
:   `ROLE`

### `elf:SOURCE_CALL_NUMBER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_REPOSITORY_CITATION]`

Substructures
:   `[elf:SOURCE_MEDIA_TYPE]` ?

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    An identifier used by the repository to refer to the cited source.
    
Default tag
:   `CALN`

### `elf:SOURCE_DESCRIPTIVE_TITLE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD]`

Substructures
:   None

Payload
:   A *block string* of arbitrary length.
    
    A description of the source defined by the superstructure;
    for example, 
    a periodical article's `elf:SOURCE_DESCRIPTIVE_TITLE` might include the title of the article and the title of the periodical;
    a family bible's `elf:SOURCE_DESCRIPTIVE_TITLE` might include a list of past and present owners and the book's dimensions and appearance.

{.note} Although this tag is called a "title", it is not (just) the title of the work in the usual sense of the word.

Default tag
:   `TITL`

### `elf:SOURCE_FILED_BY_ENTRY`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.
    
    A short title for this source.
    Intended to be used for sorting and filing.
    
Default tag
:   `ABBR`

### `elf:SOURCE_JURISDICTION_PLACE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:EVENTS_RECORDED]`

Substructure
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A comma-separated list of names of regions, where each element of the list is subsumed within all subsequent elements.
    An assertion that all events recorded in this source
    would have a `[elf:PLACE_STRUCTURE]` payload ending with this payload.

{.note ...} While similar to the format of a `[elf:PLACE_STRUCTURE]` payload, this differs in a few key ways:

- it must use the default `[elf:DEFAULT_PLACE_FORMAT]` as it has no `[elf:PLACE_HIERARCHY]` substructure.
- it may (and often does) omit the first several elements of the list. Unlike a `[elf:PLACE_STRUCTURE]`, the omitted parts are not represented by empty strings, but by removal of their entire entry.
    - a `[elf:PLACE_STRUCTURE]` for an unknown location in Nevada would be "`, , Nevada, USA`"
    - a `[elf:SOURCE_JURISDICTION_PLACE]` the entirety of Nevada "`Nevada, USA`"
{/}

Default tag
:   `PLAC`

### `elf:SOURCE_MEDIA_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_FORMAT]`
:   `[elf:SOURCE_CALL_NUMBER]`

Substructure
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 15 characters.

    The medium of the source.
    Known values include {`audio`, `book`, `card`, `electronic`, `fiche`, `film`, `magazine`, `manuscript`, `map`, `newspaper`, `photo`, `tombstone`, `video`}

Default tag
:   `MEDI`

### `elf:SOURCE_ORIGINATOR`

{.ednote} GEDCOM has this singular (0 or 1 per source record) and describes it listing only one creator. Should we change it to multiple, or de-describe it as listing all creators?

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD]`

Substructure
:   None

Payload
:   A *block string* of arbitrary length.

    The name of the primary creator of the source.

Default tag
:   `AUTH`

### `elf:SOURCE_PUBLICATION_FACTS`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD]`

Substructure
:   None

Payload
:   A *block string* of arbitrary length.

    Full publication information for the source: when, where, and by whom it was created..

Default tag
:   `PUBL`

### `elf:SUBMITTER_NAME`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SUBMITTER_RECORD]`

Substructure
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 60 characters.

    The name of the submitter, formatted as it should be displayed.

Default tag
:   `NAME`

### `elf:TEXT_FROM_SOURCE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_RECORD]`
:   `[elf:SOURCE_CITATION]`
:   `[elf:SOURCE_CITATION_DATA]`

Substructure
:   None

Payload
:   A *block string* of arbitrary length.

    An excerpt of contents of the source.

Default tag
:   `TEXT`

### `elf:TRANSMISSION_DATE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:HEADER]`

Substructure
:   `[elf:TIME_VALUE]`

Payload
:   A string matching the [Exact Date](#exact-date) syntax.

    The date that this dataset was created.

Default tag
:   `DATE`

### `elf:VERSION_NUMBER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:DOCUMENT_SOURCE]`
:   `[elf:GEDCOM_FORMAT]`

Substructure
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 15 characters.

    A version identifier, with syntax and semantics varying by context.

Default tag
:   `VERS`

### `elf:WHERE_WITHIN_SOURCE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_CITATION]`

Substructures
:   None

Payload
:   A *block string* of arbitrary length.

    Location information expressing what part of the cited source is being cited.

Default tag
:   `PAGE`

### `elf:NAME_PIECE_GIVEN`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PersonalName]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A comma-separated list of given or earned named.

Default tag
:   `GIVN`

### `elf:NAME_PIECE_NICKNAME`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PersonalName]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.

    A comma-separated list of familiar or informal names.

Default tag
:   `NICK`

### `elf:NAME_PIECE_PREFIX`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PersonalName]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.

    A comma-separated list of non-name elements traditionally placed before the proper name, such as titles.

Default tag
:   `NPFX`

### `elf:NAME_PIECE_SUFFIX`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PersonalName]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 30 characters.

    A comma-separated list of non-name elements traditionally placed after the proper name, such as generational marks and ordinals.

Default tag
:   `NSFX`

### `elf:NAME_PIECE_SURNAME`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PersonalName]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A comma-separated list of surnames and family names.

Default tag
:   `SURN`

### `elf:NAME_PIECE_SURNAME_PREFIX`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:PersonalName]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A comma-separated list of non-name elements traditionally attached to and placed before a surname of family name, such as prepositions.

Default tag
:   `SPFX`


### `elfm:HEADER`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:DOCUMENT_SOURCE]` !
:   `[elf:RECEIVING_SYSTEM_NAME]` ?
:   `[elf:TRANSMISSION_DATE]` ?
:   `[elf:SUBMITTER_POINTER]` !
:   `[elf:FILE_NAME]` ?
:   `[elf:COPYRIGHT_GEDCOM_FILE]` ?
:   `[elf:GEDCOM_FORMAT]` !
:   `[elf:LANGUAGE_OF_TEXT]` ?
:   `[elf:DEFAULT_PLACE_FORMAT]` ?
:   `[elf:GEDCOM_CONTENT_DESCRIPTION]` ?

Default tag
:   `HEAD`

### `elf:DOCUMENT_SOURCE`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   `[elf:VERSION_NUMBER]`
:   `[elf:NAME_OF_PRODUCT]`
:   `[elf:NAME_OF_BUSINESS]`
:   `[elf:NAME_OF_SOURCE_DATA]`

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 20 characters.

    In early GEDCOM, this was a unique string assigned to each product through a registration process.
    That process no longer exists.

{.ednote} Do we want to make a new recommendation for the contents of this payload? Perhaps an IRI + date pair? A UUID? A generic "UNREGISTERED_PRODUCT" string or the like?

Default tag
:   `SOUR`

### `elf:RECEIVING_SYSTEM_NAME`

Superstructures
:   `[elfm:HEADER]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 20 characters.

    Identifies the intended recipient software of this dataset.

Default tag
:   `DEST`

### `elf:BINARY_OBJECT`

Binary object was in GEDCOM 5.5 but removed from GEDCOM 5.5.1. Implementations *should* be able to parse them, but *should not* generate new binary objects.

{.ednote} The definition of the base-64 encoding used the terminology "byte" when GEDCOM had elsewhere defined its stream as consisting of characters, not bytes. It is unclear to me if it is possible to follow the spec for an encoding that does not permit byte 0xFF as a single character.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]`

Substructures
:   None

Payload
:   A *block string* containing
    two or more lines of base-64 encoded data, in the custom format described below.

    The first line of of a blob is always empty.
    Each subsequent line is between 4 and 72 characters long, encoded in a base-64 format that differs from other base-64 encodings in two ways.

    First, it uses byte 0xFF as padding instead of the more common U+003D (EQUALS SIGN `=`)
    (how to represent the padding when byte 0xFF is not a legal character in the encoding is not defined by this specification).

    Second, it maps six-bit values to code points as follows:

    | Byte range | Code point mapping |
    |------------|--------------------|
    | 0x00--0x0B | byte + 0x2E        |
    | 0x0C--0x25 | byte + 0x35        |
    | 0x25--0x3F | byte + 0x3B        |
    
    See also the discussion under `[elf:CONTINUED_BINARY_OBJECT]` for how multiple `elf:BINARY_OBJECT` payloads are combined to represent large binary values.

Default tag
:   `BLOB`

### `elf:CONTINUED_BINARY_OBJECT`

Binary object was in GEDCOM 5.5 but removed from GEDCOM 5.5.1. Implementations *should* be able to parse them, but *should not* generate new binary objects.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]`

Substructures
:   None

Payload
:   A pointer to a `[elf:MULTIMEDIA_RECORD]`.

    Used to split `elf:BINARY_OBJECT`s across multiple records.
    Prior to decoding, the payloads of all `elf:BINARY_OBJECT` in the superstructure
    should be concatentated in the order in which they appear,
    and then concatenated with the `elf:BINARY_OBJECT`s in the pointed-to record
    and those pointed to by its `elf:CONTINUED_BINARY_OBJECT`, recursively.

Default tag
:   `OBJE`



<!--
egrep -o '`\[[^`]*\]`' iri-data-model.md  | sort -u | while read link; do m=${link:2}; m=${m%]\`}; grep -q '`'$m'`' iri-data-model.md || echo $m; done

elfm:Document
elf:Structure
-->
