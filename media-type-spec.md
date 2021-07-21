---
title: "GEDCOM Media Type"
date: 25 June 2021
numbersections: true
editors: Luther Tychonievich
...

# GEDCOM Media Type

{.ednote ...} This is a **working draft** of a description of the GEDCOM file format independent of any particular GEDCOM specification or standard. It is intended to be fully compatible with GEDCOM version 5.5 through 7.0 and makes an effort to be compatible to earlier GEDCOM versions where those are not in contradiction with one another.

My intention in drafting this document is to have a specification suitable for registering the `text/gedcom` Media Type with the IANA without needing to identify any particular version or data model.
{/}


A GEDCOM file is a text file (in any character set, with or without a byte-order mark) storing a representation of a hierarchical set of **structures**.

## General

Where this standard gives a specific technical meaning to a word or
phrase, that word or phrase is formatted in bold text in its initial
definition, and in italics when used elsewhere.
The key words **must**, **must not**, **required**, **shall**, 
**shall not**, **should**, **should not**, **recommended**,
**not recommended**, **may** and **optional** in this standard are to be
interpreted as described in
&#x5B;[RFC 2119](https://tools.ietf.org/html/rfc2119)].

Indented text in grey or coloured boxes does not form a normative part
of this standard, and is labelled as either an example or a note.  

{.ednote} Editorial notes, such as this, are used to record outstanding
issues, or points where there is not yet consensus; they will be
resolved and removed for the final standard.  Examples and notes will be
retained in the standard.

The grammar given here uses the form of EBNF notation defined in §6 of
&#x5B;[XML](https://www.w3.org/TR/xml11/)], except that no significance is
attached to the capitalisation of grammar symbols.  *Conforming*
applications *must not* generate data not conforming to the syntax given
here, but non-conforming syntax *may* be accepted and processed by a
*conforming* application in an implementation-defined manner.

**Characters** are atomic units of text which are specified by reference to
their **code point** number in [Unicode], without regard to any particular
character encoding.
A **string** is a sequence of zero or more *characters* which is used to
encode textual data.

{.ednote} I chose not to reference Basic Concepts here because all we needed from it was character and string, and only a portion of what it said about those.

{.ednote} C0 Control characters are banned starting with 5.3; should we also ban them?

## Structures   {#structures}

Each structure *shall* have a **tag** which matches production `Tag`

    Tag ::= [A-Z] [A-Z0-9_]* | "_" [A-Z0-9_]+

The tags "`CONC`" and "`CONT`" are used to encode breaks in text payloads during serialization (see {§serialize}) and *must not* be used as the tag of any structure.

{.ednote ...}
The rules for tags have varied by version:

- 3.0 said "TAG may contain valid character except the TERMINATOR or SPACE".
- 4.0 said "A tag can consist of any capitalized alpha characters. Use the underscore (`_`) to create a tag that is more than one word"
- 5.0 said "A tag consists of a variable length sequence of letters and digits, beginning with a letter."
- 5.3 through 5.5.1 said
        
        alpha:= [ (0x41)-(0x5A) | (0x61)-(0x7A) | 0x5F ]
        alphanum:= [ alpha | digit ]
        tag:= [ alphanum | tag alphanum ]
    
    5.3 did not say they were case-insensitive or otherwise constraint case, but 5.5 added a separate constraint that "TAGS are always UPPERCASE."
- 7.0's `Tag` ABNF rule matches the proposed `Tag` production exactly.

None of those preclude tags starting with or being entirely digits, but to the best of my knowledge no system has ever permitted initial digits in tags.

I thus consider the rule to have been an unchanging upper-case alphanum + underscore since 4.0, with 4.0's omission of digits and 5.0's omission of underscore and failure to mention upper-case as accidental specification errors.
{/}

Each structure *shall* have either a **text payload** consisting of a (possibly empty) string or a **pointer payload** consisting of a reference to another structure, which *may* be a structure within the file or a structure not in the file.
A structure with text payload equal to the empty string is equivalently said to have "an empty payload" or "no payload".

{.ednote ...}
Every GEDCOM specification leaves open the ability to distinguish between an empty payload like "`1 XYZ `" and a missing payload like "`1 XYZ`" but no known specification uses this ability. 7.0 was the first to explicitly state they were interchangeable.
{/}

{.ednote ...}
The provision of structures not in the file is needed by 7.0's `@VOID@` pointers, and alluded to in 5.5's discussion of the future use of `:` in pointers.
{/}

Each structure *may* be the **substructure** of exactly one other structure.
A structure that is not a substructure is called a **record**.
Substructure relationships *must* be acyclic.
The substructures of a structure are stored as a sequence, not a set, and the order of the substructures of a structure is significant unless that significance is relaxed by the semantic model the GEDCOM file's structures conform to.

{.ednote ...}
Order rules have varied widely by version, sometimes via ambiguous statements.

- 3.0 said "The sender and receiver do not have to agree in advance on the order of field occurrence in a record" and "A sending system is not constrained to transmit records in any particular order" and "TAGs may occur in any order".
- 4.0 said "you can send these tags in any order, as long as you preserve the data relationships".
- 5.0 and 5.3 gave a meaning to order "when multiple opinions or other items are presented for which only one may be expected by a receiving system".
- 5.5 and 5.5.1 said order of a single tag is significant, but the order of distinct tags is not.
- 7.0 said order of a single structure type is significant, and provides a SCHMA whereby distinct tags may have the same structure type.
{/}

Every GEDCOM file contains two special records: the header and trailer.
The "header" *must* have tag "`HEAD`" and no payload.
The "trailer" *must* have tag "`TRLR`", no payload, and no substructures.
These *shall* be the only structures with these tags and *shall not* be pointed to by any pointer payload.
Substructures of the header *should* provide metadata about the entire GEDCOM file
and *should* be sufficient to identify the semantic model the GEDCOM file's structures conform to.


## GEDCOM for Family History {#gedc}

One common use of GEDCOM files is for the storage of information related to family history, including representations of individuals, their relationships and life events.
Several distinct standards exist for using GEDCOM in that way;
the specific family history data standard in use is indicated by a specific substructure of the header as follows.

If the header of the GEDCOM file contains exactly one structure with tag "`GEDC`", that structure and its substructures identify the semantic model the GEDCOM file's structures conform to.
Each semantic model is described in its own specification, which define such details as

- the set of tags allowed and their meaning
- restrictions on payloads
- semantics equivalence rules, such as relaxations of tag order or equivalences of various tags
- what pointers to structures not in the file are permitted
- which serialization relaxations (see {§relax}) are permitted
- any serialization restrictions (see {§restrict}) that conformant applications are expected to adhere to

{.note ...} The Family History Department of The Church of Jesus Christ of Latter-Day Saints created GEDCOM and has published several GEDCOM specifications.

Early versions of GEDCOM contained no identifying version information and cannot be reliably identified by file content alone.

Four versions are identified by the `GEDC`-tagged substructure of the header having two specific substructures:

- one tagged `FORM` with "`LINEAGE_LINKED`" as its payload
- one tagged `VERS` with one of the following four strings as its payload: "`5.0`", "`5.3`", "[`5.5`](https://gedcom.io/specifications/ged55.pdf)", or "[`5.5.1`](https://gedcom.io/specifications/ged551.pdf)". The payload identifies the version of "The GEDCOM Standard" to which the file conforms.

The order of these two substructures of `GEDC` is not significant.

Subsequent versions are identified by the `GEDC`-tagged substructure of the header having exactly one substructure with tag `VERS` and payload being a [semantic version](https://semver.org) of "[The FamilySearch GEDCOM Specification](https://gedcom.io/specifications/FamilySearchGEDCOMv7.html)" to which the file conforms.

Groups other that The Family History Department of The Church of Jesus Christ of Latter-Day Saints have also published specifications, but as of 2021 none of these have seen large-scale implementation.
{/}

## Serialization {#serialize}

Each GEDCOM file consists of the concatenated serialization of every record,
starting with the header record and ending with the trailer record.
The order of the other records in the serialization is arbitrary and is not semantically meaningful.

Every structure is given a non-negative integer called its **level** as part of serialization.
The level of a record is 0;
the level of each substructure of a structure is one greater than the level of the structure.
Thus all substructures of a record have level 1;
all substructures of a substructure of a record have level 2;
and so on.

A structure may be given a string called its **xref** as part of serialization.
Any two structures with an xref in a single GEDCOM file *must* have distinct xrefs.
Any structure pointed to by a pointer payload *must* be given an xref.
The header and trailer *must not* be given an xref.
Other structures *may* be given an xref, which *may* be further restricted by a serialization restrictions (see {§restrict}).

The serialization of a structure consists of the concatenation of the following elements in order:

1. the structure's level encoded as a base-ten integer
2. a *delimiter*
3. if the structure has an xref,
    1. an at sign (U+0040 "`@`")
    2. the structure's xref
    3. an at sign (U+0040 "`@`")
    4. a *delimiter*
4. the structure's tag
5. a serialization of the structure's payload
6. a *line break*
7. the serialization of each of the structure's substructures in order.

The serialization of a pointer payload consists of

1. a single space (U+0020 "` `")
2. an at sign (U+0040 "`@`")
3. if the pointed-to structure is within this file, the xref of the pointed-to structure;\
   otherwise, an xref that is not the xref of any structure in the file
4. an at sign (U+0040 "`@`")

The serialization of a text payload consists of

1. a single space (U+0020); if the payload is empty, this *may* be omitted

2. the text of the payload, with the following modifications:

    - insert splits into the payload as follows:

        - each *line terminator* *shall* be replaced with a CONT-split
        - and any number of CONC-splits *may* be added before or after any character.
        
        Each split is encoded as
        
        1. a *line break*
        2. the structure's level plus 1, encoded as a base-ten integer
        3. a *delimiter*
        4. either `CONT` or `CONC`, depending on the split type
        5. a single space (U+0020 "` `") which *may* be omitted if this split is immediately followed by another split or if there are no characters in the text payload after this split

    - each at sign (U+0040 "`@`") which is either the first character of the text payload or is immediately preceded by a split, and which is not immediately followed by a number sign (U+0023 "`#`"), *shall* be represented by two at signs (U+0040 U+0040, "`@@`")


{.ednote ...}
The above handling of @ is sufficient to encode both (a) the 1.0 through 5.5.1 specifications written rules where @#...@ is special and all other @ are doubled;
and (b) the common practice which ignored those rules. In particular,

- it ensures a text paylaod can never be confused with a pointer
- it keeps pre-7.0 "escape sequences" unchanged
- it adds the minumum number of @ to meet the above two goals
- it allows a spec to require more doubling by defining a text payload modification as part of the semantics of payloads in that spec
{/}

Each **line terminator** matches production `LineTerm`

    LineTerm ::= #xA #xD? | #xD #xA?

When replacing *line terminators* with splits, two-character *line terminators* shall be preferred over one-character *line terminators* in order to minimize the number of splits created.

{.note ...}
Splits are encoded in a way that intentionally looks like substructures, and parsers can be written that parse every line as if it were a structure, then un-double leading at signs, then remove CONT and CONC substructures by concatenating their payloads to the superstructure's payload.

However, CONT and CONC are not structures: they cannot be pointed to, cannot have pointer payloads, cannot have substructures, cannot appear as records, and cannot be further defined by a GEDCOM-compliant specification.
{/}

### Canonical serialization

Canonical GEDCOM serializations are accepted by all GEDCOM processing systems and obey the following limitations:

- Each xref *shall* be 20 characters or fewer in length
    and *shall* match production `Tag`.
- The same character sequence *shall* be used by each **line break** in the file,
    and that character sequence  *shall* match production `LineTerm`.
- Each **delimiter** *shall* be a single space character (U+0020 "` `").
- CONC-splits *shall not* be used.

### Serialization relaxations {#relax}

Specifications and tools may chose to selectively relax canonical restrictions while still abiding by the following restrictions:

- Each xref *shall* match production `XrefRelaxed`

        XrefRelaxed ::= [a-zA-Z0-9_] [#x20-#x3E#x41-#7xE]*

- Each line break *shall* match production `LineBreakRelaxed`
    
        LineBreakRelaxed ::= [#x9#x20]* [#xA#xD] [#x9#xA#xD#x20]*

- Each delimiter *shall* match production `DelimiterRelaxed`

        DelimiterRelaxed ::= [#x9#x20]+

{.example ...}
FamilySearch GEDCOM 7.0 mostly describes the canonical form, but uses the following relaxations:

- allows xref to be any length
- requires xrefs match `[A-Z0-9_]+`
- requires line breaks to match `LineTerm`, but does not require them to all be the same

Because these relaxations obey the relaxed rules above, they are consistent with the GEDCOM file format.
{/}


### Serialization restrictions {#restrict}

A specification *may* impose additional restrictions on serialization that GEDCOM file producers must obey to be considered conformant to that specification.

Examples of such restrictions include but are not limited to

- how to serialize pointers to structures not in the file
- a maximum number of characters of a text payload that may be serialized without an intervening split
- where CONC-splits are permitted
- a requirement that empty payloads always (or never) include the optional space character
- which not-pointed-to structures are given xrefs
- the use of a specific character set or set of character sets
- the agreement between the character set and some structure in the GEDCOM file
- limitations on permitted xrefs

{.example ...}
FamilySearch GEDCOM 7.0 includes the following serialization restrictions:

- only one external structure may be pointed to, and pointers to it are serialized as `@VOID@`
- no CONC-splits allowed
- records may be given xref, but substructures may not
- must use the UTF-8 character set
- xrefs must not be the specific  string "`VOID`"
{/}

## Character Sets

The serialization process defined in {§serialize} describes a relationship between a string an a set of interlinked structures, as defined in {§structures}.
That string can be serialized into binary data using any character set, including both unicode and non-unicode encodings.

GEDCOM predates the first version of [Unicode] and has recommended different character encodings in different versions (see {§gedc}), including [US-ASCII], [ANSEL], and [Unicode].
Some versions recommended the use of a structure with tag `CHAR` in the header of the GEDCOM file with a payload that identifies the character encoding in use.
Specific payloads of the `CHAR`-tagged structure included:

`CHAR` payload      Character sets represented
----------------    ---------------------------------
`ANSEL`             [ANSEL] with the [MARC-8] extensions
`ASCII`             [US-ASCII]
`UNICODE`           UTF-16LE or UTF-16BE, as defined by [Unicode]
`UTF-8`             UTF-8, as defined by [Unicode]

There are known to be systems that provide GEDCOM files which they indicate are in one character set
that have a `CHAR` in the header claiming a different character set.
Such files are in violation of their respective standards
and there exists no reliable way to determine if the providing systems
correctly adjusted the character encoding without modifying the `CHAR`,
or misidentified the character encoding,
or applied another encoding on top of the already-encoded data.





## References

### Normative references

[ANSEL]
:   NISO (National Information Standards Organization).
    *ANSI/NISO Z39.47-1993. Extended Latin Alphabet Coded Character Set for Bibliographic Use*.
    1993.
    (See 
    <http://www.niso.org/apps/group_public/project/details.php?project_id=10>).
    Standard withdrawn, 2013.

[MARC-8]
:   The Library of Congress.
    *MARC 21 Specifications for Record Structure, Character Sets, and Exchange Media*, Part 3.
    2008.
    (See <https://www.loc.gov/marc/specifications/speccharucs.html>)

[RFC 2119]
:   IETF (Internet Engineering Task Force).  *RFC 2119:  Key words for
    use in RFCs to Indicate Requirement Levels.*  Scott Bradner, eds., 1997.
    (See <https://tools.ietf.org/html/rfc2119>.)

[Unicode]
:   The Unicode Consortium.  *The Unicode Standard*, version 12.1.0.
    2019.  (See <https://www.unicode.org/versions/Unicode12.1.0/>.)

[US-ASCII]
:   American Standards Association (ASA).
    *American Standard Code for Information Interchange. ASA X3.4-1963.*
    1963.
    (See <https://www.sr-ix.com/Archive/CharCodeHist/X3.4-1963/index.html>)

[XML]
:   W3C (World Wide Web Consortium). *Extensible Markup Language (XML) 1.1*, 
    2nd edition.  Tim Bray, Jean Paoli, C. M. Sperberg-McQueen, Eve
    Maler, François Yergeau, and John Cowan eds., 2006.  W3C
    Recommendation.  (See <https://www.w3.org/TR/xml11/>.)

### Other references

[GEDCOM 3.0]
:   The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, release 3.0.  9 October 1987.

[GEDCOM 4.0]
:   The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, release 4.0.  August 1989.

[GEDCOM 5.0]
:   The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, draft release 5.0.  25 Sep 1991.

[GEDCOM 5.3]
:   The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, draft release 5.3.  4 Nov 1993.

[GEDCOM 5.5]
:   The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, release 5.5.  2 Jan 1996.
    (See <https://gedcom.io/specifications/ged55.pdf>).

[GEDCOM 5.5.1]
:   The Church of Jesus Christ of Latter-day Saints.
    *The GEDCOM Standard*, draft release 5.5.1.  2 Oct 1999.
    Re-released without draft notice, 15 Nov 2019.
    (See <https://gedcom.io/specifications/ged551.pdf>).

[GEDCOM 7]
:   The Church of Jesus Christ of Latter-day Saints.
    *The FamilySearch GEDCOM Standard*, version 7.0.0. 27 May 2021.
    (See <https://gedcom.io/specifications/FamilySearchGEDCOMv7.html>).

----
Copyright © 2021, [Family History Information Standards Organisation,
Inc](https://fhiso.org/).  
The text of this standard is available under the
[Creative Commons Attribution 4.0 International
License](https://creativecommons.org/licenses/by/4.0/).
