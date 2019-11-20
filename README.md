# hasm
A poorly-written assembler for the [Nand2Tetris](https://www.nand2tetris.org/) Hack computer.\
Implemented entirely in C++, with a a few test programs in Hack assembly included.

# Build
### Linux
Build for Linux with `make`. The makefile currently uses `ccache g++` by default.

### Windows
A target for 64-bit Windows exists -- run `make windows`. Note that the Windows binaries are significantly slower; I can only imagine this is because the standard C++ and GCC libraries are statically linked, but I haven't tested enough to know.