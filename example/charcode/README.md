# Reference implementation of character encoding

This project is designed to be a stand-alone zero-dependency ANSI-C implementation of the character encoding detection algorithm from [ELF Serialisation](https://fhiso.org/TR/elf-serialisation).

To compile, run

```
cc ansel2utf8.c ged2utf8.c -o ged2utf8
```

replacing `cc` with your C compiler of choice if needed
and adding `.exe` at the end of the second `ged2utf8` if running a version of the Windows OS that uses file endings to identify programs.

To run, execute the resulting `ged2utf8`. It accepts .ged files from stdin or the command line and outputs them in UTF-8, correcting a few other simple errors like blank lines, etc, along the way. See the comments in the code for more.
