# Ambiguities in the GEDCOM 5.5 spec

1.  Structures with primitive elements fall into several categories:
    
    1.   Those with full-line character length and explicit listing of `CONT` and `CONC` substructures allowing greater length (`NOTE`, `TEXT`, `SOUR`, `SOUR.TITL`, ...).
    2.   Those with full-line character length but no mention of `CONC`/`CONT` (`DESC`, `EDUC`, `PROP`, `OBJE.TITL`, ...).
    3.   Those with shorter length limits but free-text content.
    4.   Those with enumerated values and length limits fitting the longest enumeration.
    
    This informs the following questions:
    
    -   May structures in category 2 have `CONC`/`CONT` substructures or not?
        
        {.ednote} Luther: I think we should allow this
    
    -   Are enumerative sets extensible?  If so, does the length limit need to be followed?

        {.ednote} Luther: I think they are extensible, and the length can be exceeded by extensions
    
    -   What is the meaning of a primitive that exceeds the listed length limit, such as an 11-character postal code or an 100-character copyright notice?

        {.ednote} Luther: I think we define that to be legal; all length limits of primitives are redefined to be suggestions, not limitations, and `CONC`/`CONT` allowed under any structure with a .  But I realize this could break some implementations.

2.  `OBJE` (only) is listed as being extensible past the 32K-per-record encoding limit by including a `+1 OBJE @<XREF:OBJE>@` pointer to the next continuation of the structure.

    This informs the following questions:
    
    -   Can similar mechanisms be used to extend other records, to permit having long `NOTE` or an `INDI` with many details?
        
        {.ednote} Luther: I think so.  I think I've seen GEDCOM files in the wild that did this with `NOTE` records, though I don't have one on hand.
    
    -   Do the continuation records repeat required substructures? (for `OBJE`, the only required substructure is `FORM`)
        If so, what is meant if the repeated substructure is not indentical to the original?
        If not, does it matter which records in the chain contain the required (and optional) substructures?
 
        {.ednote} Luther: My gut is "they MUST be in the first record, and MAY be repeated in later records, but MAY NOT be repeated with different values in later records".  But I can also see a case for "They appear only once, somewhere," as it makes it possible to have oversized required substructures and/or more optional substructures than can fit within a single record.  I have not looked to see what existing systems do.
    
    -   Suppose we have 
        
        ````gedcom
        0 @O1@ OBJE
        1 OBJE @O2@
        0 @O2@ OBJE
        1 @n@ NOTE this is a note
        ````
        
        Which of the following points to the `NOTE` substructure of `@O2@`: `@O1!n@` or `@O2!n@`?
        
        {.ednote} assuming we allow substructure pointers (see the next item in this list of ambiguities), our answer to this question dictates if reorganizing the record split requires searching for pointers to update (in the `@O2!n@` case) or if figuring out the proper pointer to a substructure requires searching for the first record in the continuation linked list (in the `@O1!n@` case).  Neither is particularly appealing, but I prefer `@O1!n@` because it lets more of the standard ignore the existence of the 32K-character record size limit.
 
3.  There is explicit syntax for pointers to substructures (using `!`).
    However, the grammar does not include identifiers for any structure that is not a record,
    and the at no point does the grammar include any record as a substructure.
    
    Does this mean 
    
    -   substructure pointers are unfinished and cannot be used?
        
        {.ednote} Luther: I'm having trouble envisioning a use case for substructure pointers, but don't see a reason to forbid them…
    
    -   identifiers may be added for non-record structures?

        {.ednote} Luther: I think we allow this
    
    -   the substructure pointer feature assumes that record structures may be used as substructures outside of the grammar's constraints?

        {.ednote} Luther: I doubt this was the intent…
    

4.  The grammar lists sets of permitted substructures under each structure.
    However, it is ambiguous if omitted substructures are forbidden or not.
    
    -   Can existing substructure types, 
        such as the `note_structure` version of `NOTE`,
        be added in places not specified in the grammar?

        {.ednote} Luther: I vote "Yes", but that implementations MAY ignore all such non-documented substructures and still claim standards compliance.
    
    -   Can existing record types, such as the `note_record` version of NOTE, be added as a substructure somewhere?

        {.ednote} Luther: I vote "No", but have not thought deeply on the topic
    
    -   Can user-defined tags such as `_UUID` (which are permitted by the standard)
        be added to the grammar anywhere without violating the standard?

        {.ednote} Luther: I think this is implied to be permitted, and hence there is an implication that the grammar is sufficient but not complete and that other substructures are allowed generally.

5.  The grammar states 
    
    ````
    n  REPO @<XREF:REPO>@ {1:1}
      +1 <<NOTE_STRUCTURE>>  {0:M}
      +1 CALN <SOURCE_CALL_NUMBER>  {0:M}
        +2 MEDI <SOURCE_MEDIA_TYPE>  {0:1}
    ````
    
    But the example included in the specification is
    
    ````gedcom
    1 REPO   @7@
      2 CALN   13B-1234.01
      2 MEDI   Microfilm
    ````
    
    Are `MEDI` tags subordinate to `REPO` or `CALN`?

# Other concerns with the spec

## The `{0:3}` cardinality

Every cardinality in the grammar is one of `{0:1}`, `{1:1}`, `{0:M}`, `{1:M}`, or `{0:3}`.
The `{0:3}` doesn't seem to me to logically belong in this set,
nor to belong in either place it appears (it is used for `.SUBM.LANG` and for `PHON`, neither of which is intrinsically limited to 3).

{.ednote} Luther: I suggest we change all `{0:3}` to `{0:M}`

## LDS-specific material

The spec contains some LDS-specific material; not only that, but some of that material is obsolete (such as the `PRE_1970` value for the `STAT` structure) or underdefined given FamilySearch's abandonment of GEDCOM (such as the `QUALIFIED` value of the `STAT` structure).

The LDS-specific subset of GEDCOM does not seem to belong in a FHISO standard, being better handled via an extension.
But to remove them breaks full compatibility…

{.ednote} We could say something like "A tag without an underscore is either documented in this standard OR is part of the LDS Legacy Descriptor Set".

{.ednote} We could say that conforming implementations MUST NOT generate undocumented non-underscored tags, but also MUST NOT crash if they are present; omit to document LDS-specific tags; and define valid GEDCOM 5.5 containing those tags to contain undocumented non-underscored extensions under the ELF standard.

# Structure generality and string length

For the logical model, it would be nice to be able to say that there are two kinds of structures: those containing one pointer and those containing a string of potentially unbounded length.
To do that, we need to combine the GEDCOM ideas of `LINE_ITEM`s and the `CONT` and `CONC` tags, as well as allow any tag to be chained to exceed 32K by containing a pointer-valued substructure of its own type (like `OBJE` can in the spec already).
This change would decouple the representation from the logical model and make it simpler to define XML and JSON serializations of ELF because (almost all of the) size limits would be either suggestions (like the 10-character limit on postal codes) or could be deferred to the serialization format binding.

Example of a serialization format binding limitations that cannot be bypassed in this way is the inability to have very long escape strings or runs of more than 248 consecutive spaces inside a string.

The chaining of substructures is not clear to me.
OBJE handles this by chaining the root record and asserting that same-tag substructures of the chained records are concatenated,
but that is not generalizable to potentially-long `{0:M}`-cardinality substructures.

Consider the following options:

1.  Any type of structure may be chained to continue on the top level

    ````gedcom
    0 @I1@ INDI
      1 DSCR a wizened old man w
        2 DATE 1850
        2 DSCR @D1@
      1 DSCR a spry young lad y
        2 DATE 1785
        2 DSCR @D2@
    0 @D1@ DECR ith no teeth
    0 @D2@ DECR et to hit puberty
    ````

2.  Only top-level record s may be continued; 
    the first same-tag substructures of each chain element 
    are `CONC`atentated with the last tag of that name in the previous record
    
    ````gedcom
    0 @I1@ INDI
      1 DSCR a wizened old man w
        2 DATE 1850
      1 INDI @I2@
    0 @I2@ INDI
      1 DECR ith no teeth
      1 DSCR a spry young lad y
        2 DATE 1785
      1 INDI @I3@
    0 @I3@ INDI
      1 @D2@ DECR et to hit puberty
    ````
    
3.  Only top-level record s may be continued; 
    if subsubstructures wish to be continued, they must point to the sub-elements of the continuation records.

    ````gedcom
    0 @I1@ INDI
      1 DSCR a wizened old man w
        2 DATE 1850
        2 DSCR @I2!d@
      1 DSCR a spry young lad y
        2 DATE 1785
        2 DSCR @I3!d@
      1 INDI @I2@
    0 @I2@ INDI
      1 @d@ DECR ith no teeth
      1 INDI @I3@
    0 @I3@ INDI
      1 @d@ DECR et to hit puberty
    ````

None of these are very pretty; perhaps we limit all substructures to be small enough we never need to worry about this?




