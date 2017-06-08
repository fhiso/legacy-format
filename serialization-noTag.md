---
title: "Extended Legacy Format (ELF)"
subtitle: Serialization Format
date: 24 May 2017
numbersections: true
...
# ELF Serialization Format

This is an early exploratory draft of a proposed FHISO standard that is fully compatible with GEDCOM 5.5.

The GEDCOM specification described a document model, a set of tags, and a serialization format.
This document describes the serialization format portion of that specification.

The working title of this draft proposed standard is the FHISO Extended Legacy Format Serialization Format,
also known as the ELF serialization format
or ELF-file.

This draft is intended to provide enough context for meaningful discussion of the value and desirable content of a potential future standard.
It will likely change significantly prior to being released.

----

## Encoding

This document presents a particular method of converting structures into a stream of characters.
It does not specify the *character* encoding needed to convert those characters into an octet stream suitable for network transfer or file saving.



### Lines     {#Line}

A ELF-file consists of a set of lines.
Each line consists of several elements.
Lines MUST NOT begin with a space.
Line MUST NOT contain any control characters (U+0000 through U+0017, U+007F) *except* the *terminator* described below; all regular expressions provided assume this constraint (e.g., `[^@]` means `[^@\u0000-\u001f\u007f]`).

The elements of each line (in order) are

1.  The **level**: a base-ten integer matching the regular expression `0|[1-9][0-9]*`

    A single space (U+0020) MUST follow the *level*, delimiting it from the next element.

2.  Optionally, a **xref_id**: an identifier surrounded by at-signs, matching the regular expression `@[a-zA-Z0-9_][^@]*@`

    If the *xref_id* is present, a single space (U+0020) MUST follow it, delimiting it from the next element.

3.  A **tag**: a string (generally [mapping to a IRI](#IRIs-and-Tags)) matching the regular expression `[0-9a-zA-Z_]+`

4.  Optionally, a **payload line**: a string matching the regular expression `([^@]|@#[^@]*@)*`

    If the *payload line* is present, a single space (U+0020) MUST precede it, delimiting it from the previous element.

5.  A **terminator**: Either U+000A or U+000D or both (in either order; i.e., `\n\r?|\r\n?`).

{.note} Although the above locations of delimiters is specified in both GEDCOM 5.5 and GEDCOM 5.5.1, those specifications also suggest that some implementations may produce multi-space delimiters and that parsers should support those files.  It is RECOMMENDED that parsers accept malformed files that would be correctly formatted if additional spaces were permitted within lines.

#### Non-structure Lines

Most lines correspond one-to-one with structures in the dataset.
However, lines with three tags do not correspond to a structure:

-   `CONT` is used to encode line breaks; see [Multi-line Strings](#CONT)
-   `CONC` may be used to break long payloads into multiple lines; see [Line Splitting](#CONC)
-   `PRFX` is used to define the tag-name/IRI mapping; see [Prefix dictionary encoding](#PRFX)

These are the *only* tags that do not map to a IRI, being part of the serialization format rather than part of the underlying data model being serialized.


### IRIs and Tags {#IRIs-and-Tags}

{.ednote} This entire section documents material that is not part of GEDCOM

All structure-type identifying IRIs in a dataset are mapped to tags as part of this serialization format.
This is done by creating a *prefix dictionary*.

#### Prefix Dictionary Format

A prefix dictionary is a set of key ↦ value pairs.
The keys are valid tag names ore prefixes thereto, matching `[0-9a-zA-Z_]*`.
The values are absolute IRIs.
Each key must be unique, and each must match at least one of the following cases:

-   a string ending in an underscore (i.e., matching `[0-9a-zA-Z]*_`)
-   an entire tag name as it appears in the dataset

#### Tag → IRI {#tag2iri}

To convert a tag to a IRI, the following checks are performed in order; the first one that matches is used.

1.  If that tag is a key in the prefix dictionary, the value associated with that key is the structure's IRI.

1.  Otherwise, if the tag contains one or more underscores, let *p* be the substring of the tag up to and including the first underscore and *s* be the substring after the first underscore.  If *p* is a key in the prefix dictionary, the structure's IRI is the value associated with *p* concatenated with *s*.

1.  Otherwise, the structure's IRI is `http://terms.fhiso.org/elf/` concatenated with the tag.

{.ednote ...} We could make this more flexible by not having a hard-coded default namespace; we'd allow empty-string keys and replacing the last rule above with 

1.  Otherwise, if there is an empty string key, the structure's IRI is the value associated with the empty string concatenated with the tag

1.  Otherwise, the structure's IRI is `_:` concatenated with the tag.\

It is this author's opinion that such flexibility is not needed.
{/}

{.example ...} Given the following prefix dictionary entries:

Key     Value
------  -------------------------------------------------
`X_`    `http://example.com/xtensions/`
`_`     `http://example.com/old_extensions.html#`
`_UID`  `http://example.com/UUID`

The following tags represent to the following IRIs:

Tag         IRI
----------  ------------------------------------------------
`HEAD`      `http://terms.fhiso.org/elf/HEAD`
`X_LAT`     `http://example.com/xtensions/LAT`
`_LOC`      `http://example.com/old_extensions.html#LOC`
`_UID`      `http://example.com/UUID`

{/}


#### IRI → Tag {#iri2tag}

Every structure type IRI MUST be replaced by a tag as part of serialization,
and every such replacement MUST be reversible via an entry in the prefix dictionary.
The simplest technique to accomplish this is to create a key-value pair for every IRI with a unique key for each.
However, it is RECOMMENDED that a more compact prefix dictionary be used;
in particular, implementations SHOULD

-   use the default prefix for all structure types documented in the [Elf Data Model](#data-model.html).
-   use one entry for each [namespace](http://tech.fhiso.org/policies/vocabularies), with a key of two or more characters.
-   use full-tag keys or just-underscore keys only for compatibility communication with GEDCOM extensions that expect particular tag names.

#### Prefix dictionary encoding {#PRFX}

The prefix dictionary is encoded as a set of lines after the line beginning `0 HEAD` and before the next `TopLevel`.
Each is entry encoded on a single [Line] with

1.  level `1`
1.  no *xref_id*
1.  tag `PRFX`
1.  the payload is
    1.  the key
    1.  a space character
    1.  the value IRI

It is RECOMMENDED that the entire prefix dictionary be encoded immediately following the `0 HEAD` line before any of the `HEAD` structure's substructures.
The order of the `PRFX` lines is not important.

{.example ...} Given the following prefix dictionary entries:

Key     Value
------  -------------------------------------------------
`X_`    `http://example.com/xtensions/`
`_`     `http://example.com/old_extensions.html#`
`_UID`  `http://example.com/UUID`

The serialization would begin

````gedcom
0 HEAD
1 PRFX X_ http://example.com/xtensions/
1 PRFX _ http://example.com/old_extensions.html#
1 PRFX _UID http://example.com/UUID
````
{/}



### Encoding a structure

A structure is encoded as a [Line](#Line) (or possibly several [Lines](#Line), as described in [Multi-line Strings] and [Line Splitting]), immediately followed by the encoding of all of its substructures.

1.  The *level* of a [TopLevel](tag-list.html#TopLevel) is `0`.
    The *level* of a substructure is 1 greater than the *level* of its superstructure.
    
    For example, the level of a the `ADDR` structure in the context .`HEAD`.`SOUR`.`CORP`.`ADDR` is `3`.

2.  If the structure has an *identifier*, that identifier surrounded by COMMERCIAL AT U+0040 (`@`) is the *xref_id*; otherwise, there is no *xref_id*.
    
    For example, the *xref_id* of a structure with *identifier* "S23" is `@S23@`.

3.  The *tag* is the a sting which will map to the structures IRI using the [prefix dictionary](#tag2iri).  Tags are case-sensitive.

    For example, the *tag* of an `http://terms.fhiso.org/elf/ADDR` structure is `ADDR`.

4.  The *payload line* has several possibilities:
    
    -   If the *payload* of the structure is None, there is no *payload line*.
    
    -   If the *payload* of the structure is a pointer, the *payload line* is the *identifier* of the pointed-to structure surrounded by U+0040 COMMERCIAL AT (`@`).
        
        For example, if the payload of a .`INDI`.`ALIA` points to an `INDI` with identifier "I45", the *payload line* is `@I45@`.
    
    -   If the *payload* of the structure is a string, the *payload line* is a prefix of the payload string, determined and encoded as described in [Payload String Encoding].

5.  The *terminator* may be any of the four valid line terminator sequences.
    It is RECOMMENDED that all structures in a given file be terminated with the same sequence.

It is RECOMMENDED that parsers accept files that are not strictly compliant with this specification by

-   ignoring blank lines and leading white space in a line
-   ignoring additional white space between elements
-   accepting missing payloads and empty payloads in one another's place (e.g., treating `"1 NOTE\n"` and `"1 NOTE \n"` interchangeably).

{.ednote} Although the GEDCOM specification never provides permission for whitespace stripping or multi-space delimiters, its Appendix A states that such occurs in the description of the CONC tag: "many GEDCOM values are trimmed of trailing spaces and some systems look for the first non-space starting after the tag to determine the beginning of the value."


#### Substructures

The [line(s)](#line) encoding a structure's *tag*, *identifier*, and *payload* is followed immediately by an encoding of each of its substructures.
The order of substructures of different IRIs is arbitrary, but the order of substructures with the same IRI MUST be preserved.

{.example ...}
The following are all equivalent:

````gedcom
0 @jane@ SUBM
1 NAME Jane Doe
1 LANG Gujarati
1 LANG English
````

````gedcom
0 @jane@ SUBM
1 LANG Gujarati
1 LANG English
1 NAME Jane Doe
````

````gedcom
0 @jane@ SUBM
1 LANG Gujarati
1 LANG English
1 NAME Jane Doe
````

But the following is *not* equivalent to any of the above:

````gedcom
0 @jane@ SUBM
1 NAME Jane Doe
1 LANG English
1 LANG Gujarati
````
{/}

{.example ...} It is the IRI that determines if order must be preserved; thus, the order of the two notes in the following must be preserved even though one has a pointer as its payload and the other has a string:

````gedcom
1 NAME Jno. /Banks/
2 NOTE @N34@
2 NOTE This is probably an abbreviation for John
````
{/}

#### Payload String Encoding

Payload strings must be encoded specially based on the following constraints:

-   *payload line*s may not contain newline or carriage return characters, so multi-line strings must be encoded in several lines.

-   U+0040 COMMERCIAL AT has special meaning.

-   Long strings may be split into several lines.
    It is RECOMMENDED that no line (including the level and terminator) exceed 255 characters.

{.note} GEDCOM listed the 255-character limit on line length as REQUIRED, not RECOMMENDED.

{.ednote} I propose relaxing the line length limit to RECOMMENDED as it no longer appears to be important given the size of computer memory.


##### Using U+0040 `@`

{.ednote} GEDCOM allows several aspects of "escapes" that are not represented here, such as the ability to encode at-signs as part of the escape text.  So far as the authors of this specification know, the omitted details have never been used in any GEDCOM-producing or GEDCOM-consuming tool.

Some structure's payloads utilize a special "escape" of the form `@#[^@]*@`; the `http://terms.fhiso.org/elf/DATE` structure is one example.
Such escapes should be preserved as-is during encoding, and MUST NOT be split into multiple lines as part of [Line splitting].

All other (literal) at signs MUST be encoded by doubling them:
a `@` in a *payload* becomes a `@@` in a *payload line*.
The `@@` MUST NOT be split into multiple lines as part of [Line splitting].


#### Multi-line Strings {#CONT}

A multi-line string is encoded as follows:

-   The first line of the string is encoded as the *payload line* of its structure's line.
-   The remaining lines of the string are encoded in order immediately following the structure's line as lines with the `CONT` tag.
    -   Each `CONT` line has a *level* one larger than the *level* of the structure.
    -   `CONT` lines never have an *xref_id*.

{.example ...} If the payload of a .`HEAD`.`NOTE` would be represented in a C-like language as `"Example:\nmulti-line notes\nsupported."`, the `NOTE` would be encoded as

````gedcom
1 NOTE Example:
2 CONT multi-line notes
2 CONT supported.
````
{/}


##### Line Splitting {#CONC}

Any string may be split into several lines; such splits may occur between any two characters, except they may not occur between matched `@` characters. It is RECOMMENDED that they occur between non-whitespace characters.

{.ednote ...} The non-whitespace recommendation is a compromise between the GEDCOM specification, which does not mention this constraint at all, and the specification's Appendix A, which states 

> Values that are split for a CONC tag must always be split at a non-space. If the value is split on a space the space will be lost when concatenation takes place.
{/}

When split, the substring before the split remains as the *payload line* of the existing line; the substring after the split is encoded immediately following that line as lines with the `CONC` tag.

-   Each `CONC` line has a *level* one larger than the *level* of the structure.
-   `CONC` lines never have an *xref_id*.

{.note} The *level* of a `CONC` is based on the *structure*, not any preceeding `CONC` or `CONT` line.

This process may be repeated as many times as desired, and may be applied to any line of a multi-line string.

{.example ...} If the payload of a .`HEAD`.`NOTE` would be represented in a C-like language as `"Example: some text to split up."`, the `NOTE` could be encoded as 

````gedcom
1 NOTE Example: some text to split up.
````

or

````gedcom
1 NOTE Exa
2 CONC mple: some text to spli
2 CONC t up.
````

or with any other combination of splits.
{/}


{.example ...} If the payload of a .`HEAD`.`NOTE` would be represented in a C-like language as `"Example: multi-line\ntext to split."`, the `NOTE` could be encoded as 

````gedcom
1 NOTE Example: multi-line
2 CONT text to split.
````

or

````gedcom
1 NOTE Exampl
2 CONC e: multi-line
2 CONT text t
2 CONC o sp
2 CONC lit.
````

or with any other combination of splits.
{/}



## Decoding

{.ednote} Do we (1) write this section, (2) rework the [Encoding] section to be less procedural, or (3) assume people can infer decoding based on encoding?
