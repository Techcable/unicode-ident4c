use crate::output::Output;
use crate::parse::Properties;
use crate::CHUNK;

const HEAD: &str = "\
/* \x40generated by ../generate. To regenerate, run the following in the repo root:
 *
 * $ curl -LO https://www.unicode.org/Public/zipped/15.0.0/UCD.zip
 * $ unzip UCD.zip -d UCD
 * $ cargo run --manifest-path generate/Cargo.toml
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>

#include <assert.h>

#include \"ident4c.h\"

#define T ((uint8_t) true)
#define F ((uint8_t) false)

";

pub fn output(
    properties: &Properties,
    index_start: &[u8],
    index_continue: &[u8],
    halfdense: &[u8],
) -> Output {
    const PREFIX: &str = "_IDENT4C_";
    let mut out = Output::new();
    writeln!(out, "{}", HEAD);

    writeln!(
        out,
        "const alignas(64) uint8_t {PREFIX}ASCII_START[128] = {{",
    );
    for i in 0u8..4 {
        write!(out, "   ");
        for j in 0..32 {
            let ch = (i * 32 + j) as char;
            let is_xid_start = properties.is_xid_start(ch);
            write!(out, " {},", if is_xid_start { 'T' } else { 'F' });
        }
        writeln!(out);
    }
    writeln!(out, "}};");
    writeln!(out);

    writeln!(
        out,
        "const alignas(64) uint8_t {PREFIX}ASCII_CONTINUE[128] = {{",
    );
    for i in 0u8..4 {
        write!(out, "   ");
        for j in 0..32 {
            let ch = (i * 32 + j) as char;
            let is_xid_continue = properties.is_xid_continue(ch);
            write!(out, " {},", if is_xid_continue { 'T' } else { 'F' });
        }
        writeln!(out);
    }
    writeln!(out, "}};");
    writeln!(out);

    writeln!(
        out,
        "static_assert(({PREFIX}CHUNK == {CHUNK}), \"Unexpected chunk size\");"
    );
    writeln!(out);

    writeln!(
        out,
        "const alignas(8) uint8_t {PREFIX}TRIE_START[{}] = {{",
        index_start.len(),
    );
    for line in index_start.chunks(16) {
        write!(out, "   ");
        for byte in line {
            write!(out, " 0x{:02X},", byte);
        }
        writeln!(out);
    }
    writeln!(out, "}};");
    writeln!(out);

    writeln!(
        out,
        "const alignas(8) uint8_t {PREFIX}TRIE_CONTINUE[{}] = {{",
        index_continue.len(),
    );
    for line in index_continue.chunks(16) {
        write!(out, "   ");
        for byte in line {
            write!(out, " 0x{:02X},", byte);
        }
        writeln!(out);
    }
    writeln!(out, "}};");
    writeln!(out);

    writeln!(
        out,
        "const alignas(64) uint8_t {PREFIX}LEAF[{}] = {{",
        halfdense.len(),
    );
    for line in halfdense.chunks(16) {
        write!(out, "   ");
        for byte in line {
            write!(out, " 0x{:02X},", byte);
        }
        writeln!(out);
    }
    writeln!(out, "}};");

    out
}
