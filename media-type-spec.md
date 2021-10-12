---
title: "The GEDC Serialisation Format"
date: 12 October 2021
numbersections: true
editors: Luther Tychonievich
...

# The GEDC Serialisation Format

{.ednote ...} This is a **working draft** of a description of the object model and serialisation scheme used by PAF 2.0 and its successors and by all GEDCOM specifications except GEDCOM-X. It defines only the object model and serialisation scheme, not any particular data model; in this way, it is similar to JSON or XML.

My intention in drafting this document is to have a specification suitable for registering the `text/gedc` Media Type with the IANA.
We could then register other media types that use this serialisation,
such as `text/elf+gedc` or `text/gedcom+gedc`.
{/}

This standard defines an object model and serialisation format called GEDC.
GEDC is the underlying format used by many applications and specifications that predate this standard,
including 

- \[PAF 2.0]: a manual for software produced by The Family History Department of The Church of Jesus Christ of Latter-Day Saints (FHD) in 1985 
- \[GEDCOM 3.0], \[GEDCOM 4.0], \[GEDCOM 5.0], \[GEDCOM 5.3], \[GEDCOM 5.5], \[GEDCOM 5.5.1], \[GEDCOM 5.6]: specifications published by FHD from 1986 to 2000 
- \[Event GEDCOM]: a specification published by Commsoft in 1994
- \[GEDCOM 5.5EL]: a specification published by GEDCOM-L in 2018
- \[GEDCOM 5.5.5]: a specification published by Tamura Jones in 2019
- \[GEDCOM 7.0]: a specification published by FHD in 2021

Each of these documents has included a definition of GEDC integrated with a definition of a particular data model.
Each has also included its own restrictions on certain parts of GEDC.

:::note
GEDCOM is an acronym for GEnealogical Data Communication.
While most GEDCOM specifications have used GEDC,
at least three do not:

- PAF 1.0 used an incompatible format that is sometimes called "GEDCOM 1.0"
- \[GEDCOM-X] uses JSON and XML instead of GEDC
- \[GEDCOM 5.6] included two serialisations, one of which was XML
:::


This standard defines the GEDC serialisation format independent of any particular data model.
It also specifies the five parameters which various GEDC-compliant specifications have used to restrict GEDC,
together with their allowed range.


## General

Where this standard gives a specific technical meaning to a word or
phrase, that word or phrase is formatted in bold text in its initial
definition, and in italics when used elsewhere.
The key words **must**, **must not**, **required**, **shall**, 
**shall not**, **should**, **should not**, **recommended**,
**not recommended**, **may** and **optional** in this standard are to be
interpreted as described in
&#x5B;[RFC 2119](https://www.rfc-editor.org/info/rfc2119)].

Indented text in grey or coloured boxes does not form a normative part
of this standard, and is labelled as either an example or a note.  

{.ednote} Editorial notes, such as this, are used to record outstanding
issues, or points where there is not yet consensus; they will be
resolved and removed for the final standard.  Examples and notes will be
retained in the standard.

This standard depends on FHISO's **The Pattern Datatype** standard described in \[FHISO Patterns].
Concepts defined in that standard are used here without further definition.

{.note} In particular, the precise meaning of *pattern*, and *match* are given in \[FHISO Patterns].

**Characters** are atomic units of text which are specified by reference to
their **code point** number in \[Unicode], without regard to any particular
character encoding.
A **string** is a sequence of zero or more *characters* which is used to
encode textual data.

*Characters* may be identified in this standard by their hexadecimal
*code point* prefixed with "U+".

{.example}  The exclamation mark "`!`" is *code point* 33 in Unicode,
or 21 in hexadecimal.  In this standard it written U+0021.

{.ednote} I chose not to reference Basic Concepts here because all we needed from it was character and string, and only a portion of what it said about those.

This standard refers to some characters by their \[General Category] as defined by \[Unicode].
In particular, general categories *Cc* (control codes) and *Z* (separators) are used in this standard without further definition.

This standard uses the name **LF** to refer to U+000A;
**CR** to refer to U+000D;
and **CRLF** to refer to U+000A followed by U+000D.


## Object model

A **GEDC dataset** consists of a forest: a sequence of n-ary trees, where the nodes of the trees are **GEDC structures** and the children of each node are stored as a sequence, not a set.
In addition to its position in the forest, each *structure* has

- One **tag**, a *string* from set `Tag`

- Zero or one **cross-reference identifier**, a *string* from set `XrefID` that is not shared by any other *cross-reference identifier* in the dataset.

- Exactly one of the following options:
    
    - **No payload**
    
    - A **pointer payload**, which points to a structure with a cross-reference identifier in the same dataset
    
    - A **string payload**

{.note ...}
While GEDC can distinguish between the "no payload" and "*string payload* containing the empty string" cases, as of 2021 all such specifications have treated these two cases as semantically equivalent.
{/}

## Tiers

There are two tiers of GEDC datasets, distinguished by their *string payloads*:

Tier-1 GEDC
:   *String payloads* contain neither *LF* nor *CR*
    and may be limited to at most *maxLen* code points.
    
Tier-2 GEDC
:   *String payloads* can contain line breaks and have unlimited length, but cannot distinguish between *LF*, *CR*, and *CRLF*.


Tier-1 and tier-2 datasets have the same expressive power by virtual of the Tier-1 [CONT and CONC universal tags](#cont-and-conc).
During serialisation, a tier-2 dataset is first converted to a tier-1 dataset, then serialised;
similarly, parsing first produces a tier-1 dataset which is then converted to a tier-2 dataset.

{.note ...}
Early GEDC-compatible specifications were designed for memory-constrained applications and discussed tier-1 only.
More recent specifications generally operate in tier-2.
{/}

## Serialisation

Each Tier-1 *structure* can be expressed as a single line of text;
an entire dataset is serialised by converting all *structures* to lines in a preorder traversal of the dataset
and concatenating the lines with a *string* from the set `lineSep` between each one,
optionally adding another *string* from the set `lineSep` at the end.

Each line consists of the following:

1. An integer expressed with base-10 ASCII digits called the line's **level**.
    Root nodes have *level* 0;
    other *structures* have *level* one greater than their parent node.
    
    A line with *level* $n > 0$ represents a substructure of the nearest preceding line with *level* $n-1$.

2. A *string* from set `Delim`

3. If the *structure* has a *cross-reference identifier*,
    1. U+0040 (commercial at, `@`)
    2. the *structure*'s *cross-reference identifier*
    3. U+0040 (commercial at, `@`)
    4. A *string* from set `Delim`

5. The *structure*'s *tag*

6. If the *structure* has a *pointer payload*,
    
    1. U+0020 (a single space)
    2. U+0040 (commercial at, `@`)
    3. the *cross-reference identifier* of the pointed-to *structure*
    4. U+0040 (commercial at, `@`)

    Otherwise, if the *structure* has a *string payload*,

    1. U+0020 (a single space)
    2. if the *string payload* begins with U+0040 (`@`)
        but not with U+0040 U+0023 (`@#`),
        then the first character is doubled by inserting a U+0040 here
    3. the *structure*'s *string payload*

## Universal Tags

The semantics meaning of each *structure*, *tag*, and *string payload* is generally left to individual data models to define.
However, the five specific *structures*, identified by their *tag* and position in the dataset, have special universal meaning regardless of the data model in use.

### HEAD

The first structure in every GEDC dataset *must* have *tag* "`HEAD`",
with no cross-reference identifier and no payload.
This structure is called the **header**.
Metadata about the dataset itself *should* be placed in the *header*;
regular data *should* be placed elsewhere in the dataset.

### HEAD.GEDC

At most one substructure of the *header* may have *tag* "`GEDC`".
This structure is called the **data model identifier**.
The *data model identifier* *must not* have a cross-reference identifier and *should not* have a payload.
Information used to identify the specific data model used in the dataset *should* be placed in the *data model identifier* and its substructures.

### HEAD.CHAR

At most one substructure of the *header* may have *tag* "`GEDC`".
This structure is called the **character set hit**.
The *character set hit* *must not* have a cross-reference identifier and *must* have a payload
which identifies the character set in which this dataset was originally serialised.

### CONT and CONC   {#cont-and-conc}

Each *structure* with *tag* "`CONT`" or "`CONC`" is restricted as follows:

- Must not appear anywhere in tier-2 datasets
- Must not be the root of a tree
- Must not be a substructure of a structure with a *pointer payload*
- Must not be a substructure of a structure with *tag* "`CONT`" or "`CONC`" 
- Any structure that appears before it in the sequence of substructures must also have *tag* "`CONT`" or "`CONC`"

Additionally, these structures never have cross-reference identifiers
and never have pointer payloads.

If a *structures* with *tag* "`CONT`" or  `CONC`" or its superstructure has no payload, it *shall* be treated as it if has an empty *string payload* instead.

A *structure* whose first substructure has *tag* "`CONC`"
can be converted to an equivalent *structure* without that substructure
by concatenating the substructure's payload to the end of the superstructure's payload.

A *structure* whose first substructure has *tag* "`CONT`"
can be converted to an equivalent *structure* without that substructure
by concatenating one `lineBreak` followed by the substructure's payload to the end of the superstructure's payload.
If the superstructure's *string payload* ended with U+000D, the `lineBreak` chosen *must not* be U+000A.

## Parametrisation

Several dialects of GEDC datasets exist; all are broadly compatible, but some impose more restrictions than others on characters and lengths.
These variants are defined by five parameters.

There is one numeric parameter:

*maxLen*
:   The maximum number of characters that may appear a tier-1 *string payload*.
    
    Valid values are any positive integer or two special values:
    
    - "Unconstrained", effectively meaning ∞
    - "Unconstrained with no CONC", effectively meaning ∞ and prohibiting structures with tag `CONC` even in tier-1.

There are four parameters that are sets of *strings*.
Two (`Tag` and `XrefID`) limit contents of *structures*
and all four constrain serialised representations.
The serialisation-only sets (`LineSep` and `Delim`) are limited to range over a specific set of characters characters in order to facilitate [Character encoding detection].
None of these sets may ever contain the empty string.

`Tag`
:   The set of permitted *tags*.
    
    This set always includes all *strings* matching the *pattern* `[_A-Z][_A-Z0-9]{1,7}`.
    
    No *string* in this set ever includes any character from general category *Cc* or *Z*.
    
    No *string* in this set ever begins with U+0040 (`@`).
    
    The default value of this set is all *strings* that match the *pattern*
    `[_A-Z][_A-Z0-9]*`.

`XrefID`
:   The set of permitted *cross-reference identifies*.
    
    This set always includes all *strings* matching the *pattern* `[A-Z0-9]{1,15}`.
    
    No *string* in this set ever includes any character from general category *Cc* except U+0009 (horizontal tab).

    No *string* in this set ever contains U+0040 (`@`).
    
    No *string* in this set ever begins with U+0023 (`#`).

    The default value of this set is all *strings* that match the *pattern*
    `[A-Z0-9][_A-Z0-9]*`.

`LineSep`
:   The set of *strings* that can occur between two serialised lines.

    This set always includes *LF*, *CR*, and *CRLF*.

    This set may be defined to include additional *strings*
    that begin with either *LF* or *CR*
    and contain only *LF*, *CR*, space (U+0020), and tab (U+0009).
    
    The default value for this set is its minimal value: {*LF*, *CR*, *CRLF*}.

`Delim`
:   The set of *strings* that can occur between the pre-payload components of a line.
    
    This set always includes the length-1 *string* containing just U+0020 (space).
    
    This set may be defined to include additional *strings*
    contain only space (U+0020) and tab (U+0009).

    The default value for this set is its minimal value: just the single-space *string*.

{.note ...}
General-purpose GEDC processing libraries can be created
by parsing with the most generous possible definitions of these parameters:

- unconstrained *maxLen*
- `Tag` is one or more characters other than *Cc* or *Z* that do not begin with `@`
- `XrefID` is one or more characters other than `@` and non-tab *Cc* and that do not begin with `#`
- `LineSep` is any sequence of ASCII whitespace beginning with either *CR* or *LF*
- `Delim` is any sequence of spaces and tabs

and serialise with the most limited possible definitions

- `Tag` is implicit in the tags used in the dataset
- `XrefID` is the *strings* matching the *pattern* `[_A-Z0-9]{1,15}`
- `LineSep` is just CRLF, or just CR, or just LF
- `Delim` is just a single space

However, *maxLen* does need to be specified
as "Unconstrained with no CONC" is incompatible with any finite bound.
{/}

## Character encoding detection

GEDC datasets are serialised as text, and can be converted to octets using any character set.

If the character set is specified externally to and along with the octet stream (for example by an HTTP Content-type header), any character set may be used.

Otherwise, the following character sets may be used:

- UTF-32LE, UTF-32BE, UTF-16LE, UTF-16BE
    
    Detected either by a leading byte-order mark
    or by by consulting the first four bytes of the file:
    
    | first 4 bytes | encoding |
    |---------------|----------|
    | 00 30 00 20   | UTF-16BE |
    | 30 00 20 00   | UTF-16LE |
    | 00 00 00 30   | UTF-32BE |
    | 30 00 00 00   | UTF-32LE |

- An ASCII-compatible character set -- i.e., one where a byte between 01 and 7F (inclusive) always indicates the corresponding code point.
    
    To detect the specific character encoding, parse the header (skipping any bytes outside the 01–7F range) to find the *character set hint*.
    If there is no character set hint, the encoding is UTF-8.
    Otherwise, the payload of the *character set hint* is the name of the character set in use.

Specific character set names known to have been used by previous specifications include

`CHAR` payload      Character sets represented
----------------    ---------------------------------
`ANSEL`             [ANSEL] with the [MARC-8] extensions
`ASCII`             [US-ASCII]
`UNICODE`           UTF-16LE or UTF-16BE, as defined by [Unicode]
`UTF-8`             UTF-8, as defined by [Unicode]


{.note ...}
GEDC predates Unicode
and early specifications that used the GEDC object model and serialisation format
presented ANSEL as the recommended character encoding.
Files that lack the *character set hint* and are not valid UTF-8
are not conformant with this specification,
but may have been created in ANSEL by an older application that did no fully implement its specification.
{/}




## References

### Normative references

\[ANSEL]
:   NISO (National Information Standards Organization).
    *ANSI/NISO Z39.47-1993. Extended Latin Alphabet Coded Character Set for Bibliographic Use*.
    1993.
    (See 
    <http://www.niso.org/apps/group_public/project/details.php?project_id=10>).
    Standard withdrawn, 2013.

\[FHISO Patterns]
:   FHISO (Family History Information Standards Organisation).
    *The Pattern Datatype*.  First public draft.

\[General Category]
:   The Unicode Consortium. 
    "Character Properties,"
    *The Unicode Standard*, version 14.0.0, section 4.5.
    2021.  
    (See <https://www.unicode.org/versions/Unicode14.0.0/ch04.pdf>)

\[MARC-8]
:   The Library of Congress.
    "MARC-8 Encoding Environment."
    *MARC 21 Specifications for Record Structure, Character Sets, and Exchange Media*,
    Character sets and encoding options: Part 2.
    2008.
    (See <https://www.loc.gov/marc/specifications/speccharucs.html>)

\[RFC 2119]
:   IETF (Internet Engineering Task Force).  *RFC 2119:  Key words for
    use in RFCs to Indicate Requirement Levels.*  Scott Bradner, eds., 1997.
    (See <https://tools.ietf.org/html/rfc2119>.)

\[Unicode]
:   The Unicode Consortium.  *The Unicode Standard*, version 14.0.0.
    2021.  (See <https://www.unicode.org/versions/Unicode14.0.0/>.)

\[US-ASCII]
:   American Standards Association (ASA).
    *American Standard Code for Information Interchange. ASA X3.4-1963.*
    1963.
    (See <https://www.sr-ix.com/Archive/CharCodeHist/X3.4-1963/index.html>)


### Other references

\[Event GEDCOM]
:   COMMSOFT, Inc.
    *GEDCOM Event-Oriented Form*, draft release 1.0. 12 September 1994.

\[GEDCOM 3.0]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *Genealogical Data communication (GEDCOM)*, release 3.0.  9 October 1987.

\[GEDCOM 4.0]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, release 4.0.  August 1989.

\[GEDCOM 5.0]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, draft release 5.0.  25 Sep 1991.

\[GEDCOM 5.3]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, draft release 5.3.  4 Nov 1993.

\[GEDCOM 5.5]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, release 5.5.  2 Jan 1996.
    (See <https://gedcom.io/specifications/ged55.pdf>).

\[GEDCOM 5.5.1]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, draft release 5.5.1.  2 Oct 1999.
    Re-released without draft notice, 15 Nov 2019.
    (See <https://gedcom.io/specifications/ged551.pdf>).

\[GEDCOM 5.5EL]
:   GEDCOM-L.
    *GEDCOM 5.5EL*.  Published 5 Oct 2004, updated through 17 Mar 2018.
    <http://wiki-de.genealogy.net/Gedcom_5.5EL>, accessed 2021-10-12.

\[GEDCOM 5.5.5]
:   Tamura Jones.
    *The GEDCOM 5.5.5 Specification with Annotations*. 2 Oct 2019.

\[GEDCOM 5.6]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, draft release 5.6.  18 December 2000.

\[GEDCOM 7]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *The FamilySearch GEDCOM Standard*, version 7.0.0. 27 May 2021.
    (See <https://gedcom.io/specifications/FamilySearchGEDCOMv7.html>).

\[GEDCOM-X]
:   Intellectual Reserve, Inc.
    *The GEDCOM X Conceptual Model*. Published 1 June 2012.
    <https://github.com/FamilySearch/gedcomx> accessed 2021-10-12.

\[PAF 2.0]
:   The Family History Department of The Church of Jesus Christ of Latter-day Saints.
    *Personal Ancestral File™ Release 2.0 Family Records Data Structure Description*.
    December 185.


----
Copyright © 2021, [Family History Information Standards Organisation,
Inc](https://fhiso.org/).  
The text of this standard is available under the
[Creative Commons Attribution 4.0 International
License](https://creativecommons.org/licenses/by/4.0/).
