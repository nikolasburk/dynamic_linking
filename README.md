### Dynamic Linking

1. Creating [Mach-O](https://en.wikipedia.org/wiki/Mach-O) file (`.o`)

    clang -c calculator.c -o calculator.o


2. Creating library

    libtool -dynamic calculator.o -o libcalc_dynamic.dylib -lSystem

This command creates the dynamic library (_dylib_) and links against `libSystem`. The dynamic library is a Mach-O binary file and will be loaded dynamically at launch time by `dyld` as a dependency of another binary. If other (static?) frameworks should be linked, the `-framework` flag can be used, for example:

    libtool -dynamic calculator.o -o libcalc_dynamic.dylib -framework CoreFoundation -lSystem


3. Generate object file for `main`

    clang -c main.c -o main.o


4. Linking

    ld main.o -lSystem -L. -lcalc_dynamic -arch x86_64 -o test_dynamic -v

This command geneates a binary executable from the main object file, also passing: 
- `lSystem` for `dyld_stub_binder`
- `lcalc_dynamic` for linking agfainst `libcalc_dynamic.dylib`
- `-arch x86_64` to specify for which architecture the program should be executable


5. Running

    ./test_dynamic


7. Checking symbols

The `nm` tool can be used to display the name list (symbol table) of each object file in the argument list. Each symbol is preceded by its value (blanks if undefined). Unless the option `-m` is specified, this value is followed by one of the following characters, representing the symbol type: U (undefined), A (absolute), T (text section symbol), D (data section symbol), B (bss section symbol), C (common symbol), S (symbol in section other than those mentioned above), I (indirect symbol).

    $ nm test_dynamic 
    0000000100000000 T __mh_execute_header
                     U _add
    0000000100000ee0 T _main
                     U _multiply
                     U _printf
                     U dyld_stub_binder

The symbols `_add` and `_multiply` do not have addresses, because they don't exist inside of the main binary, they only exist in the dynamic library that was created. 


8. Checking references

The `otool` tool can be used to display specified parts of object files or libraries. The `L` option prints all shared libraries that are used.


    $ otool -L test_dynamic 
    test_dynamic:
      /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1226.10.1)
      libcalc_dynamic.dylib (compatibility version 0.0.0, current version 0.0.0)

The output shows that the binary `test_dynamic` only links against `libSytem` and the custom dynamic library that was created before. The library `calc_dynamic` (`libcalc_dynamic.dylib`) is responsible for linking against any additional libraries it needs. This is resolved at launch time, dynamically. In this case, the search path for `libcalc_dynamic.dylib` is going to be the same as the search location as the main executable.

Dynamic libraries and frameworks are loaded at launch time by the dynamic linker. They have associated search paths to help the linker find where they are located on the file system and load them.