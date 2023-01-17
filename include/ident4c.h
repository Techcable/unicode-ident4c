#ifndef IDENT4C_H
#define IDENT4C_H

/**
 * Implementation of [Unicode Standard Annex #31][tr31] for determining which
 * `char` values are valid in programming language identifiers.
 *
 * [tr31]: https://www.unicode.org/reports/tr31/
 *
 * This is a port of the rust crate `unicode-ident` by @dtolnay
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdalign.h>

#include <assert.h>

/* Tables */
extern const alignas(64) uint8_t _IDENT4C_ASCII_START[128];

extern const alignas(64) uint8_t _IDENT4C_ASCII_CONTINUE[128];

#define _IDENT4C_CHUNK 64

extern const alignas(8) uint8_t _IDENT4C_TRIE_START[402];

extern const alignas(8) uint8_t _IDENT4C_TRIE_CONTINUE[1793];

extern const alignas(64) uint8_t _IDENT4C_LEAF[7520];

/* Impl */

#define IDENT4C_MAX_CODEPOINT 0x10FFFF

inline bool ident4c_is_xid_start(uint32_t ch)  {
    assert(ch <= IDENT4C_MAX_CODEPOINT);
    if (ch < 128) {
        return _IDENT4C_ASCII_START[ch];
    }
    uint32_t chunk_index = ch / 8 / _IDENT4C_CHUNK;
    uint8_t chunk = chunk_index < sizeof(_IDENT4C_TRIE_START) ? _IDENT4C_TRIE_START[chunk_index] : 0;
    size_t offset = ((size_t) chunk) * _IDENT4C_CHUNK / 2 + ((size_t) ch)  / 8 % _IDENT4C_CHUNK;
    return ((_IDENT4C_LEAF[offset] >> (ch % 8)) & 1) != 0;
}

inline bool ident4c_is_xid_continue(uint32_t ch) {
    assert(ch <= IDENT4C_MAX_CODEPOINT);
    if (ch < 128) {
        return _IDENT4C_ASCII_CONTINUE[ch];
    }
    uint32_t chunk_index = ch / 8 / _IDENT4C_CHUNK;
    uint8_t chunk = chunk_index < sizeof(_IDENT4C_TRIE_CONTINUE) ? _IDENT4C_TRIE_CONTINUE[chunk_index] : 0;
    size_t offset = ((size_t) chunk) * _IDENT4C_CHUNK / 2 + ((size_t) ch) / 8 % _IDENT4C_CHUNK;
    return ((_IDENT4C_LEAF[offset] >> (ch % 8)) & 1) != 0;
}

#endif // IDENT4C_H
