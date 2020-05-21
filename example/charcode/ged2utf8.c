/**
 * This code is an implementation of the character encoding detection 
 * algorithm outlined by FHISO in the December 2019 draft ELF Serialisation.
 * In particular, it recodes ANSEL and UTF-16 as UTF-8. It also handles 
 * UTF-32 and if given actual 7-bit ASCII, handles it too, though it does
 * not check for invalid ASCII bytes. Currently it make no effort at 
 * handling ANSI (e.g., CP-1252 and so on), though extending it to do so 
 * should be relatively straightforward.
 * 
 * The code also makes a few other cleanups, mostly related to spurious 
 * whitespace. It allow horizontal tabs most places GEDCOM allows spaces
 * and converts them to spaces; it also strips excess blank lines and
 * spaces outside of line payloads. It turns empty payloads (e.g. "1 NOTE ") 
 * into missing payloads (e.g. "1 NOTE").
 * 
 * All code in this file was written by Luther Tychonievich without
 * consulting any code, pseudo-code, or algorithm except the ELF
 * Serialisation draft document.
 * 
 * 2020-05-09: Initial version
 * 2020-05-10: fixed leading whitespace handling; moved CHAR to second line
 * 
 * This code is knowing and willfully released to the public domain by 
 * its author and may be used in whole or in part, with or without
 * attribution, for any purpose without requiring any additional permission,
 * payment, notification, or other action.
 */


#include "ansel2utf8.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

static size_t sb_cap, sb_size;
static char *sb_data;
/** codepoint-at-a-time string builder (uses global, not threadsafe) */
void sb_push(int codepoint) {
    if (!sb_data || !sb_cap) { sb_cap = 4; sb_data = realloc(sb_data, sb_cap); }
    if (sb_size+4 > sb_cap) { sb_cap *= 2; sb_data = realloc(sb_data, sb_cap); }
    sb_size += codepoint_to_utf8(codepoint, sb_data + sb_size);
}
/** codepoint-at-a-time string builder (uses global, not threadsafe) */
char *sb_finish() {
    if (!sb_data) return NULL;
    char *ans = realloc(sb_data, sb_size + 1);
    ans[sb_size] = 0;
    sb_data = NULL;
    sb_size = sb_cap = 0;
    return ans;
}

typedef struct {
    int level;
    char *tag, *xref, *payload;
} GedLine;
void free_GedLine(GedLine *line) {
    if (!line) return;
    if (line->tag) { free(line->tag); line->tag = NULL; }
    if (line->xref) { free(line->xref); line->xref = NULL; }
    if (line->payload) { free(line->payload); line->payload = NULL; }
    line->level = 0;
}

typedef enum {
    GAP0, LEVEL, GAP1, XREF, GAP2, TAG, GAP3, PAYLOAD
} _BuildStage;
_BuildStage gb_stage;

/**
 * Feeds a unicode code point into a GedLine, returning 1 if more characters needed, 0 if not, and -1 on error.
 */
int feed(int codepoint, GedLine *line) {
    switch(gb_stage) {
        case GAP0:
            if (codepoint == 0xFEFF || isspace(codepoint)) return 1;
            gb_stage = LEVEL;
            // reset all fields before starting
            free_GedLine(line);
            if (sb_data) { sb_data = NULL; sb_size = sb_cap = 0; }
            // fallthrough
        case LEVEL:
            if (isdigit(codepoint)) {
                line->level *= 10;
                line->level += codepoint-'0';
                return 1;
            }
            if (!isspace(codepoint)) return -1;
            gb_stage = GAP1;
            // fallthrough
        case GAP1:
            if (codepoint == '\n' || codepoint == '\r') return -1;
            if (isspace(codepoint)) return 1;
            if (codepoint == '@') {
                gb_stage = XREF;
            } else {
                gb_stage = TAG;
                sb_push(codepoint);
            }
            return 1;
        case XREF:
            if (codepoint == '\n' || codepoint == '\r') return -1;
            if (codepoint == '@') {
                gb_stage = GAP2;
                line->xref = sb_finish();
                return 1;
            }
            sb_push(codepoint);
            return 1;
        case GAP2:
            if (codepoint == '\n' || codepoint == '\r') return 0;
            if (isspace(codepoint)) return 1;
            gb_stage = TAG;
            // fallthrough
        case TAG:
            if (isspace(codepoint)) {
                line->tag = sb_finish();
                gb_stage = PAYLOAD; // or GAP3 to strip leading whitespace
                if (codepoint == '\n' || codepoint == '\r') return 0;
                return 1;
            }
            sb_push(codepoint);
            return 1;
        case GAP3: // Unused; if enabled, strips leading whitespace
            if (codepoint == '\n' || codepoint == '\r') return 0;
            if (isspace(codepoint)) return 1;
            gb_stage = PAYLOAD;
            // fallthrough
        case PAYLOAD:
            if (codepoint == '\n' || codepoint == '\r') {
                line->payload = sb_finish(); // treats empty payload and missing payload the same (both as missing)
                return 0;
            }
            sb_push(codepoint);
            return 1;
    }
    return -2;
}

void fPutLine(FILE *out, GedLine *line) {
    fprintf(out, "%d ", line->level);
    if (line->xref) fprintf(out, "@%s@ ", line->xref);
    fprintf(out, "%s", line->tag);
    if (line->payload) fprintf(out, " %s", line->payload);
    fprintf(out, "\n"); // change to \r or \r\n for other line ending options
}

FILE *_istream;
int readFrom_iStream() {
    return fgetc(_istream);
}

/**
 * Reads one GedLine, mallocing as needed
 */
GedLine readLine(FILE *f, int(*decoder)(int (*)())) {
    GedLine ans = {0,0,0,0};
    gb_stage = GAP0;
    _istream = f;

    int cp, again;
    do {
        if (decoder) cp = decoder(readFrom_iStream);
        else cp = readFrom_iStream();
        if (cp < 0) break;
        again = feed(cp, &ans);
    } while (again > 0);

    return ans;
}

typedef enum { NONE, ANSEL, UTF8, UTF16LE, UTF16BE, UTF32LE, UTF32BE, ASCII } Codec;
const char *codec_names[] = {
    "Unknown",
    "ANSEL",
    "UTF-8",
    "UTF-16 little-endian",
    "UTF-16 big-endian",
    "UTF-32 little-endian",
    "UTF-32 big-endian",
    "ASCII (note: incomplete implementation)",
};
int (*decoders[])(int (*)()) = {
    NULL,
    ansel_next_codepoint,
    utf8_next_codepoint,
    utf16le_next_codepoint,
    utf16be_next_codepoint,
    utf32le_next_codepoint,
    utf32be_next_codepoint,
    NULL,
};


int main(int argc, char *argv[]) {
    if ((argc > 1 && argv[1][0] == '-') || (argc > 2 && argv[2][0] == '-')) {
        fprintf(stderr, "USAGE:\n"
            "%s\n"
            "    reads from stdin, writes to stdout\n"
            "%s infile.ged\n"
            "    reads from infile.ged, writes to stdout\n"
            "%s infile.ged outfile.ged\n"
            "    reads from infile.ged, creates and writes to outfile.ged\n", 
            argv[0],argv[0],argv[0]);
        return 1;
    }
    
    FILE *in = stdin;
    if (argc > 1) in = fopen(argv[1], "rb");
    if (!in) {
        fprintf(stderr, "ERROR: unable to open %s\n", argv[1]);
        return 2;
    }
    
    FILE *out = stdout;
    if (argc > 2) out = fopen(argv[2], "wxb");
    if (!out) {
        fprintf(stderr, "ERROR: unable to create %s\n", argv[2]);
        return 3;
    }
    
    // detected character encoding based on first 4 bytes
    Codec dce = NONE;
    unsigned char check[4];
    int bom = 0;
    if (fread(check, 1, 4, in) != 4) { 
        fprintf(stderr, "ERROR: empty file %s\n", argv[1]);
        return 4;
    }
    if (check[0] == 0xef && check[1] == 0xbb && check[2] == 0xbf) {
        dce = UTF8;
        bom = 3;
    } else if (check[0] == 0xFF && check[1] == 0xFE) {
        dce = (check[2] || check[3]) ? UTF16LE : UTF32LE;
        bom = (check[2] || check[3]) ? 2 : 4;
    } else if (check[0] == 0xFE && check[1] == 0xFF) {
        dce = UTF16BE;
        bom = 2;
    } else if (!check[0] && !check[1] && check[2] == 0xFF && check[3] == 0xFE) {
        dce = UTF32BE;
        bom = 4;
    } else if (check[0] && !check[1] && !check[2] && !check[3]) {
        dce = UTF32LE;
        bom = 0;
    } else if (!check[0] && !check[1] && !check[2] && check[3]) {
        dce = UTF32BE;
        bom = 0;
    } else if (check[0] && !check[1]) {
        dce = UTF16LE;
        bom = 0;
    } else if (!check[0] && check[1]) {
        dce = UTF16BE;
        bom = 0;
    }
    fprintf(stderr, "Detected character encoding: %s (%s BOM)\n", codec_names[dce], bom ? "with" : "without");
    
    // use detected character encoding to look for CHAR tag in HEAD
    Codec sce = NONE;
    fseek(in, bom, SEEK_SET);
    GedLine gl = readLine(in, decoders[dce]);
    for(int i=0; (gl.tag && (i==0 || gl.level > 0)); i+=1) {
        if (gl.level == 1 && strcasecmp("CHAR", gl.tag) == 0) {
            if (strcasecmp(gl.payload, "UTF-8") == 0) {
                sce = UTF8;
            } else if (strcasecmp(gl.payload, "ASCII") == 0) {
                sce = ASCII;
            } else if (strcasecmp(gl.payload, "ANSEL") == 0) {
                sce = ANSEL;
            } else if (strcasecmp(gl.payload, "UNICODE") == 0) {
                if (dce == UTF16LE || dce == UTF16BE) sce = dce;
            } else {
                fprintf(stderr,"Unsupported character encoding:\n    ");
                fPutLine(stderr, &gl);
                return 5;
            }
            break;
        }
        free_GedLine(&gl);
        gl = readLine(in, decoders[dce]);
    }
    free_GedLine(&gl);
    fprintf(stderr, "Specified character encoding: %s\n", codec_names[sce]);
    
    Codec ce = sce != NONE ? sce : (dce != NONE ? dce : UTF8);
    fprintf(stderr, "Parsing with character encoding: %s\n", codec_names[ce]);
    
    // Uncomment the following to put a byte-order mark in the output
    // fprintf(out, "\xef\xbb\xbf"); // UTF-8 byte order mark
    
    int inHead = 0;
    fseek(in, bom, SEEK_SET);
    gl = readLine(in, decoders[ce]);
    while(gl.tag) {
        // remove CHAR from HEAD (will be replaced with UTF-8 instead)
        if (inHead && gl.level == 1 && strcasecmp(gl.tag, "CHAR") == 0) {
            do {
                free_GedLine(&gl);
                gl = readLine(in, decoders[ce]);
            } while (gl.level > 1);
        }
        
        // print each line
        fPutLine(out, &gl);
        
        // track if in the header, and if so add "1 CHAR UTF-8"
        if (gl.level == 0) {
            inHead = strcasecmp(gl.tag, "HEAD") == 0;
            if (inHead) {
                GedLine utf8 = {1, "CHAR", NULL, "UTF-8"};
                fPutLine(out, &utf8);
            }
        }
        
        // get ready for next pass
        free_GedLine(&gl);
        gl = readLine(in, decoders[ce]);
    }
    free_GedLine(&gl);

    return 0;
}
