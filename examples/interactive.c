#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <utf8proc.h>

#include "ident4c.h"

#ifndef __STDC_UTF_32__
#warning "Not using UTF-32"
#endif

typedef int32_t char32_t;

static bool is_unicode_printable(char32_t tgt) {
    return utf8proc_charwidth(tgt) != 0;    
}


int ceil_log10(uint64_t i) {
    if (i > 10000000000000000000ULL) return 20; // i > 2**19
    uint64_t bound = 10;
    int pow = 1;
    while (i > bound) {
        pow += 1;
        bound *= 10;
    }
    return pow;
}
int ceil_log2(uint64_t i) {
    if (i > (1ULL << 63)) return 64;
    uint64_t bound = 2;
    int pow = 1;
    while (i > bound) {
        pow += 1;
        bound *= 2;
    }
    return pow;
}
static int hex_width(uint64_t value) {
    int res = (ceil_log2(value) + 3) / 4;
    if (res < 1) res = 1;
    if (res % 2 != 0)  res += 1;
    assert(res % 2 == 0 && res >= 2);
    return res;
}

static size_t decode_codepoints(char32_t **dest, const char *text, size_t text_size);

int main(int argc, char **argv) {
    if (argc != 1) {
        fprintf(stderr, "Expected no arguments, got %d\n", argc);
        return 1;
    }

    size_t line_cap = 1024;
    char *line = malloc(line_cap);
    if (line == NULL) abort();

    size_t line_size;
    while ((line_size = getline(&line, &line_cap, stdin)) >= 0) {
        if (line_size > 0 && line[line_size - 1] == '\n') {
            line[--line_size] = '\0';
        }
        size_t codepoints_capacity = line_size;
        char32_t *codepoints = malloc(line_size * sizeof(char32_t));
        if (codepoints == NULL) abort();
        utf8proc_ssize_t num_codepoints = utf8proc_decompose(
            (const uint8_t*) line,
            line_size,
            codepoints,
            codepoints_capacity,
            0
        );
        if (num_codepoints == SIZE_MAX) {
            fprintf(stderr, "WARNING: Unable to decode codepoints\n");
            continue;
        }
        printf("Decoded %zd codepoints (bytes: %zd): [\n", num_codepoints, line_size);
        int max_codepoint_width = 1;
        char32_t max_codepoint = 0;
        for (size_t i = 0; i < num_codepoints; i++) {
            char32_t c = codepoints[i];
            size_t width = utf8proc_charwidth(c);
            if (width > max_codepoint_width) {
                max_codepoint_width = (int) width;
            }
            if (c > max_codepoint) {
                max_codepoint = c;
            }
        }
        int codepoint_max_hex_width = hex_width(max_codepoint);
        for (size_t i = 0; i < num_codepoints; i++) {
            char32_t c = codepoints[i];
            uint8_t char_buf[5];
            size_t dst_size = utf8proc_encode_char(c, char_buf);
            assert(dst_size != 0);
            char_buf[dst_size] = '\0';
            printf(
                "  %*zd: %*.*X -- %-*s -- (%-15s = %s)\n",
                ceil_log10(num_codepoints), i,
                codepoint_max_hex_width,
                hex_width(c),
                c,
                max_codepoint_width,
                is_unicode_printable(c) ? (char*) char_buf : "??",
                i == 0 ? "is_xid_start" : "is_xid_continue" ,
                (i == 0 ? ident4c_is_xid_start(c) : ident4c_is_xid_continue(c)) ? "true" : "false"
            );
        }
        printf("]\n");
        if (codepoints != NULL) free(codepoints);
    }
}