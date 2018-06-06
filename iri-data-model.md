{.ednote ...} This is an early WIP complete re-write of `data-model.md`. Goals:

- Avoid quoting GEDCOM, and where necessary attribute the quote.
- Use IRI = meaning instead of tag-hierarchy = meaning.
{/}

### `elf:Record`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

`elf:Record` types always contain cross-reference IDs.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:USER_REFERENCE_NUMBER]` ?
:   `[elf:AUTOMATED_RECORD_ID]` ?
:   `[elf:CHANGE_DATE]` ?
:   `[elf:NOTE_STRUCTURE]` \*
:   `[elf:SUBMITTER_POINTER]` ?
:   `[elf:SOURCE_CITATION]` \*

Payload
:   None

Subtypes
:   `[elf:FAM_RECORD]`
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elf:MULTIMEDIA_RECORD]`
:   `[elf:NOTE_RECORD]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SOURCE_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

#### `elf:SUBMITTER_POINTER`

{.ednote} GEDCOM limits these to at most one per HEAD, FAM, and INDI. This seems odd; surely a source, note, etc., can also have a submitter, and there can be more than one contributing submitter per record...

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Record]`

Payload
:   A pointer to an `[elf:SUBMITTER_RECORD]`

    Indicates that the pointed-to `[elf:SUBMITTER_RECORD]` describes a contributor of information to the containing structure.

Default tag
:   `SUBM`

#### `elf:USER_REFERENCE_NUMBER`

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


#### `elf:USER_REFERENCE_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:USER_REFERENCE_TYPE]`

Payload
:   A *line string*. It is RECOMMENDED that implementations support payloads of at least 40 characters.

    A user-defined definition of the superstructure.

Default tag
:   `TYPE`

#### `elf:AUTOMATED_RECORD_ID`

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
:   `[elf:FAMILY_EVENT_STRUCTURE]` \*
:   `[elf:FIRST_PARENT_POINTER]` ?
:   `[elf:SECOND_PARENT_POINTER]` ?
:   `[elf:CHILD_POINTER]` \*
:   `[elf:COUNT_OF_CHILDREN]` ?
:   `[elf:MULTIMEDIA_LINK]` \*

Payload
:   None

Default tag
:   `FAM`

#### `elf:FIRST_PARENT_POINTER`

A pointer to the spouse or parent traditionally presented on the left in a vertical family tree or on the top in a horizontal family tree. In a heterosexual pair union, this is traditionally the husband or father.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`

Default tag
:   `HSUB`


#### `elf:SECOND_PARENT_POINTER`

A pointer to the spouse or parent traditionally presented on the right in a vertical family tree or on the bottom in a horizontal family tree. In heterosexual pair unions, this is traditionally the wife or mother.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`

Default tag
:   `WIFE`

#### `elf:CHILD_POINTER`

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
:   `[elf:INDIVIDUAL_EVENT_STRUCTURE]` \*
:   `[elf:INDIVIDUAL_ATTRIBUTE_STRUCTURE]` \*
:   `[elf:CHILD_TO_FAMILY_LINK]` \*
:   `[elf:SPOUSE_TO_FAMILY_LINK]` \*
:   `[elf:ASSOCIATION_STRUCTURE]` \*
:   `[elf:ALIAS_POINTER]` \*
:   `[elf:ANCESTOR_INTEREST_POINTER]` \*
:   `[elf:DESCENDANT_INTEREST_POINTER]` \*
:   `[elf:MULTIMEDIA_LINK]` \*

Payload
:   None

Default tag
:   `INDI`


#### `elf:ALIAS_POINTER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`
    
    Points to a different `[elf:INDIVIDUAL_RECORD]` that may describe the same historical individual as the superstructure.

Default tag
:   `ALIA`

#### `elf:ANCESTOR_INTEREST_POINTER`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Payload
:   A pointer to an `[elf:SUBMITTER_RECORD]`

    Indicates that the pointed-to `[elf:SUBMITTER_RECORD]` describes someone interested in the ancestors of the individual described by the superstructure.

Default tag
:   `ANCI`


#### `elf:DESCENDANT_INTEREST_POINTER`

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
:   `[elf:MULTIMEDIA_FILE_REFN]` \* -- GEDCOM 5.5.1
:   `[elf:MULTIMEDIA_FORMAT]` ! -- GEDCOM 5.5
:   `[elf:DESCRIPTIVE_TITLE]` ? -- GEDCOM 5.5
:   `[elf:BLOB_CONTINUATION]` ? -- GEDCOM 5.5
:   `[elf:BLOB]` ! -- GEDCOM 5.5

Payload
:   None

Default tag
:   `OBJE`


{.ednote} TO DO: document blob and continuation

#### `elf:MULTIMEDIA_FILE_REFN`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]`

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


#### `elf:MULTIMEDIA_FORMAT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]` -- GEDCOM 5.5
:   `[elf:MULTIMEDIA_FILE_REFN]` -- GEDCOM 5.5.1

Substructures
:   None

Payload
:   A *line string*. Known values include {`bmp`, `gif`, `jpg`, `ole`, `pcx`, `tif`, `wav`}.

    Indicates the format of the multimedia data associated with the superstructure of the `elf:MULTIMEDIA_FORMAT` structure.

{.ednote} Should we expand this to allow MIME-type as well as the seven known Windows-style file endings?

Default tag
:   `FORM`

#### `elf:DESCRIPTIVE_TITLE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:MULTIMEDIA_RECORD]` -- GEDCOM 5.5
:   `[elf:MULTIMEDIA_FILE_REFN]` -- GEDCOM 5.5.1

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

Substructures
:   Although it is an `[elf:Record]`, an `elf:NOTE_RECORD` *should not* include an `[elf:NOTE_STRUCTURE]` substructure.

Payload
:   A *block string* of arbitrary length.

Default tag
:   `NOTE`

### `elf:REPOSITORY_RECORD`

Supertype
:   `[elf:Record]`

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:NAME_OF_REPOSITORY]` !
:   `[elf:ADDRESS_STRUCTURE]` ?
:   Although it is an `[elf:Record]`, an `elf:REPOSITORY_RECORD` *should not* include an `[elf:SOURCE_CITATION]` substructure.

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
:   `[elf:SOURCE_DATA]` ?
:   `[elf:SOURCE_ORIGINATOR]` ?
:   `[elf:SOURCE_DESCRIPTIVE_TITLE]` ?
:   `[elf:SOURCE_FILED_BY_ENTRY]` ?
:   `[elf:SOURCE_PUBLICATION_FACTS]` \*
:   `[elf:TEXT_FROM_SOURCE]` ?
:   `[elf:SOURCE_REPOSITORY_CITATION]` \*
:   `[elf:MULTIMEDIA_LINK]` \*
:   Although it is an `[elf:Record]`, an `elf:SOURCE_RECORD` *should not* include an `[elf:SOURCE_CITATION]` substructure.

Payload
:   None

Default tag
:   `SOUR`

#### `elf:SOURCE_DATA`

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

Superstructures
:   `[elfm:Document]`

Substructures
:   `[elf:SUBMITTER_NAME]` !
:   `[elf:ADDRESS_STRUCTURE]` ?
:   `[elf:MULTIMEDIA_LINK]` \*
:   `[elf:LANGUAGE_PREFERENCE]` \*

Payload
:   None

{.note} GEDCOM required that an `[elf:USER_REFERENCE_NUMBER]` in an `elf:SUBMITTER_RECORD` be preregistered with Ancestral File, a service that is no longer available. It is *recommended* that an `[elf:USER_REFERENCE_NUMBER]` not be included in an `elf:SUBMITTER_RECORD`.

Default tag
:   `SUBM`


### `elf:ADDRESS_STRUCTURE`

{.ednote} What to do with this is under discussion on the technical-work mailing list

#### `elf:ADDRESS_LINE`

{.ednote} Might become a subtype of or substructure of elf:ADDRESS_STRUCTURE depending on outcome of technical-work email thread

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`
:   `[elf:NAME_OF_BUSINESS]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

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

#### `elf:PHONE_NUMBER`

{.ednote} Might become a subtype of or substructure of elf:ADDRESS_STRUCTURE depending on outcome of technical-work email thread

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`
:   `[elf:NAME_OF_BUSINESS]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

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

#### `elf:ADDRESS_EMAIL`

{.ednote} Might become a subtype of or substructure of elf:ADDRESS_STRUCTURE depending on outcome of technical-work email thread

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`
:   `[elf:NAME_OF_BUSINESS]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

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

#### `elf:ADDRESS_FAX`

{.ednote} Might become a subtype of or substructure of elf:ADDRESS_STRUCTURE depending on outcome of technical-work email thread

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`
:   `[elf:NAME_OF_BUSINESS]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

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

#### `elf:ADDRESS_WEB_PAGE`

{.ednote} Might become a subtype of or substructure of elf:ADDRESS_STRUCTURE depending on outcome of technical-work email thread

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Event]`
:   `[elf:NAME_OF_BUSINESS]`
:   `[elf:REPOSITORY_RECORD]`
:   `[elf:SUBMITTER_RECORD]`

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

#### `elf:CHANGE_DATE_DATE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Record]`

Substructures
:   `[elf:TIME_VALUE]` ?

Payload
:   A string matching the [Exact Date](#exact-date) syntax.
    
    Indicates the last change to the containing structure.

Default tag
:   `DATE`

#### `elf:TIME_VALUE`

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
:   A *pointer* to a `[elf:FAMILY_RECORD]`

    It *must* be the case that the pointed-to `[elf:FAMILY_RECORD]` contains a `[elf:CHILD_POINTER]` pointing to the superstructure of this structure.

Default tag
:   `FAMC`

#### `elf:PEDIGREE_LINKAGE_TYPE`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:CHILD_TO_FAMILY_LINK]`

Substructures
:   None

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 7 characters.

    Contains a description of how this child is related to the superstructure's pointed-to `[elf:FAMILY_RECORD]`.
    Known values include {`adopted`, `birth`, `foster`}.

Default tag
:   `PEDI`


#### `elf:CHILD_LINKAGE_STATUS`

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

Supertype
:   `[elf:Structure]`

Superstructures
:   None

Substructures
:   `[elf:EVENT_OR_FACT_CLASSIFICATION]` ?
:   `[elf:DATE_VALUE]` ?
:   `[elf:PLACE_STRUCTURE]` ?
:   `[elf:ADDRESS_STRUCTURE]` ?
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

### `elf:FamilyEvent`

Supertype
:   `[elf:Event]`

Superstructures
:   `[elf:FAMILY_RECORD]`

Substructures
:   `[elf:FirstParentAge]` ?
:   `[elf:SecondParentAge]` ?

Subtypes
:   `[elf:ANUL]`
:   `[elf:CENS]`
:   `[elf:DIVF]`
:   `[elf:ENGA]`
:   `[elf:MARB]`
:   `[elf:MARC]`
:   `[elf:MARR]`
:   `[elf:MARL]`
:   `[elf:MARS]`
:   `[elf:RESI]`
:   `[elf:EVEN#FAM]`

Payload
:   A *string*, which may be limited by subtypes.
    
    The special value `Y` indicates an assertion that the event in question did occur,
    even if it has no subordinate date or place.

### `elf:IndividualAttribute`

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
:   `[elf:COUNT_OF_CHILDREN]`
:   `[elf:COUNT_OF_MARRIAGES]`
:   `[elf:OCCUPATION]`
:   `[elf:POSSESSIONS]`
:   `[elf:RELIGIOUS_AFFILIATION]`
:   `[elf:RESIDES_AT]`
:   `[elf:SOCIAL_SECURITY_NUMBER]`
:   `[elf:NOBILITY_TYPE_TITLE]`
:   `[elf:ATTRIBUTE_DESCRIPTOR]`

### `elf:IndividualEvent`

Supertype
:   `[elf:Event]`

Superstructures
:   `[elf:INDIVIDUAL_RECORD]`

Substructures
:   *inherited*

Subtypes
:   `[elf:BIRT]`
:   `[elf:CHR]`
:   `[elf:DEAT]`
:   `[elf:BURI]`
:   `[elf:CREM]`
:   `[elf:ADOP]`
:   `[elf:BAPM]`
:   `[elf:BARM]`
:   `[elf:BASM]`
:   `[elf:BLES]`
:   `[elf:CHRA]`
:   `[elf:CONF]`
:   `[elf:FCOM]`
:   `[elf:ORDN]`
:   `[elf:NATU]`
:   `[elf:EMIG]`
:   `[elf:IMMI]`
:   `[elf:CENS]`
:   `[elf:PROB]`
:   `[elf:WILL]`
:   `[elf:GRAT]`
:   `[elf:RETI]`
:   `[elf:EVEN#INDI]`

Payload
:   A *string*, which may be limited by subtypes.

    The special value `Y` indicates an assertion that the event in question did occur,
    even if it has no subordinate date or place.

