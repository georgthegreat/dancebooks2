# hda-bild

## About

`bild` is intended to simplify the process of writing the Makefile (as the title suggests). Sooner or later, it will support the entire lifecycle of some common software types: compiling, testing, installing, etc.

`bild` is written using `GNU Makefile` technology, so building a project with `bild` is just writing a proper `Makefile`.

## Builing a library from C++ source code

Put your library interface files (headers) into `include/` subfolder. It will be added to include path during compilation.

Define the following methods variables:

* `LIB` — basename for the library to be built (both shared object and static library will be built)
* `SOURCES` — list of C++ files to be compiled
* **(optional)** `CXX` — compiler to be used for compilation (default: `g++`). Compiler should properly support `g++` options
* **(optional)** `CXXFLAGS` — additional flags to be passed to compiler. Some rational defaults will be applied.
* **(optional)** `INCLUDES` — additional folders with header files (local `include/` subfolder will be added by default)
* **(optional)** `LDFLAGS` — additional flags to be passed to linker.
* **(optional)** `LIBS` — names of libraries to be linked against

## Testing your newly compiled library

* `TESTS` — list of test modules (`.cpp` files) to be executed. `bild` assumes that each test module will be compiled independently (i. e. it should define `main` function)
* Each test module will be linked against `libboost-unit-test-framework`
* Each test module will be linked against newly compiled library and executed with proper `LD_LIBRARY_PATH` set
* Tests can be executed explicilty by running `test` make target

Following additional variables are supported (their meaning is the same as described above) 

* **(optional)** `TEST-CXX`
* **(optional)** `TEST-CXXFLAGS`
* **(optional)** `TEST-INCLUDES`
* **(optional)** `TEST-LDFLAGS`
* **(optional)** `TEST-LIBS`

## Installing and uninstalling

Though proper packaging is not currently supported by the `bild`, some (much more simple) tools are available to replace this functionality.

* Define variable `FILES` containing a list of destination install paths. Here and below we would consider the following example:
	```
	FILES := \
		/usr/lib/$(LIB).so \
		/usr/include \
	```
* Upon invoking `make install`, `bild` will process this variable with the following effect:
	1. First line will install `$(LIB).so` to `/usr/lib` folder
	2. Second line will install files from local `include` folder (containing library interface headers) to global `/usr/include`, keeping the folder structure properly
* Upon invoking `make uninstall`, `bild` will remove all files previously installed
* **WARNINING** Since `bild` doesn't store any installation state anywhere (compared with e. g. `apt` suite), side-effects are possible if `make uninstall` happens with folder structure different from those of `make install` time. Try to avoid such things
* Both `make install` and `make uninstall` will require root priveleges when installing to a global folder
