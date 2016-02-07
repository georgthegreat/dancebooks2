# hda-bild

## About

`bild` is intended to simplify the process of writing the Makefile (as the title suggests). Sooner or later, it will support the entire lifecycle of some common software types: compiling, testing, installing, etc.

`bild` is written using `GNU Makefile` technology, so building a project with `bild` is just writing a proper `Makefile`.

## Builing a library from C++ source code

Put your library interface files (headers) into `include/` subfolder. It will be added to include path of your compiler.

Define the following methods variables:

* `LIB` — basename for the library to be built (both shared object and static library will be built)
* `SOURCES` — list of C++ files to be compiled
* `TESTS` — list of test modules to be executed. `bild` assumes that each test module will be compiled independently (i. e. it should define `main` function). Each test module will be linked against newly compiled library and executed with proper `LD_LIBRARY_PATH` set. Tests can be executed by running `test` make target.

You can optionally define the following variables:

* `CXX` — compiler to be used for compilation (default: `g++`). Compiler should properly support `g++` options (i. e. it's OK to use some backported `g++` version or `clang`, but it's not OK to use Visual Studio CL)
