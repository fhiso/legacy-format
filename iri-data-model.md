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

Subtypes
:   `[elf:FAMILY_RECORD]`
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
:   `[elf:FAMILY_RECORD]`
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elfm:HEAD]`

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
:   `[elf:COUNT_OF_CHILDREN#Family]` ?
:   `[elf:MULTIMEDIA_LINK]` \*
:   `[elf:SOURCE_CITATION]` \*

Payload
:   None

Default tag
:   `FAM`

#### `elf:ParentPointer`

This is an abstract datatype and should not be used as the *structure type identifier* of any concrete structure.

{.ednote} This supertype is added primarily as a place-holder for potential future extensions to support more general models of families.

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`


#### `elf:FIRST_PARENT_POINTER`

A pointer to the spouse or parent traditionally presented on the left in a vertical family tree or on the top in a horizontal family tree. In a heterosexual pair union, this is traditionally the husband or father.

Supertype
:   `[elf:ParentPointer]`

Superstructures
:   `[elf:FAM_RECORD]`

Payload
:   A pointer to an `[elf:INDIVIDUAL_RECORD]`

Default tag
:   `HSUB`


#### `elf:SECOND_PARENT_POINTER`

A pointer to the spouse or parent traditionally presented on the right in a vertical family tree or on the bottom in a horizontal family tree. In heterosexual pair unions, this is traditionally the wife or mother.

Supertype
:   `[elf:ParentPointer]`

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
:   `[elf:SOURCE_CITATION]` \*

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

Payload
:   A *block string* of arbitrary length.

Default tag
:   `NOTE`

{.note} GEDCOM did not `[elf:NOTE_STRUCTURE]` as a substructure of `elf:NOTE_RECORD`, but they do appear in the wild and have valid semantics (notes about the note itself) so `elf:NOTE_RECORD` inherits the `[elf:NOTE_STRUCTURE]` substructure from `[elf:Record]` in this specification.


### `elf:REPOSITORY_RECORD`

A representation of where a source or set of sources is located. May be formal, like a library, or informal, like the owner of a family bible.

Supertype
:   `[elf:Record]`

Superstructures
:   `[elfm:Document]`
:   `[elf:Agent]`

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

#### `elf:SOURCE_RECORD_DATA`

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
:   `[elf:Agent]`

Substructures
:   `[elf:SUBMITTER_NAME]` !
:   `[elf:MULTIMEDIA_LINK]` \*
:   `[elf:LANGUAGE_PREFERENCE]` \*

Payload
:   None

{.note} GEDCOM required that an `[elf:USER_REFERENCE_NUMBER]` in an `elf:SUBMITTER_RECORD` be preregistered with Ancestral File, a service that is no longer available. It is *recommended* that an `[elf:USER_REFERENCE_NUMBER]` not be included in an `elf:SUBMITTER_RECORD`.

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
:   `[elf:REPOSITORY_RECORD]`

### `elf:ADDRESS`

Supertype
:   `[elf:Structure]`

Superstructures
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

#### `elf:PHONE_NUMBER`

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

#### `elf:ADDRESS_EMAIL`

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

#### `elf:ADDRESS_FAX`

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

#### `elf:ADDRESS_WEB_PAGE`

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

#### `[elf:FirstParentAge]`

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

#### `[elf:SecondParentAge]`

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
:   `[elf:EVEN#INDI]`

Payload
:   A *string*, which may be limited by subtypes.

    The special value `Y` indicates an assertion that the event in question did occur,
    even if it has no subordinate date or place.

### `elf:MULTIMEDIA_LINK`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:SOURCE_CITATION]`
:   `[elf:FAMILY_RECORD]`
:   `[elf:INDIVIDUAL_RECORD]`
:   `[elf:SOURCE_RECORD]`
:   `[elf:SUBMITTER_RECORD]`
:   `[elf:Event]`

Substructures
:   `[elf:MULTIMEDIA_FILE_REFN]` \*
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
:   `[elf:SOURCE_DATA]`
:   `[elf:ASSOCIATION_STRUCTURE]`
:   `[elf:CHILD_TO_FAMILY_LINK]`
:   `[elf:Event]`
:   `[elf:PersonalName]`
:   `[elf:SOURCE_CITATION]`
:   `[elf:SOURCE_REPOSITORY_CITATION]`
:   `[elf:SPOUSE_TO_FAMILY_LINK]`

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
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.
    
    This *should* be a full name, preferably presented in the order usually spoken by the individual with the name and with the capitalization customary in that individual's culture.
    It *should not* include commas or digits.
    It *should* include exactly two U+002F SOLIDUS `/` characters, one on each side of the family name or surname if present or adjacent to one another if no family name or surname name is known.
    
    Portions of the name may be elided and replaced by three U+002E FULL STOP `...`.
    
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

Payload
:   A *line string*.
    It is RECOMMENDED that implementations support payloads of at least 120 characters.

    A comma-separated list of names of jurisdictions, where each element of the list is subsumed within all subsequent elements.
    
    If this structure has a `[elf:PLACE_HIERARCHY]` substructure or there is a default `[elf:PLACE_HIERARCHY]` defined for the dataset, then this payload *should* contain one name for each jurisdictional elements in that `[elf:PLACE_HIERARCHY]`, using empty strings in place of any unknown or non-present elements.

Default tag
:   `PLAC`

### `elf:SOURCE_CITATION`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:Record]`
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

#### `elf:SOURCE_CITATION_DATA`

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
:   A *pointer* to a `[elf:FAMILY_RECORD]`

    It *must* be the case that the pointed-to `[elf:FAMILY_RECORD]` contains a `[elf:ParentPointer]` pointing to the superstructure of this structure.

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

Indicates that the person resided at the location indicated by the `[elf:PLACE_STRUCTURE]` substructure.

Supertype
:   `[elf:IndividualAttribute]`

Substructures
:   `[elf:PLACE_STRUCTURE]` !

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

### `elf:BAPTISM`

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

Payload
:   A pointer to a `[elf:FAMILY_RECORD]`.

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
    `HUSB` means the adoption was to the individual indicated by the `[elf:FIRST_PARENT_POINTER]` of the `[elf:FAMILY_RECORD]` pointed to by the payload of the containing superstructure;
    `WIFE` means the adoption was to the individual indicated by the `[elf:SECOND_PARENT_POINTER]`pointed to by the payload of the containing superstructure;
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
:   A pointer to a `[elf:FAMILY_RECORD]`.

    The pointed-to record describes the family unit associated with the individual event described by the superstructure.

Default tag
:   `FAMC`

### `elf:AGE_AT_EVENT`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FirstParentAge]`
:   `[elf:SecondParentAge]`

Payload
:   A *line string*, which uses an age-specification microformat matching the production `Age`:

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
    
    Additional white space may be inserted between tokens.

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

### `elf:COUNT_OF_CHILDREN#Family`

Supertype
:   `[elf:Structure]`

Superstructures
:   `[elf:FAMILY_RECORD]`

Payload
:   A *line string* taking the form of a decimal number.
    It is RECOMMENDED that implementations support payloads of at least 3 characters.
    
    The total number of children this family unit had,
    either at some (unspecified) point in time or in total its entire existence.
    This does not need to match the number of children identified through `[elf:CHILD_POINTER]` substructures of the containing superstructure.

Default tag
:   `NCHI`

{.ednote} It seems odd to me that `elf:COUNT_OF_CHILDREN#Family` is not a `elf:FamilyEvent` (or `elf:FamilyAttribute`, though no such supertype currently exists) as surely the number of children of a family would need sourcing and an as-of date? Should we leave it as a stand-alone structure, or boost it to event status?

