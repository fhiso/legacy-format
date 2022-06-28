Notes on mappings in languages.tsv
==================================

**Cantonese**.   The preferred code is `yue`, which is new addition from
[ISO 639-3].  Before [ISO 639-3] was published (in 2007) and [RFC 5646]
was published (in 2008) to allow them to be used in language tags,
Cantonese was regarded as a dialect of Chinese and encoded `zh-yue`;
`yue` is registered in [IANA Lang Subtag] as an *extlang* tag for this
purpose.  Because historically not all systems have supported *extlang*
tags and because Cantonese is the main language spoken in Hong Kong,
`zh-HK` has been used to represent Cantonese, even though this properly
means any Chinese dialect localised for Hong Kong.

**Catalan_Spn**.  Presumably this represents the Catalan language
(`ca`), as spoken in Spain, though [GEDCOM] is silent on this point.
The preferred code is therefore `ca-ES`.  Why Catalan should be singled
out as the only language to be given a regional variant is unclear, as
Catalan is not widely spoken outside Spain.  Perhaps the GEDCOM authors
considered Occitan (`oc`) and dialects of it such as Gascon and
Provençal, all of which are spoken in Southern France, to be forms of
Catalan.

**Mandarin**.  The preferred code is `cmn`, which like `yue` for
Cantonese is a new addition from [ISO 639-3].  Previously it was encoded
`zh-cmn`, where `cmn` is registered in [IANA Lang Subtag] as an
*extlang* tag for this purpose, and `zh` refers to the family of Chinese
dialects or languages.  However because Mandarin is the most widely
spoken Chinese language, as well as the official language of China, `zh`
is often used to refer specifically to Mandarin Chinese.

**Norwegian**.  The preferred code is `no`, which is the code for the
Norwegian macrolanguage encompassing the two standard written forms of
Norwegian in common use today, Bokmål (`nb`) and Nynorsk (`nn`).
Neither is regarded as the official form of Norwegian, but Bokmål is
used by considerably more people people than Nynorsk.

**Pahari**.  This is a group of around 20 languages spoken in Nepal and
northern India.  The most widely spoken one is Nepali which has is
assigned its own language code in [GEDCOM].  Once Nepali speakers are
excluded, the majority of non-Nepali Pahari speakers speak one of the
Himachali languages, which collectively have the [ISO 693-5] code `him`,
which is the preferred code; however there are also significant numbers
of speakers of Kumaoni (`kfy`) and Garhwali (`gbm`), which are Pahari
languages that are not in the Himachali group.

**Serbo_Croa**.  Serbo-Croat formerly had the language code `sh` in [ISO
639-1].  After the break-up of Yugoslavia, this was removed and replaced
with three separate codes: `sr` for Serbian, `hr` for Croat and `bs` for
Bosnian.  Although `sh` no longer appears in [ISO 639-1], it does appear
in [IANA Lang Subtag] which does not remove old codes, and is therefore
the preferred code.
