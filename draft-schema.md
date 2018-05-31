---
title: "Multiple IRI/Tag options"
...

# The GEDCOM 5.3 SCHEMA

{.ednote ...}
GEDCOM 5.3 had a .`HEAD`.`SCHEMA` which was removed from GEDCOM 5.4.
It did not allow for IRIs, but provided an extensible mechanism where IRIs can easily be added.
The `SCHEMA` tag has, within itself, a full nested tree of tags that provides context.

Pros:

- based on a previous GEDCOM standard
- readily extensible
- context sensitive

Cons:

- does not allow for context-insensitive mappings or namespaces
- quite verbose 
- Per GEDCOM 5.4, "Although the schema concept is valid and
essential to the growth of GEDCOM, it is too complex and premature to be implemented
successfully into current products"
- as written, any extension pseudostructure would be interpreted as a tag definition instead of an extension

Editorial notes:

- This text is loosely based on GEDCOM 5.3, but is mostly original to this document.
- GEDCOM 5.3 was ambiguous how nested user-defined tags would be defined
- GEDCOM 5.3 lacked a definition of what the tag `ISA` indicated (one example was provided, but is not followed )
{/}

## The `SCHEMA` pseudostructure

The .`HEAD` record *may* contain a single *schema* pseudostructure
with *tag* `SCHEMA` and no *payload*.

### Defining pseudostructures

For each *structure* that may appear in a *dataset* in a the *context* .`C`~1~.`C`~2~. ... .`C`~*n*~,
there is a parallel **defining pseudostructure** with the same *tag* and no *payload*
that may appear in the *context* .`HEAD`.`SCHEMA`.`C`~1~.`C`~2~. ... .`C`~*n*~.

Subpseudostructures
:   (*defining pseudostructure*)\*
:   `[DEFN]`?
:   `[IRI]`?
:   `[ISA]`*
:   `[LABL]`?

A *defining pseudostructure* MUST have an `IRI` subpseudostructure if it has any subpseudostructures other than *defining pseudostructures*.

### `http://terms.fhiso.org/elf/DEFN`  {#DEFN}

User Tag Definition: A formal description of the user defined tag. This description can be used by the receiving system to give
meaning to the user defined tags.

Contexts
:   (*defining pseudostructure*).`[DEFN]`

Description
:   Developer-facing text defining the meaning and semantics of the structure.

Payload
:   A *block string* of arbitrary length.

Substructures
:   None


### `http://terms.fhiso.org/elf/IRI`  {#IRI}

The formal meaning of this tag.

Contexts
:   (*defining pseudostructure*).`[IRI]`

Description
:   An *structure type identifier* which is represented by this *tag* when this *tag* appears as the *tag* of a *structure*.

Payload
:   a *string* that SHALL take the form of an IRI matching the `IRI` production in §2.2 of [[RFC 3987](//tools.ietf.org/html/rfc3987)].

Substructures
:   None


### `http://terms.fhiso.org/elf/ISA`  {#ISA}

The *structure type identifier* of a supertype of a *structure*.

If a *defining pseudostructure* has multiple `[ISA]` psueostructures,
they SHALL be distinct and SHALL be ordered such that each `[ISA]` other than the last identifies a subtype of the type identified by the subsequent `[ISA]`.

{.example ...}
`http://terms.fhiso.org/elf/NATU` could appear in *schema* as

````gedcom
0 HEAD
1 SCHEMA
2 INDI
3 NATU
4 IRI http://terms.fhiso.org/elf/NATU
4 ISA http://terms.fhiso.org/elf/IndividualEvent
````

or as


````gedcom
0 HEAD
1 SCHEMA
2 INDI
3 NATU
4 IRI http://terms.fhiso.org/elf/NATU
4 ISA http://terms.fhiso.org/elf/IndividualEvent
4 ISA http://terms.fhiso.org/elf/Event
4 ISA http://terms.fhiso.org/elf/Structure
````

but not as

````gedcom
0 HEAD
1 SCHEMA
2 INDI
3 NATU
4 IRI http://terms.fhiso.org/elf/NATU
4 ISA http://terms.fhiso.org/elf/IndividualEvent
4 ISA http://terms.fhiso.org/elf/Structure
4 ISA http://terms.fhiso.org/elf/Event
````

because `http://terms.fhiso.org/elf/Structure` is not a subtype of `http://terms.fhiso.org/elf/Event`.
{/}

Contexts
:   (*defining pseudostructure*).`[IRI]`

Description
:   An *structure type identifier* of a supertype of the *structure* is represented by this *tag* when this *tag* appears as the *tag* of a *structure*.

Payload
:   a *string* that SHALL take the form of an IRI matching the `IRI` production in §2.2 of [[RFC 3987](//tools.ietf.org/html/rfc3987)].

Substructures
:   None


### `http://terms.fhiso.org/elf/LABL`  {#LABL}

A name assigned to a field or product which helps to identify it.

Contexts
:   (*defining pseudostructure*).`[IRI]`

Description
:   The long name of a user defined tag. For example, `HOSP` tag would have a long name of `HOSPITAL`. This name should be a name that could be used as a field label for reports and screens. The name may include underscore characters (`_`).

Payload
:   a *line string*.  It is RECOMMENDED that implementations support payloads of at least 15 characters.

Substructures
:   None

{.ednote} It is not clear what implied character limitations lead to the "The name may include underscore characters (`_`)." in GEDCOM 5.3.

## Tags to/from IRIs

When an implementation encounters a *tag* `C`~*n*~ in context .`C`~1~.`C`~2~. ... .`C`~*n*~, it SHALL be converted to a *structure type identifier* as follows:

1. If there exists a pseudostructure .`HEAD`.`SCHEMA`.`C`~1~.`C`~2~. ... .`C`~*n*~.`[IRI]`, the payload of that pseudostructure is the *structure type identifier* of `C`~*n*~.
2. Otherwise, the *structure type identifier* is obtained by concatenating `http://terms.fhiso.org/elf/` and the tag `C`_*n*_.

When an implementation is encoding a dataset, it SHALL select a *tag* `C`~*n*~ and include a pseudostructure .`HEAD`.`SCHEMA`.`C`~1~.`C`~2~. ... .`C`~*n*~.`[IRI]` for each .`C`~1~.`C`~2~. ... .`C`~*n*~ whose *structure type identifier* is not equal to the concatenation of `http://terms.fhiso.org/elf/` and `C`_*n*_. It MAY also include additional pseudostructures within the *schema*.

{.ednote} The encoding part above needs to be refined.

----


# Our June 2017 draft's version

{.ednote ...}
The following was the text in the June 2017 draft of ELF-serialization.
Conceptually, it uses two different tools:
each Tag is either directly mapped to an IRI, with a set of *is-a* relationships for that IRI encoded as a list;
or it is transformed into an IRI via a prefix replacing production.

Pros: 

- succinct representation of many IRIs if they belong to a common namespace
- can encode arbitrary distinct IRIs both within and outside of the namespace
- provides a compact representation of fallback semantics

Cons:

- does not allow for context-specific mappings
- does not have room for extension metadata other than is-a relationships (e.g., patterns, known substructures, semantic definitions, etc)

Editorial notes:

- The text is somewhat opaque, with only one example late in the description
{/}

## IRI to/from Tag

Each *structure type identifier* in a dataset
is represented by a **tag** in the serialisation format.
The mapping between *tag*s and *structure type identifier*s is handled by an **IRI dictionary**.
The *IRI dictionary* may also define a set of alternate IRIs for a *tag*.

{.note} The intent of the set of alternate IRIs
is to aid implementations in handling unknown extensions
without the overhead of a full discovery mechanism.

{.example ...} Suppose that `http://terms.fhiso.org/sources/authorName` is a subtype of `http://terms.fhiso.org/elf/AUTH` that provides additional structural information within the *payload*.  An implementation might create the mapping 

| Tag  | IRIs                                        |
|------|---------------------------------------------|
|`AUTH`|`http://terms.fhiso.org/sources/authorName` <br> `http://terms.fhiso.org/elf/AUTH`|

to inform implementations that lines tagged `AUTH`
are `authorName`s, 
but can be treated like `AUTH`s
if full `authorName` semantics are not understood.
{/}


### IRI dictionary format  {#IRI-dictionary}

The IRI dictionary contains any mix of

-   zero or one *default namespace definition*,
-   zero or more *namespace definitions*, and
-   zero or more *individual tag mappings*.

The *default namespace definition* specifies an absolute IRI.

Each *namespace definition* maps a key matching the production `Prefix` to an absolute IRI.
No two *namespace definition*s within a single dataset may share a key.

    Prefix  ::= [0-9A-Za-z]* "_"

Each *individual tag mapping* maps a key matching the production `Tag` to an ordered sequence of absolute IRIs.
No two *individual tag mapping*s within a single dataset may share a key.


### Tag to IRI {#tag2iri}

To convert a *tag* to an IRI, the following checks are performed in order; the first one that matches is used.

1.  If the *tag* is one of `CHAR`, `CONC`, `CONT`, `DEFN`, `PRFX`, or `TRLR`,
    the *tag* is identifying a *pseudo-structure* and does not map to an IRI.

1.  Otherwise, if the *tag* is a key of an *individual tag mapping*,
    the IRI associated with that *tag* is the first IRI in the IRI sequence of that mapping.
    Additional IRIs in that sequence provide *hints* to implementations that *structures* with this IRI MAY be treated like *structures* with other IRIs in the sequence, with a *preference* for the first usable IRI.

1.  Otherwise, if the *tag* contains one or more underscores,
    let *p* be the substring of the *tag* up to and including the first underscore
    and *s* be the substring after the first underscore.
    If *p* is a key in the prefix dictionary,
    the IRI associated with the *tag* 
    is the value associated with *p* concatenated with *s*.

1.  Otherwise, if there is a *default namespace definition*,
    the IRI associated with the *tag*
    is the IRI of the *default namespace definition* concatenated with the *tag*.

1.  Otherwise, the IRI associated with the *tag*
    is `http://terms.fhiso.org/elf/` concatenated with the *tag*.

{.example ...} Given the following namespace mappings dictionary entries:

Key     Value
------  ---------------------------------------------------------------
`X_`    `http://example.com/extensions/`
`_`     `http://example.com/old_extensions.html#`

and the following individual tag mapping:

Key     Value
------  -------------------------------------------
`_UID`  `http://example.com/UUID` <br> `http://purl.org/dc/terms/identifier`

the following tags convert to the following IRIs:

Tag         IRI
----------  ------------------------------------------------
`HEAD`      `http://terms.fhiso.org/elf/HEAD`
`X_LAT`     `http://example.com/extensions/LAT`
`_LOC`      `http://example.com/old_extensions.html#LOC`
`_UID`      `http://example.com/UUID`

Note that `http://purl.org/dc/terms/identifier` is *not* the IRI of `_UID`:
even if an implementation does not understand `http://example.com/UUID`,
the first element in the IRI sequence is always the IRI of a tag,
the others being instead hints about how to treat that type.
{/}


### IRI to Tag {#iri2tag}

Every *structure* type IRI MUST be replaced by a *tag* as part of serialisation,
and every such replacement MUST be reversible via the IRI dictionary.
The simplest technique to accomplish this is to create an *individual tag mapping* for every IRI with a unique key for each.
However, it is RECOMMENDED that more compact *namespace definition*s be used;
in particular, implementations SHOULD

-   use the default prefix for all *structure* types documented in the [Elf-DM].
-   use one *namespace definition* for each *namespace* (as defined in [Vocabularies]),
    with a key of two or more characters.
-   use just-underscore keys only for compatibility communication with implementations that expect particular *tag*s.
-   provide additional IRIs for extensions that extend *structure* types documented in the [Elf-DM].

{.ednote} Should we say "implementations MUST NOT use any of the six pseudo-structure tags" or add contexts to the definition of pseudo-structures?  In other words, is .INDI.NOTE.DEFN a pseudo-structure or can it be defined as a structure?

### IRI dictionary encoding {#IRI-enc}

The IRI dictionary is encoded as a set pseudo-substructures of the *head*.

Each *namespace definition* is encoded as a *pseudo-structure* with *tag* `[PRFX]`
and *payload* consisting of
the key of the *namespace definition*, a *delimiter*, and the absolute IRI of the *namespace definition*.

Each *default namespace definition* is encoded as a *pseudo-structure* with *tag* `[PRFX]`
and *payload* consisting of
the absolute IRI of the *default namespace definition*.

Each *individual tag mapping* is encoded as a *pseudo-structure* with *tag* `[DEFN]`
and a *payload* consisting of
the key of the *individual tag mapping*, a *delimiter*, and the sequence of absolute IRIs of the *individual tag mapping* separated by *whitespace*.

{.note} The permission of *whitespace* separation allows either all IRIs to be encoded in a single line or some to be encoded in `[CONT]` lines.

{.example ...} Given the following namespace mappings dictionary entries:

Key     Value
------  ---------------------------------------------------------------
`X_`    `http://example.com/extensions/`
`_`     `http://example.com/old_extensions.html#`

and the following individual tag mapping:

Key     Value
------  ---------------------------------------------------------------
`_UID`  `http://example.com/UUID` <br> `http://purl.org/dc/terms/identifier`

the serialisation could begin

````
0 HEAD
1 CHAR UTF-8
1 DEFN _UID http://example.com/UUID
2 CONT http://purl.org/dc/terms/identifier
1 PRFX X_ http://example.com/extensions/
1 PRFX _ http://example.com/old_extensions.html#
````
{/}

----

# A flat dictionary with context selectors

{.ednote ...}
The is designed to try to marry the best of both worlds; in particular,

- it wraps all mappings inside a single `SCHEMA`, like GEDCOM 5.3
- it is a flat structure with few pseudostructures, like June 2017
- it allows both contextual and context-free definitions
- it is extension-friendly

This is still a very rough outline
{/}

A `SCHEMA` can contain any number of `DEFN`s.
The payload of a `DEFN` is a tag.
Each `DEFN` must contain exactly one `IRI`
and may contain any number of `ISA` and `CONTEXT`.

{.ednote} Perhaps we should add things like *description*, *known substructures*, *payload*, etc?

If a `DEFN` contains at least one `CONTEXT`, it only applies if at least one of its `CONTEXT` matches.
`CONTEXT`s payloads are an optional `/` and a sequence of IRIs separated by whitespace;
if `/` is present, the sequence begins are level 0; otherwise it is unanchored.
It ends with the direct superstructure of the defined structure.


{.example ...}
````gedcom
0 HEAD
1 SCHEMA
2 DEFN BAPL
3 IRI https://example.com/gedcom/lds/BAPL
3 ISA https://fhiso.org/elf/BAPM
2 DEFN DATE
3 IRI https://example.com/DATE
3 CONTEXT / http://terms.fhiso.org/elf/REPO http://terms.fhiso.org/elf/ADDR
3 CONTEXT http://terms.fhiso.org/elf/IndividualAttribute
2 DEFN DATE
3 IRI https://fhiso.org/elf/DATE
````
{/}

