# linux\_utility
Command line utilities made for linux, and in theory, also for other POSIX-compliant operating systems.
## Compilation
Most files are built individually.

### byteedit
Edit bytes of a file, bytes will be shown in hexadecimal.
Uses environmental variable EDITOR to determine editor, defaulting to vi if it does not exist.
After editor is closed, converts the saved text from hexadecimal text into raw binary data.
Any character that is not 0-9, A-Z, a-z will be ignored.

### char
Character, converts numbers into characters based on unicode value.
Each argument is a byte, given in decimal.

### clock
A clock that uses the command-line interface.

Shows a twenty-four hour clock, with seconds.
Exits automatically when typing next command for the shell.

### cxso
C Executable Shared Object, createds a file that is both a shared library and an executable program.

If the first argument is ++, C++ is assumed to be the language.

SCRT environment variable will be used to determine the location of Scrt1.o, for calling main function.
If it is not set, then directories in ld.so.conf will be searched.
If it is still not found, main will be set to the program entry point instead.
This could mean a lot of C language features break, so try to ensure Scrt1.o is found.

### floattoy
Play with a single precision IEEE-754 floating-point number.

Use left and right arrows to move a single bit, press space to flip selected bit.

Use Ctrl+left and Ctrl+right to move a single byte.

Use HOME to go to most significant bit, END to least significant bit.

Use f to flip all bits, e to flip exponent bits, m to flip mantissa bits.

Use E to go to beginning of exponent, M to beginning of mantissa.

Use q to quit.

The exponent and mantissa bits are in different colours.

### fromfloat
Shows the bytes of a single-precision IEEE-754 floating-point number.
All arguments are numbers, their bits will be printed, one number per line.

### fromhexdec
Reads hexadecimal numbers and prints every byte as a character.

First argument is file to read from, second is file to write to.
If any argument is -, then it uses stdin or stdout.

Uses stdin and stdout if no arguments are present.

### lnc
Link C, links an object file using a C compiler.

COMPILER environment variable is used to determine the C compiler.

The first argument of the program is the object file, without the .o.
If the object file is main.o, then main should be the first argument.
The executable file produced is ${1}.out, if first argument is main, then main.out.

All other arguments are passed to directly to the C compiler.

### lnc++
Link c++, same as lnc, but uses COMPILERPP variable for C++ compiler.

### lscmd
List commands, lists available programs based on PATH variable.

Looks in the directories of PATH environment variable for executable programs.
If two programs has the same file name but in different directories, it is only listed once.
Only lists file name if the file is a regular file with execute permission or a symbolic link.

The first argument is the prefix, if present, program only lists commands with that prefix.

### lsinc
List includes, lists the files included by a C or C++ source file.

Looks in /usr/include and /usr/local/include.
Can add additional include paths using arguments -I/path/to/include, relative paths allowed.
All other arguments are files to search through.
Each file listed will have preceeding spaces based on the inclusion depth.

A file directly included in a searched file will have one preceding space.
A file included by a file directly included will have two, so on and so forth.

### mft
Make from template.
Formerly mf.c, make file, requires a directory in home directory called .ftemplates.

In that directory a file called .c will be the template for c files.
The second argument to the program is the text after the dot in the file name of the template.
\_\_NAME\_\_ will be replaced with the first argument of the program.
\_\_TYPE\_\_ will be replaced with the second argument of the program.

So, use them as placeholders for where the name or type of the program should go in the newly created file.

### pause
Waits a certain amount of time or for a key press.

The program waits for an input from stdin, in non-canonical mode, then exits.
Thus, pressing any key will terminate the program.

If the first argument is present, it will determine the number of milliseconds to pause instead.

### pidx
Process Identifier Execute, prints the process ID before exec(3)uting the program.

$1, or the first command line argument (first not counting program name), is the program to be executed, and all arguments following are arguments to that program.

### preturn
Print Return, executes a program and prints exit status of the process.

First arguemnt is radix, b for binary, d for decimal, x for hexadecimal.
The remaining arguments are the program to run and its arguments.

### Return
Program exits with specified exit status.

The first argument is the exit status, it is assumed to be in decimal.
However, prefixes 0b can be used for binary, 0 for octal, and 0x hexadecimal.

### Reverse
Reverses the characters of its arguments.

All arguments are printed, one on each line, but with the characters in reverse order.

### runc
Runs C code, for tempc.

Uses value of COMPILER environment variable as C compiler, or cc if not found.

### runc++
Runs C++ code, for tempc.

Uses value of COMPILERPP environment variable as C++ compiler, or c++ if not found.

### separate
Runs a program and detaches the process from terminal.

If compiled with QUIET macro defined, does not print the process ID.
Otherwise, shows process ID of launched process.

### sharg
Show arguments, prints all command line arguments.

Displays all command line arguments, each preceding with its position.
This is to clearly display multi-line arguments.
This can be useful when command line arguments are shell expansions.

### tempc
Temporary code, is for running code without manually creating a file and compiling.

There should be a directory called .tempc in the home directory.
The program runs the program specified by the enviornment variable EDITOR or vi is no such variable.
After the editor exits, the program attempts to compile and run the file.

The program looks for an executable file with exact same name as the first argument to the program.
If found, the program will execute the file to compile and run the temporary program.
That program will take in the path to the temporary source code as the first argument.
That program shall run the temporary program and exit after the process exits with same exit status.

The second argument is optional, and specifies the name of the temporary file, excluding the file extension.

If the .ftemplates directory exits in the home directory, mft will be used to initialize the template code.

### tofloat
Shows decimal value of the bytes of a single-precision IEEE-754 floating-point number.

Each argument is the bits of a single number.
If an argument has prefix 0x, it is assumed to be in hexadecimal, binary otherwise.

### tohexdec
Reads characters and prints their unicode values in hexadecimal.

First argument is file to read from, second is file to write to.
If any argument is -, then it uses stdin or stdout.

Uses stdin and stdout if no arguments are present.

### tpcsv
Transpose Comma Separated Values
Reads CSV files, and transposes the matrix, and writes back into the CSV file.

Entries cannot have commas, there are no options to escape them.

### ttylog
Teletype log, logs data displayed by a virtual terminal.

Spawns a new virtual terminal, running /usr/bin/bash if SHELL environment variable is empty.
If the first argument is present, it shall be used as the log file, otherwise, ttylog.log is used.

### unic
Unicode, displays the unicode values of characters.

Each argument has its values printed on a new line, in hexadecimal.

### zero
Fills a file with zeros.

File size may be expanded, all bytes are overwritten with zero.
Each argument is a file to clear with zeros.
