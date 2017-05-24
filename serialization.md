---
title: "Extended Legacy Format (ELF)"
subtitle: Serialization Format
date: 24 May 2017
numbersections: true
...
This is an early exploratory draft of a proposed FHISO standard that is fully compatible with GEDCOM 5.5.

The GEDCOM specification described a document model, a set of tags, and a serialization format.
This document describes the serialization format portion of that specification.

The working title of this draft proposed standard is the FHISO Extended Legacy Format Serialization Format,
also known as the ELF serialization format
or ELF-file.

This draft is intended to provide enough context for meaningful discussion of the value and desirable content of a potential future standard.
It will likely change significantly prior to being released.

----

# Lines

A ELF-file consists of a set of lines.
Each line consists of several elements.
Lines MUST NOT begin with a space.
Line MUST NOT contain any control characters (U+0000 through U+0017, U+007F) *except* the *terminator* described below; all regular expressions provided assume this constraint (e.g., `[^@]` means `[^@\u0000-\u001f\u007f]`).

The elements of each line (in order) are

1.  The **level**: a base-ten integer matching the regular expression `0|[1-9][0-9]*`

    A single space (U+0020) MUST follow the *level*, delimiting it from the next element.

2.  Optionally, a **xref_id**: an identifier surrounded by at-signs, matching the regular expression `@[a-zA-Z0-9_][^@]*@`

    If the *xref_id* is present, a single space (U+0020) MUST follow it, delimiting it from the next element.

3.  A **tag**: a tag name, matching the regular expression `[0-9a-zA-Z_]+`

4.  Optionally, a **payload line**: a string matching the regular expression `([^@]|@#[^@]*@)*`

    If the *payload line* is present, a single space (U+0020) MUST precede it, delimiting it from the previous element.

5.  A **terminator**: Either U+000A or U+000D or both (in either order; i.e., `\n\r?|\r\n?`).

{.note} Although the above locations of delimiters is specified in both GEDCOM 5.5 and GEDCOM 5.5.1, those specifications also suggest that some implementations may produce multi-space delimiters and that parsers should support those files.  It is RECOMMENDED that parsers accept malformed files that would be correctly formatted if additional spaces were permitted within lines.


# Encoding a structure

Each structure is encoded as one (or in some cases, several consecutive) line(s).

1.  The *level* of a [TopLevel](tag-list.html#TopLevel) is `0`.
    The *level* of a substructure is 1 greater than the *level* of its superstructure.
    
    For example, the level of a the `ADDR` structure in the context .`HEAD`.`SOUR`.`CORP`.`ADDR` is `3`.

2.  If the structure has an *identifier*, that identifier surrounded by COMMERCIAL AT U+0040 (`@`) is the *xref_id*; otherwise, there is no *xref_id*.
    
    For example, the *xref_id* of a structure with *identifier* "S23" is `@S23@`.

3.  The *tag* is the structure's tag name.  Tags are case-sensitive.

    For example, the *tag* of an `ADDR` structure is `ADDR`.

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

## Payload String Encoding {#Payload Encoding}

Payload strings must be encoded specially based on the following constraints:

-   *payload line*s may not contain newline or carriage return characters, so multi-line strings must be encoded in several lines.

-   U+0040 COMMERCIAL AT has special meaning.

-   Long strings may be split into several lines.
    It is RECOMMENDED that no line (including the level and terminator) exceed 255 characters.

{.note} GEDCOM listed the 255-character limit on line length as REQUIRED, not RECOMMENDED.

### Multi-line strings

A multi-line string is encoded as follows:

-   The first line of the string is encoded as the *payload line* of its structure's line.
-   The remaining lines of the string are encoded in order immediately following the structure's line as lines with the `CONT` tag.
    -   Each `CONT` line has a *level* one larger than the *level* of the structure.
    -   `CONT` lines never have an *xref_id*.

For example, if the payload of a .`HEAD`.`NOTE` would be represented in a C-like language as `"Example:\nmulti-line notes\nsupported."`, the `NOTE` would be encoded as

````gedcom
1 NOTE Example:
2 CONT multi-line notes
2 CONT supported.
````


### Using U+0040 `@`

{.ednote} GEDCOM allows several aspects of "escapes" that are not represented here, such as the ability to at-signs as part of the escape text.  So far as the authors of this specification know, the omitted details have never been used in any GEDCOM-producing or GEDCOM-consuming tool.

Some tags may utilize a special "escape" of the form `@#[^@]*@`; the `DATE` tag is one example.
Such escapes should be preserved as-is during encoding, and MUST NOT be split into multiple lines as part of [Line splitting].

All other (literal) at signs MUST be encoded by doubling them:
a `@` in a *payload* becomes a `@@` in a *payload line*.
The `@@` MUST NOT be split into multiple lines as part of [Line splitting].


### Line Splitting {#Line splitting}

Any string may be split into several lines; such splits may occur between any two characters, except they may not occur between matched `@` characters. It is RECOMMENDED that they occur between non-whitespace characters.

{.ednote} The non-whitespace recommendation is a compromise between the GEDCOM specification (which does not mention this constraint at all) and the specification's Appendix A (which states "Values that are split for a CONC tag must always be split at a non-space. If the value is split on a space the space will be lost when concatenation takes place.").

When split, the substring before the split remains as the *payload line* of the existing line; the substring after the split is encoded immediately following that line as lines with the `CONC` tag.

    -   Each `CONC` line has a *level* one larger than the *level* of the structure.
    -   `CONC` lines never have an *xref_id*.

This process may be repeated as many times as desired.  For example, if the payload of a .`HEAD`.`NOTE` would be represented in a C-like language as `"Example: some text to split up."`, the `NOTE` could be encoded as 

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
