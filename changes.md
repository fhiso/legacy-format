# Proposed intentional changes to the GEDCOM 5.5.1 specification

The following changes are proposed with the goal of clarifying the existing practice, not to improve or modify current practice.
However, they are changes to the spec as written, and thus ought to be made explicit.



## Length limits become implementation-defined

The existing spec has various length limits built into the spec.
Notable examples include a 32K-per-record limit on all records,
character-length limits on almost all structure payloads,
and some structures that are not listed as permitting `CONC` or `CONT` tags.

We propose that all such limits shall be replaced with the following statements, the first two of which are adapted from Annex B of the C++ standard:

1.  Because computers are finite, implementations are inevitably limited in the size of the values they can successfully process.
    Every implementation shall document those limitations where known.
    This documentation may cite fixed limits where they exist,
    say how to compute variable limits as a function of available resources,
    or say that fixed limits do not exist or are unknown.

2.  The limits may constrain quantities such as payload length, record size, lines per payload, substructures per structure, or others.
    In some instances this specification gives a RECOMMENDED minimum for one or more of these quantities.
    However, these quantities are only guidelines and do not determine compliance.

3.  When an implementation encounters a value in excess of its ability to process,
    it SHOULD attempt to continue to process the remainder of the data
    and MAY truncate the excess information to meet implementation-defined limits.
    If truncation occurs, the implementation SHOULD inform the user that potential data loss has occurred.

4.  In addition to size limitations, implementations MAY limit some payloads to not contain newlines.
    Implementations encountering unsupported newlines SHOULD treat them the same way they would excess size.

The GEDCOM specification is also ambiguous as to where `CONT` and `CONC` are permitted.
We propose taking the general statement in the **grammar rules** section,

> Long values can be broken into shorter GEDCOM lines by using a subordinate CONC or CONT tag.

to be a universal rule, independent of the presence or absence of a `CONT` or `CONC` in the lineage-linked grammar specification, and state that `CONC` and `CONT` are *encoding* details rather than *data model* concepts.

Changing limits to be implementation-defined also involves 

-   Replacing all `{0:3}` cardinalities in the grammar to `{0:M}` -- limiting to 3 is implementation-defined.
-   Removing the ambiguity regarding `line_value` length, whitespace stripping, and escapes -- for example, implementations that cannot encode strings of 500 spaces should now document that themselves.


## Reframing Escapes

Escapes are in the GEDCOM lexical specification of the GEDCOM grammar, implied to be admissible in any location within and payload of any structure.
However, the only known implementation of escapes is in the prefix position of `DATE`-tag payloads.

To simplify the grammar and remove the under-defined escape terminology, we propose the following change:

1.  When encoded, strings must have `@` (U+0040, COMMERCIAL AT) in pairs.

1.  Two adjacent `@` represent a literal `@`; thus `person@example.com` would be encoded as `person@@example.com`

1.  If other characters are present between a pair of `@`, they have special meaning.
    Two such special meanings are defined in this specification;
    other uses may be defined by extensions to this specification.
    
    1.  Pointers are encoded as `@identifier@`.
        Payloads that are not pointers MUST NOT begin with a valid pointer.
        
        To support limited-look-ahead lex-free parsers,
        payloads that are not pointers SHOULD NOT begin with an `@` followed by a valid first character of a pointer (i.e., `[A-Za-z0-9_]`).
    
    2.  The `DATE` tag payload may begin with a calender identifier such as `@#DROMAN@`

1.  When encountering an `@` outside of the above cases, implementations SHOULD treat any `@[^@]+@` as equivalent to the empty string.

This change is intended to

1.  Remove the ambiguity of what an `@` followed by something other than a `#` or an `@` means
1.  Remove the ambiguity of when escapes are allowed and what they mean
1.  More clearly give room for extensions to define other `@`-delimited special meanings

### Pointers as continuations

The following passage in the GEDCOM specification

> Logical GEDCOM record sizes should be constrained so that they will fit in a memory buffer of less than 32K. GEDCOM files with records sizes greater than 32K run the risk of not being able to be loaded in some programs.  Use of pointers to records, particularly NOTE records, should ensure that this limit will be sufficient.

can be taken, particularly in concert with the OBJE chaining in GEDCOM 5.5 (but not 5.5.1) to suggest that oversized records can be created by breaking them into chunks that chain from one to another.
However, it does not unambiguously assert that such chaining is permitted.

Due to complexity in defining the meaning of chained records,
particularly as relates to substructures but also as relates to semantic ambiguity (e.g., is a `NOTE` inside a `NOTE` a meta-note *about* the note or a split continuation of the note?),
we propose that split-and-chain is not supported except where explicitly supported by OBJE.
We are unaware of any implementations that this decision breaks.



## Only point to records

GEDCOM gives the ability to point to substructures using `!` and to external files using `:`,
but neither is fully defined nor used by any system of which we are aware.
We thus propose replacing these with the following:

1.  While any structure can have an ID, only the IDs of records carry meaning.
    Implementations MAY ignore other IDs, though preserving them if present is RECOMMENDED.

1.  Pointers that contain either `!` or `:` have undefined semantics.




## The `OBJE` mess

GEDCOM 5.5.1 changed the meaning of `OBJE` in several ways, making 5.5.1 and 5.5 incompatible in this regard.
In addition, both versions contained unusual quirks, such as the inconsistent set of places a `SOUR` may contain an `OBJE` substructure -- these are not progress blockers, but are hard to defend and difficult to document clearly.

It is not clear to us that either 5.5 or 5.5.1 usage of this tag predominates to a sufficient degree not to mention the other usage.
This suggests the need to document a more permissive hybrid syntax that encompasses both.
