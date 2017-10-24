The following provides the complete information needed to convert GEDCOM-extension ANSEL into Unicode code points.
Each ANSEL byte converts into a single code point in Unicode.
For those bytes marked "Swap with next", the byte and it's subsequent byte are reversed in the codepoint sequence;
for example the byte sequence

    64      65      EA      66      67

is converted to code point sequence 
    
    U+0064  U+0065  U+0066  U+030A  U+0067

and is rendered as

    def̊g

Bytes 00 through 7F map to themselves as code points.
Bytes 80 through A0, AF, BB, C9 through CC, D0 through DF, FD, and FF will never appear in any valid ANSEL encoding.
All of these appear in the following table for completeness.


ANSEL  |Unicode|Swap?         |Version note           |Extension
-------|-------|--------------|-----------------------|------------------------
00     |U+0000 |              |                       |
01     |U+0001 |              |                       |
02     |U+0002 |              |                       |
03     |U+0003 |              |                       |
04     |U+0004 |              |                       |
05     |U+0005 |              |                       |
06     |U+0006 |              |                       |
07     |U+0007 |              |                       |
08     |U+0008 |              |                       |
09     |U+0009 |              |                       |
0A     |U+000A |              |                       |
0B     |U+000B |              |                       |
0C     |U+000C |              |                       |
0D     |U+000D |              |                       |
0E     |U+000E |              |                       |
0F     |U+000F |              |                       |
10     |U+0010 |              |                       |
11     |U+0011 |              |                       |
12     |U+0012 |              |                       |
13     |U+0013 |              |                       |
14     |U+0014 |              |                       |
15     |U+0015 |              |                       |
16     |U+0016 |              |                       |
17     |U+0017 |              |                       |
18     |U+0018 |              |                       |
19     |U+0019 |              |                       |
1A     |U+001A |              |                       |
1B     |U+001B |              |                       |
1C     |U+001C |              |                       |
1D     |U+001D |              |                       |
1E     |U+001E |              |                       |
1F     |U+001F |              |                       |
20     |U+0020 |              |                       |
21     |U+0021 |              |                       |
22     |U+0022 |              |                       |
23     |U+0023 |              |                       |
24     |U+0024 |              |                       |
25     |U+0025 |              |                       |
26     |U+0026 |              |                       |
27     |U+0027 |              |                       |
28     |U+0028 |              |                       |
29     |U+0029 |              |                       |
2A     |U+002A |              |                       |
2B     |U+002B |              |                       |
2C     |U+002C |              |                       |
2D     |U+002D |              |                       |
2E     |U+002E |              |                       |
2F     |U+002F |              |                       |
30     |U+0030 |              |                       |
31     |U+0031 |              |                       |
32     |U+0032 |              |                       |
33     |U+0033 |              |                       |
34     |U+0034 |              |                       |
35     |U+0035 |              |                       |
36     |U+0036 |              |                       |
37     |U+0037 |              |                       |
38     |U+0038 |              |                       |
39     |U+0039 |              |                       |
3A     |U+003A |              |                       |
3B     |U+003B |              |                       |
3C     |U+003C |              |                       |
3D     |U+003D |              |                       |
3E     |U+003E |              |                       |
3F     |U+003F |              |                       |
40     |U+0040 |              |                       |
41     |U+0041 |              |                       |
42     |U+0042 |              |                       |
43     |U+0043 |              |                       |
44     |U+0044 |              |                       |
45     |U+0045 |              |                       |
46     |U+0046 |              |                       |
47     |U+0047 |              |                       |
48     |U+0048 |              |                       |
49     |U+0049 |              |                       |
4A     |U+004A |              |                       |
4B     |U+004B |              |                       |
4C     |U+004C |              |                       |
4D     |U+004D |              |                       |
4E     |U+004E |              |                       |
4F     |U+004F |              |                       |
50     |U+0050 |              |                       |
51     |U+0051 |              |                       |
52     |U+0052 |              |                       |
53     |U+0053 |              |                       |
54     |U+0054 |              |                       |
55     |U+0055 |              |                       |
56     |U+0056 |              |                       |
57     |U+0057 |              |                       |
58     |U+0058 |              |                       |
59     |U+0059 |              |                       |
5A     |U+005A |              |                       |
5B     |U+005B |              |                       |
5C     |U+005C |              |                       |
5D     |U+005D |              |                       |
5E     |U+005E |              |                       |
5F     |U+005F |              |                       |
60     |U+0060 |              |                       |
61     |U+0061 |              |                       |
62     |U+0062 |              |                       |
63     |U+0063 |              |                       |
64     |U+0064 |              |                       |
65     |U+0065 |              |                       |
66     |U+0066 |              |                       |
67     |U+0067 |              |                       |
68     |U+0068 |              |                       |
69     |U+0069 |              |                       |
6A     |U+006A |              |                       |
6B     |U+006B |              |                       |
6C     |U+006C |              |                       |
6D     |U+006D |              |                       |
6E     |U+006E |              |                       |
6F     |U+006F |              |                       |
70     |U+0070 |              |                       |
71     |U+0071 |              |                       |
72     |U+0072 |              |                       |
73     |U+0073 |              |                       |
74     |U+0074 |              |                       |
75     |U+0075 |              |                       |
76     |U+0076 |              |                       |
77     |U+0077 |              |                       |
78     |U+0078 |              |                       |
79     |U+0079 |              |                       |
7A     |U+007A |              |                       |
7B     |U+007B |              |                       |
7C     |U+007C |              |                       |
7D     |U+007D |              |                       |
7E     |U+007E |              |                       |
7F     |U+007F |              |                       |
80     |       |              |                       |
81     |       |              |                       |
82     |       |              |                       |
83     |       |              |                       |
84     |       |              |                       |
85     |       |              |                       |
86     |       |              |                       |
87     |       |              |                       |
88     |       |              |                       |
89     |       |              |                       |
8A     |       |              |                       |
8B     |       |              |                       |
8C     |       |              |                       |
8D     |       |              |                       |
8E     |       |              |                       |
8F     |       |              |                       |
90     |       |              |                       |
91     |       |              |                       |
92     |       |              |                       |
93     |       |              |                       |
94     |       |              |                       |
95     |       |              |                       |
96     |       |              |                       |
97     |       |              |                       |
98     |       |              |                       |
99     |       |              |                       |
9A     |       |              |                       |
9B     |       |              |                       |
9C     |       |              |                       |
9D     |       |              |                       |
9E     |       |              |                       |
9F     |       |              |                       |
A0     |       |              |                       |
A1     |U+0141 |              |                       |
A2     |U+00D8 |              |                       |
A3     |U+0110 |              |                       |
A4     |U+00DE |              |                       |
A5     |U+00C6 |              |                       |
A6     |U+0152 |              |                       |
A7     |U+02B9 |              | 5.5 but not 5.5.1     | 
A8     |U+00B7 |              |                       |
A9     |U+266D |              | 5.5.1 but not 5.5     | 
AA     |U+00AE |              | 5.5.1 but not 5.5     | 
AB     |U+00B1 |              | 5.5.1 but not 5.5     | 
AC     |U+01A0 |              | Not 5.5               |
AD     |U+01AF |              | Not 5.5               |
AE     |U+02BE |              |                       |
AF     |       |              |                       |
B0     |U+02BF |              |                       |
B1     |U+0142 |              |                       |
B2     |U+00F8 |              |                       |
B3     |U+0111 |              |                       |
B4     |U+00FE |              |                       |
B5     |U+00E6 |              |                       |
B6     |U+0153 |              |                       |
B7     |U+02BA |              | Not 5.5               |
B8     |U+0131 |              |                       |
B9     |U+00A3 |              |                       |
BA     |U+00F0 |              |                       |
BB     |       |              |                       |
BC     |U+01A1 |              | Not 5.5               |
BD     |U+01B0 |              | Not 5.5               |
BE     |U+25A1 |              | Not 5.5               | GEDCOM extension
BF     |U+25A0 |              | Not 5.5               | GEDCOM extension
C0     |U+00B0 |              | Not 5.5               | 
C1     |U+2113 |              | Not 5.5               | 
C2     |U+2117 |              | Not 5.5               | 
C3     |U+00A9 |              |                       |
C4     |U+2667 |              | Not 5.5               | 
C5     |U+00BF |              |                       |
C6     |U+00A1 |              |                       |
C7     |U+00DF |              | Not 5.5               | MARC 21 extension
C8     |U+20AC |              | Not 5.5               | MARC 21 extension
C9     |       |              |                       |
CA     |       |              |                       |
CB     |       |              |                       |
CC     |       |              |                       |
CD     |U+0065 |              | Not 5.5               | GEDCOM extension
CE     |U+006F |              | Not 5.5               | GEDCOM extension
CF     |U+00DF |              |                       | GEDCOM extension
D0     |       |              |                       |
D1     |       |              |                       |
D2     |       |              |                       |
D3     |       |              |                       |
D4     |       |              |                       |
D5     |       |              |                       |
D6     |       |              |                       |
D7     |       |              |                       |
D8     |       |              |                       |
D9     |       |              |                       |
DA     |       |              |                       |
DB     |       |              |                       |
DC     |       |              |                       |
DD     |       |              |                       |
DE     |       |              |                       |
DF     |       |              |                       |
E0     |U+0309 |Swap with next|                       |
E1     |U+0300 |Swap with next|                       |
E2     |U+0301 |Swap with next|                       |
E3     |U+0302 |Swap with next|                       |
E4     |U+0303 |Swap with next|                       |
E5     |U+0304 |Swap with next|                       |
E6     |U+0306 |Swap with next|                       |
E7     |U+0307 |Swap with next|                       |
E8     |U+0308 |Swap with next|                       |
E9     |U+030C |Swap with next|                       |
EA     |U+030A |Swap with next|                       |
EB     |U+FE20 |Swap with next| Not 5.5               |
EC     |U+FE21 |Swap with next| Not 5.5               |
ED     |U+0315 |Swap with next|                       |
EE     |U+030B |Swap with next|                       |
EF     |U+0310 |Swap with next| Not 5.5               |
F0     |U+0327 |Swap with next| Not 5.5               |
F1     |U+0328 |Swap with next|                       |
F2     |U+0323 |Swap with next| Not 5.5               |
F3     |U+0324 |Swap with next| Not 5.5               |
F4     |U+0325 |Swap with next| Not 5.5               |
F5     |U+0333 |Swap with next| Not 5.5               |
F6     |U+0332 |Swap with next|                       |
F7     |U+0326 |Swap with next| Not 5.5               |
F8     |U+0328 |Swap with next| Not 5.5               |
F9     |U+032E |Swap with next| Not 5.5               |
FA     |U+FE22 |Swap with next| Not 5.5               |
FB     |U+FE23 |Swap with next| Not 5.5               |
FC     |U+0338 |Swap with next| Not 5.5               | GEDCOM Extension
FD     |       |              |                       |
FE     |U+0313 |Swap with next|                       |
FF     |       |              |                       |


# Addendum: multiple combining diacritics

The above describes how to convert text where only one combining diacritic is present per letter.
Multiple accents need to be handled specially, as the stacking order of multiple diacritics is different in UTF-8 and ANSEL (or at least the MARC-8 clarification of ANSEL; ANSEL itself is silent on the topic) depending on where they appear:

ANSEL  |Unicode|Type
-------|-------|------
E0     |U+0309 |High
E1     |U+0300 |High
E2     |U+0301 |High
E3     |U+0302 |High
E4     |U+0303 |High
E5     |U+0304 |High
E6     |U+0306 |High
E7     |U+0307 |High
E8     |U+0308 |High
E9     |U+030C |High
EA     |U+030A |High
EB     |U+FE20 |High
EC     |U+FE21 |High
ED     |U+0315 |High
EE     |U+030B |High
EF     |U+0310 |High
F0     |U+0327 |Low
F1     |U+0328 |Low
F2     |U+0323 |Low
F3     |U+0324 |Low
F4     |U+0325 |Low
F5     |U+0333 |Low
F6     |U+0332 |Low
F7     |U+0326 |Low
F8     |U+0328 |Low
F9     |U+032E |Low
FA     |U+FE22 |High
FB     |U+FE23 |High
FC     |U+0338 |Center
FD     |       |
FE     |U+0313 |High
FF     |       |

Given an ANSEL byte sequence consisting of any number of bytes &ge; E0 followed by a single byte &lt; E0,
re-order that sequence as follows:

1.  Put the final byte's code point first
2.  If there is a center-type character (byte FC), put it second
3.  Then put the high-type and low-type characters, reversing the order of the high-type characters and preserving the order of the low-type characters.
    How high- and low-type characters are ordered is does not impact rendering;
    however, Unicode normalized forms prefer low-type before high-type.

Example:

    ANSEL:      E0      E9      F1      FC      E8      F7      65
    
    Reorder:    65      FC      F1      F7      E8      E9      E0
    
    Unicode:    U+0065  U+0338  U+0328  U+0326  U+0308  U+030C  U+0309
    
    Rendering:  ę̸̦̈̌̉
