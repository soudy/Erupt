# Erupt
Erupt is my attempt at building a compiler in C. This project is for learning
purpose only and should not be used in production.

# Example
A simple hello world program may look like this:

```ex
use IO

main =>
  IO.print("Hello world")
```

# Installation
Installing Erupt requires some dependencies:

## Dependencies
* git (optional)
* LLVM >= 3.6 installed and `llvm-config` in your `$PATH`
* a C and C++ compiler (eg. `gcc`, `clang`)
* GNU `make`

## Building

```bash
$ git clone https://github.com/soudy/Erupt
$ cd Erupt
$ make
```

This will create the binary `build/erupt`. If you wish to install `erupt` into
your path, run `make install` with superuser privileges.

If you wish to use a non-default C compiler:
```bash
$ CC=compiler make
```
(where `compiler` is the compiler of choice).

Erupt has been tested and proven to work with `gcc` 5.2.0 and `clang` 3.6.2 on
x86_64 GNU/Linux, but most other C compilers with C99 support should work.

# Run
```bash
$ erupt [options] file
```

## Options
```
-o, --output
       set output path (default: main)
-v, --version
       show version
-V, --verbose
       verbose mode
-t, --tokens
       show generated token stream
-n, --nodes
       show nodes of the generated AST
-h, --help
       show this
```
