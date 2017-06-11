---
title: "Character encoding"
...
# Character encoding

In order to parse an ELF document, an application must determine how 
to map the raw stream of octets read from the network or disk into 
characters.  This is mapping is called the **character encoding** of 
the document.  Determining it is a two-stage process, with the first 
stage being to determine a **detected character encoding**.

If a character encoding is specified via any supported external 
means, such as an HTTP header, this *shall* be the *detected 
character encoding*.  

Otherwise, if the document begins with a byte-order mark (U+FEFF) 
encoded in UTF-8 or UTF-16 (of either endianness), this determines 
the *detected character encoding*.  The byte-order mark is removed 
from the data stream before further processing.

Otherwise, if the first octets of the data stream are 30 00 or 00 30, 
the *detected character encoding* is UTF-16, in little-endian or 
big-endian form, respectively.

The second stage is to parse the initial portion of the document to 
look for a `CHAR` tag.  If the first stage resulted in a *detected 
character encoding*, it during this stage of parsing; otherwise an 
unspecified ASCII-compatible character encoding is used.  The 
document is parsed into *lines*, stopping at the second instance of a 
*line* with *level* 0.   If a *line* with *level* 1 and *tag* `CHAR` 
was found, its *payload* is the **specified character encoding** of 
the document.

{.note} The unspecified ASCII-compatible character encoding may be 
ANSEL, providing the implementation does not give errors or warnings 
if reserved characters are used.

The *specified character encoding* *shall* be one of the following 
values:

------    --------------------------------------------------------------
`ASCII`   The US version of ASCII defined in [ASCII].

`ANSEL`   The extended Latin character set for bibliographic use defined
          in [ANSEL].

`UNICODE` Either the UTF-16LE or the UTF-16BE encodings of Unicode
          defined in [ISO 10646].

`UTF-8`   The UTF-8 encodings of Unicode defined in [ISO 10646].
------    --------------------------------------------------------------

If this algorithm produces both a *detected character encoding* and a
*specified character encoding*, they *shall* refer to the same *character
encoding* and this *character encoding* shall be used when parsing the
document.  Otherwise, if there is either a *detected character encoding*
or a *specified character encoding*, it shall be used to parse the
document.  If there is neither, the *character encoding* shall default
to ANSEL.

## References

[ANSEL]
:   NISO (National Information Standards Organization).  *ANSI/NISO
    Z39.47-1993.  Extended Latin Alphabet Coded Character Set for
    Bibliographic Use*.  1993.

[ASCII]
:   ANSI (American National Standards Institute).  *ANSI X3.4-1986.
    Coded Character Sets -- 7-Bit American National Standard Code
    for Information Interchange (7-Bit ASCII)*.  1986.

[ISO 10646]
:   ISO (International Organization for Standardization).  *ISO/IEC
    10646:2014. Information technology &mdash; Universal Coded Character
    Set (UCS).*  2014.

