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
-   Creating a archival standard, as GEDCOM 5.5 is no longer available from its authoring corporation.

The working title of this draft proposed standard is the FHISO Extended Legacy Format, or ELF.

This draft is intended to provide enough context for meaningful discussion of the value and desirable content of a potential future standard.
It will likely change significantly prior to being released.

----

# The organization of ELF data

Every ELF dataset consists of the following, in order:

1. one [HEAD] record, 
1. optionally one [SUBN] record, 
1. any number of [FAM], [INDI], [OBJE], [NOTE], [REPO], [SOUR], and [SUBM] records in any order
1. one [TRLR] record

## Structures

Each structure consists of the following parts:

-------------------------------------------------------------------------------------------
Name            Required?                               Notes
-------------   -------------------------------------   -----------------------------------
Tag             Required                                short identifiers
                                                        describing structure

Identifier      Forbidden for [HEAD] and [TRLR],        short identifiers allowing
                required for all *records*,             pointers to point to structure
                optional otherwise

Payload         Required, Optional, or Forbidden        EITHER string content OR pointer
                (varies by Tag)                         (but never both)

Substructures   Required or Optional                    structured content
                (varies by Tag)
-------------------------------------------------------------------------------------------

A structure is called a *record* if it is not contained within another structure.
If a structure is contained within another structure, it is called a *substructure* and its containing structure is called its *superstructure*.
The *root* of a record is itself; the *root* of a substructure is the root of its parent.
The set of substructures sharing a common root are called the *nested substructures* of that root.

{.ednote} About the 32K limit: the OBJE tag allows exceeding this limit by letting one OBJE structure contain an OBJE pointer to the continuation of the structure.  I see no reason why this cannot be extended generally: a structure of any type may contain a single pointer to the same type, that being a continuation of the structure.  I thus believe we can leave the limit out of this discussion and leave it for the serialization discussion instead.

### Tags

A tag is a string of between 2 and 31 characters from the class `[A-Za-z0-9_]`.
No two distinct tags within a data set may share the same initial 15 characters.
It may contain at most one underscore, and maps to an IRI as follows:

-   If there is no underscore, the IRI of the tag is `http://terms.fhiso.org/legacy/` followed by the tag.
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

- `!` means "required": there must be one and only one of these.
- `?` means "optional": there may be one and no more than one of these.
- `*` means "set-valued": there may be zero or more of these.  Some implementations may provide a maximum number for some or all set-values substructures.
- `+` means "non-empty set-valued": there must be one and may be more than one of these. In the current spec, `+` only appears for the .[OBJE].[FILE].



