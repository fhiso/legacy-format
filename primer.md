---
title: "Extended Legacy Format (ELF)"
subtitle: Primer
date: 8 October 2019
numbersections: true
...

# Extended Legacy Format (ELF):<br/> Primer

{.ednote ...} This is an **early draft** of a primer intended to provide
a broad overview of FHISO's proposed suite of Extended Legacy Format
(ELF) standards.  This document is not endorsed by the FHISO membership,
and may be updated, replaced or obsoleted by other documents at any
time.

Comments on this draft should be directed to the
[tsc-public@fhiso.org](http://tech.fhiso.org/tsc-public) mailing list.
{/}

FHISO's **Extended Legacy Format** (or **ELF**) is a hierarchical
serialisation format and genealogical data model that is fully
compatible with GEDCOM, but with the addition of a structured
extensibility mechanism.  It also clarifies some ambiguities that were
present in GEDCOM and documents best current practice. 

The **GEDCOM** file format developed by The Church of Jesus Christ of
Latter-day Saints is the *de facto* standard for the exchange of
genealogical data between applications and data providers.  Its most
recent version is GEDCOM 5.5.1 which was produced in 1999, but despite
many technological advances since then, GEDCOM has remained unchanged.

{.note} Strictly, GEDCOM 5.5 was the last version to be publicly
released back in 1995.  However a draft dated 2 October 1999 of a
proposed GEDCOM 5.5.1 was made public; it is generally considered to
have the status of a standard and has been widely implemented as such.

FHISO are undertaking a program of work to produce a modernised yet
backward-compatible reformulation of GEDCOM under the name ELF, the new
name having been chosen to avoid confusion with any other updates or
extensions to GEDCOM, or any future use of the name by The Church of
Jesus Christ of Latter-day Saints.  This document is one of five
that form the initial suite of ELF standards, known collectively as ELF
1.0.0:

* **ELF: Primer**.  This is not a formal standard, but is being
  released alongside the ELF standards to provide a broad overview of
  ELF written in a less formal style.  It gives particular emphasis to
  how ELF differs from GEDCOM.

* **ELF: Serialisation Format**.  This standard defines a
  general-purpose serialisation format based on the GEDCOM data format
  which encodes a dataset as a hierarchical series of lines, and
  provides low-level facilities such as escaping.

* **ELF: Schemas**.  This standard defines flexible extensibility and
  validation mechanisms on top of the serialisation layer.  Although it
  is an *optional* component of ELF 1.0.0, future ELF extensions to ELF
  will be defined using ELF schemas.

* **ELF: Date, Age and Time Microformats**.  This standard defines
  microformats for representing dates, ages and times in arbitrary calendars,
  together with how they are applied to the Gregorian, Julian, French
  Republican and Hebrew calendars.  

* **ELF: Data Model**.  This standard defines a data model based on the
  lineage-linked GEDCOM form, reformulated to be usable with the ELF
  serialisation model and schemas.  It is not a major update to the
  GEDCOM data model, but rather a basis for future extension and
  revision.


None of the material in this document forms a normative part of the ELF
standards.

## Overview

ELF is a structured, line-based text format for encoding genealogical
data in a hierarchical manner that is both machine-readable and
human-readable.

{.ednote} This section is intended to give a simple overview of ELF for
readers who are not familiar with GEDCOM.  This needs writing.

## Differences from GEDCOM

### Character encodings

GEDCOM files are allowed to encoded in ASCII, ANSEL, UTF-8 or UTF-16,
though the latter (which GEDCOM refers to simply as "Unicode") is not
commonly encountered.  Other character encodings may occasionally be
found.  The default is ANSEL, which was withdrawn as a standard in 2013
and is not supported in many common character encoding libraries.

ELF applications are required to be able to read ASCII, ANSEL and UTF-8,
but are  only required to be able to write ELF in UTF-8.  Support for
all other encodings, including UTF-16, is optional. 

{.note} FHISO gave serious consideration to making ANSEL support
optional too, because supporting it does impose a significant
implementation burden on vendors, but after researching how frequently
current GEDCOM files were encoded using ANSEL (as opposed to claiming to
be ANSEL but actually using the ASCII subset of ANSEL), it was felt
necessary to make this a requirement.


### Whitespace in the grammar

The GEDCOM standard is imprecise about where whitespace is permitted in
a document.  ELF introduces a distinction between what ELF parsers are
required to be able to read and what ELF writers are allowed to
generate.  FHISO's general philosophy has been to require parsers to
accept input in a wide range of formats, but to require writers only to
product output in a single standard form.  Most current applications do
this to varying degrees, and ELF standardises this behaviour.

ELF parsers are required to read input where the lines are indented, or
where multiple space or tab characters are used to separate the level
number, the optional structure identifier (e.g. `@I1@`) and the tag.
Blank lines and lines consisting only of whitespace must be accepted and
ignored.  Conformant applications must not generate any of these
constructs when writing ELF.

{.example ...} ELF parsers are required to accept the following input,
but must not produce it as output:

    0 @I1@ INDI
      1    NAME William Shakespeare
      1    BAPM
        2  DATE @#DJULIAN@ 26 APR 1564
    
    0 TRLR
{/}

{.note}  Applications are not expected to encounter data like this at
all often, but the implementation cost of accepting such data is
considered low enough that it is mandated in ELF in the interest of
backwards compatibility.

### Unicode escapes

ELF deprecates the use of ANSEL and only requires ELF writers to be able
to generate UTF-8 output.  Nevertheless, applications are permitted to
write ELF in non-Unicode character sets like ANSEL or ASCII.  This means 
a mechanism is needed to allow Unicode
characters to be included in non-Unicode output.  ELF introduces
**Unicode escapes** for this purpose.  These allow any Unicode character
to be denoted using its hexadecimal code point number, which is written
between `@#U` and `@`.

{.example ...}  The two following lines will be interpreted identically.

    1 NAME André
    1 NAME Andr@#UE9@

The "é" character is code point U+00E9 in Unicode.  It is a convention
that Unicode code point numbers are normally written with leading zeros
to make them four digits long.  ELF does not care whether they are
included, and the escape could have been written `@#U00E9@`.
{/}

The `@#U`&hellip;`@` syntax follows the general GEDCOM escape sequence
syntax, in which the character after the `@#` prefix denotes the type of
escape.  In GEDCOM 5.5, the only escape type is "`D`", which is used to
specify a calendar, though other escapes have been used in earlier
versions of GEDCOM.  The GEDCOM spec says that an escape sequence must
be followed by a space or the end of the line, and ELF does not change
that.  This space is dropped when decoding a Unicode escape, so that
characters in the middle of a word can be escaped.

{.example ...}  These two lines will be interpreted identically.

    1 NAME João
    1 NAME Jo@#UE3@ o
{/}

{.ednote}  We are reviewing whether we can remove the space after the
escape while retaining backwards compatibility.

### Escaping `@` signs

Any `@` sign occurring in data is supposed to be doubled-up to escape it
in GEDCOM.  This is required because `@` signs are also used in pointers
and escape sequences.  In practice, many applications fail to escape `@`
signs properly when writing GEDCOM, and as a result most current
applications accept unescaped `@` signs in input.  This is particularly
relevant to `EMAIL` structures.

{.example ...}  Email addresses in GEDCOM are supposed to be encoded as
follows:

    1 EMAIL user@@example.com

But very frequently the `@` sign does not get doubled-up, and lines
such as the following illegal GEDCOM are commonly encountered:

    1 EMAIL user@example.com
{/}

ELF recognises that current practice deviates from the GEDCOM standard,
and allows unescaped `@` signs in input when they cannot be confused
with an escaped `@` sign or an escape sequence.  The ELF standard
defines exactly how values are unescaped to achieve this, and requires 
the unescaping of doubled `@` signs to happen concurrently with unescaping
Unicode escapes and before merging lines split with `CONC` and `CONT`.

When writing ELF, applications are required to escape `@` signs
unless they are part of an escape sequence or pointer.  Applications may
use Unicode escapes for this, but doubling `@` signs is recommended on
compatibility grounds.

### Leading or trailing whitespace

A long-standing ambiguity in GEDCOM with more serious consequences is
whether or GEDCOM allows leading or trailing whitespace to be ignored
from a line's payload, or whether it must be preserved.  This ambiguity
adversely affects how the GEDCOM's `CONC` line continuation mechanism
works.

{.example ...}   
    0 @N1@ NOTE This is a long note
    1 CONC  which spans multiple lines.

GEDCOM defines `CONC` as a general line continuation mechanism, and says
its payload is appended to the previous line's payload.  In this example
there are two space characters after the `CONC` tag.  Should both spaces
be considered the delimiter between the tag and the `CONC` line's
payload?  Or should the second space be treated as part of the `CONC`
line's payload?  In different places, the GEDCOM standard says both,
though probably the latter is intended.  In the former interpretation,
the note becomes "This is a long notewhich spans multiple lines.",
whereas in the latter interpretation it is "This is a long note which
spans multiple lines."  
{/}

Because of this uncertainty, it has become best practice to split lines
in the middle of a word.  ELF makes this recommendation explicit.

{.example ...}  Although this example is too short for there to be a
genuine need to split the note onto two lines, ELF recommends using
`CONC` to break the line mid-word:

    0 @N1@ NOTE This is a long note wh
    1 CONC ich spans multiple lines.
{/}

{.ednote}  We may introduce a new `CONSP` line breaking mechanism that
allows lines to split reliably between words.

The ELF standard recommends that ELF parsers treat leading whitespace as
part of the payload, but stops short of making this a requirement.  If
an ELF writer is serialising a value with leading whitespace which is
known to be significant, it is required to escape the first character
using a Unicode escape.

{.example ...}
    0 NOTE @U#20@   +-------+-------+
    1 CONT @U#20@   |       |       |
    1 CONT @U#20@ Alice    Bob     Eve = Fred
    1 CONT @U#20@                      |
    1 CONT @U#20@                   Michael
{/}

A different approach to taken to trailing whitespace, and ELF parsers
are recommended to discard all trailing whitespace from lines; again,
the ELF standard stops short of making this a requirement.  Necessary
trailing whitespace should be escaped with a Unicode escape.

{.note} The rationale for this difference is twofold.  First, trailing
whitespace is normally invisible, and it can easily be lost or distorted
when being processed by external applications.  Secondly, unlike with
leading whitespace which the GEDCOM standard explicitly suggests as one
way of handling `CONC` line breaking, the GEDCOM standard does not
suggest trailing whitespace should be used and is more explicit about
the likelihood of it being lost.


### Character sets

ELF files may contain any Unicode character, including tabs, with the
proviso that applications are allowed to reject files containing 60
rarely used control characters, even if they are escaped using Unicode
escapes.

{.note} This restriction is inherited from FHISO's Basic Concepts
standard.  These 60 characters match the `RestrictedChar` production in
that §2 of Basic Concepts.  These same characters are not permitted in
XML and in many other common data formats, even if escaped, for example
with XML entities, so this limitation in ELF is not unusual.

\vfill

----
Copyright © 2019, [Family History Information Standards Organisation,
Inc](https://fhiso.org/).
The text of this standard is available under the
[Creative Commons Attribution 4.0 International
License](https://creativecommons.org/licenses/by/4.0/).

