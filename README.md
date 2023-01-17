Unicode ident 4C
=============
Port of [unicode-ident](https://github.com/dtolnay/unicode-ident) to C(11).

The library is very small and fast.

The static library is around 35 KiB vs 300+ KiB for alternative libraries (like `utf8proc`).

See upstream for more performance details.

The downside is it only does one thing (while `utf8proc` does many).

See also:
- [libgrapheme](https://libs.suckless.org/libgrapheme/)
- [utf8proc](https://github.com/JuliaStrings/utf8proc)