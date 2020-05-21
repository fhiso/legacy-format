/**
 * This code is an implementation of the ANSEL character encoding as 
 * defined in GEDCOM 5.5 and 5.5.1, with the diacritic ordering defined 
 * in MARC-8; and converts to Unicode code points as described in
 * FHISO's ANSEL conversion writuep.
 * 
 * It also includes decoders for UTF-8, UTF-16, and UTF-32 and a UTF-8 
 * encoder. While these are available in the standard libraries of many
 * languages, including them here provides a reference for those wishing
 * to code them themselves and makes the transcoding code fully stand-alone.
 * 
 * All code in this file was written by Luther Tychonievich without
 * consulting any other code, pseudo-code, or algorithm.
 * 
 * 2020-04-25: Initial version, including just ANSEL decoder
 * 2020-05-07: Added UTF-X decoders
 * 2002-05-09: Added codepoint_to_utf8
 * 
 * This code is knowing and willfully released to the public domain by 
 * its author and may be used in whole or in part, with or without
 * attribution, for any purpose without requiring any additional permission,
 * payment, notification, or other action.
 */

#include "ansel2utf8.h"


/**
 * A byte-by-byte ANSEL to code point converter. Returns -1*byte if a 
 * non-ANSEL byte is encountered.
 * 
 * Uses static storage: not thread-safe, cannot be interleaved with
 * multiple input streams, and if there are more than 15 combining
 * diacritics on a single character it may drop some of them (keeps up to
 * 1 center, 16 low, and 15 high diacritics).
 */
int ansel_next_codepoint(int (*next_byte)()) {
    static int special[] = { // code points of ANSEL bytes from 0xA1 through 0xFF
        0x141, 0xD8, 0x110, 0xDE, 0xC6, 0x152, 0x2B9, 
        0xB7, 0x266D, 0xAE, 0xB1, 0x1A0, 0x1AF, 0x2BE, -0xAF, 
        0x2BF, 0x142, 0xF8, 0x111, 0xFE, 0xE6, 0x153, 0x2BA, 
        0x131, 0xA3, 0xF0, -0xBB, 0x1A1, 0x1B0, 0x25A1, 0x25A0, 
        0xB0, 0x2113, 0x2117, 0xA9, 0x2667, 0xBF, 0xA1, 0xDF, 
        0x20AC, -0xC9, -0xCA, -0xCB, -0xCC, 0x65, 0x6F, 0xDF, 
        -0xD0, -0xD1, -0xD2, -0xD3, -0xD4, -0xD5, -0xD6, -0xD7,
        -0xD8, -0xD9, -0xDA, -0xDB, -0xDC, -0xDD, -0xDE, -0xDF, 
        0x309, 0x300, 0x301, 0x302, 0x303, 0x304, 0x306, 0x307, 
        0x308, 0x30C, 0x30A, 0xFE20, 0xFE21, 0x315, 0x30B, 0x310, 
        0x327, 0x328, 0x323, 0x324, 0x325, 0x333, 0x332, 0x326, 
        0x328, 0x32E, 0xFE22, 0xFE23, 0x338, -0xFD, 0x313, -0xFF
    };
    static int high[16]; static int hc1=0; static int hc2=0; // circular queue
    static int low[16]; static int lc=0; // stack
    static int mid=0; // 0 (none) or 0x388 (combining long solidus overlay)

    if (mid) { int tmp = mid; mid = 0; return tmp; }
    if (lc) { return low[--lc]; }
    if (hc1 != hc2) { int tmp = high[hc2]; hc2 = (hc2+1)&0xF; return tmp; }
    
    int b = next_byte();
    if (b < 0) return b; // EOF and other read errors
    if (b > 0xFF) return -b; // larger than a byte? Should be impossible
    if (b < 0x80) return b; // ASCII
    if (b < 0xA1) return -b; // unmapped by every known ANSEL variant
    if (b < 0xE0 || special[b-0xA1] < 0) // single glyph or unmapped
        return special[b-0xA1];
    
    // combining: get the answer first, then push diacritics into queue
    int ans = ansel_next_codepoint(next_byte);
    
    if (b == 0xFC) // center (only one in ANSEL)
        mid = special[b-0xA1]; // if several only keeps one
    else if (b >= 0xF0 && b <= 0xF9) { // low
        if (lc < 16) // drop 17th and beyond
            low[lc++] = special[b-0xA1];
    } else { // high
        if (((hc1+1)&0xF) != hc2) { // drop 16th and beyond
            high[hc1++] = special[b-0xA1]; hc1&=0xF;
        }
    }
    
    return ans;
}

/**
 * A byte-by-byte UTF-8 to code point converter. Returns -1*value if an 
 * invalid value is encountered (e.g., an invalid byte, value encoded in
 * the wrong number of bytes or outside the 0..0x10FFFF range).
 */
int utf8_next_codepoint(int (*next_byte)()) {
    int b = next_byte();
    if (b < 0x80) return b;
    if (b < 0xC0 || b >= 0xF8) return -b; // invalid leader

    int more = (b >= 0xC0) + (b >= 0xE0) + (b >= 0xF0);
    int ans = b & ((1<<(7-more))-1);

    for(int i=0; i<more; i+=1) {
        b = next_byte();
        if (b < 0x80 || b >= 0xC0) return -b;
        ans = (ans<<6) | (b&0x3F);
    }
    // disallow too many bytes
    if (more >= 4 && ans < (1<<16)) return -ans;
    if (more >= 3 && ans < (1<<12)) return -ans;
    if (more >= 2 && ans < (1<<7)) return -ans;

    // disallow things UTF-16 cannot handle
    if (ans >= 0x110000 || (ans >= 0xD800 && ans < 0xE000)) return -ans;

    return ans;
}

/**
 * A byte-by-byte UTF-16 to code point converter. Returns -1*short if an 
 * invalid short is encountered (e.g., a mispositioned surrogate).
 * 
 * Set the second argument `le` to 0 if big-endian, 1 if little-endian.
 */
int utf16_next_codepoint(int (*next_byte)(), int le) {
    int b1,b2;
    if ((b1 = next_byte()) < 0) return b1;
    if ((b2 = next_byte()) < 0) return b2;
    int s1 = le ? ((b2<<8)|b1) : ((b1<<8)|b2);
    if (s1 < 0xD800 || s1 >= 0xE000) return s1;

    if (s1 >= 0xDC00) return -s1; // cannot have trailing surrogate first
    if ((b1 = next_byte()) < 0) return b1;
    if ((b2 = next_byte()) < 0) return b2;
    int s2 = le ? ((b2<<8)|b1) : ((b1<<8)|b2);
    if (s2 < 0xDC00 || s2 >= 0xE000) return -s2; // must be trailing surrogate

    return (((s1&0x3FF)<<10) | (s2&0x3FF)) + 0x10000;
}


/**
 * A byte-by-byte UTF-32 to code point converter. Returns -1*value if an 
 * invalid value is encountered (e.g., an invalid byte, or value outside
 * the 0..0x10FFFF range).
 * 
 * Set the second argument `le` to 0 if big-endian, 1 if little-endian.
 */
int utf32_next_codepoint(int (*next_byte)(), int le) {
    int b1,b2,b3,b4;
    if ((b1 = next_byte()) < 0) return b1;
    if ((b2 = next_byte()) < 0) return b2;
    if ((b3 = next_byte()) < 0) return b3;
    if ((b4 = next_byte()) < 0) return b4;
    int ans = le ? ((b4<<24)|(b3<<16)|(b2<<8)|b1) 
                 : ((b1<<24)|(b2<<16)|(b3<<8)|b4);

    // disallow things UTF-16 cannot handle
    if (ans >= 0x110000 || (ans >= 0xD800 && ans < 0xE000)) return -ans;

    return ans;
}

/// convenience wrapper over parameterized version
int utf32le_next_codepoint(int (*next_byte)())
{ return utf32_next_codepoint(next_byte, 1); }
/// convenience wrapper over parameterized version
int utf32be_next_codepoint(int (*next_byte)())
{ return utf32_next_codepoint(next_byte, 0); }
/// convenience wrapper over parameterized version
int utf16le_next_codepoint(int (*next_byte)())
{ return utf16_next_codepoint(next_byte, 1); }
/// convenience wrapper over parameterized version
int utf16be_next_codepoint(int (*next_byte)())
{ return utf16_next_codepoint(next_byte, 0); }


/**
 * Puts bytes of codepoint as UTF-8 into dest, returns the number of 
 * bytes inserted (or 0 on error). Assumes dest points to enough memory 
 * for this operation (i.e., dest[3] should be OK).
 */
int codepoint_to_utf8(int codepoint, char *dest) {
    if (codepoint < 0) return 0;
    if (codepoint < (1<<7)) { *dest = codepoint; return 1; }
    if (codepoint < (1<<11)) {
        dest[0] = (codepoint>>6)|0xC0;
        dest[1] = (codepoint&0x3F)|0x80;
        return 2;
    }
    if (codepoint < (1<<16)) {
        dest[0] = (codepoint>>12)|0xE0;
        dest[1] = ((codepoint>>6)&0x3F)|0x80;
        dest[2] = (codepoint&0x3F)|0x80;
        return 3;
    }
    if (codepoint < (1<<21)) {
        dest[0] = (codepoint>>18)|0xF0;
        dest[1] = ((codepoint>>12)&0x3F)|0x80;
        dest[2] = ((codepoint>>6)&0x3F)|0x80;
        dest[3] = (codepoint&0x3F)|0x80;
        return 4;
    }
    return 0;
}


