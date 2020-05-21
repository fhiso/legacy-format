/**
 * A byte-by-byte ANSEL to code point converter. Returns -1*byte if a 
 * non-ANSEL byte is encountered.
 * 
 * Uses static storage: not thread-safe, cannot be interleaved with
 * multiple input streams, and if there are more than 15 combining
 * diacritics on a single character it may drop some of them (keeps up to
 * 1 center, 16 low, and 15 high diacritics).
 */
int ansel_next_codepoint(int (*next_byte)());

/**
 * A byte-by-byte UTF-8 to code point converter. Returns -1*value if an 
 * invalid value is encountered (e.g., an invalid byte, value encoded in
 * the wrong number of bytes or outside the 0..0x10FFFF range).
 */
int utf8_next_codepoint(int (*next_byte)());

/**
 * A byte-by-byte UTF-16 to code point converter. Returns -1*short if an 
 * invalid short is encountered (e.g., a mispositioned surrogate).
 * 
 * Set the second argument `le` to 0 if big-endian, 1 if little-endian.
 */
int utf16_next_codepoint(int (*next_byte)(), int le);

/// convenience wrapper over parameterized version
int utf16le_next_codepoint(int (*next_byte)());
/// convenience wrapper over parameterized version
int utf16be_next_codepoint(int (*next_byte)());


/**
 * A byte-by-byte UTF-32 to code point converter. Returns -1*value if an 
 * invalid value is encountered (e.g., an invalid byte, or value outside
 * the 0..0x10FFFF range).
 * 
 * Set the second argument `le` to 0 if big-endian, 1 if little-endian.
 */
int utf32_next_codepoint(int (*next_byte)(), int le);

/// convenience wrapper over parameterized version
int utf32le_next_codepoint(int (*next_byte)());
/// convenience wrapper over parameterized version
int utf32be_next_codepoint(int (*next_byte)());

/**
 * Puts bytes of codepoint as UTF-8 into dest, returns the number of 
 * bytes inserted (or 0 on error). Assumes dest points to enough memory 
 * for this operation (i.e., dest[3] should be OK).
 */
int codepoint_to_utf8(int codepoint, char *dest);
