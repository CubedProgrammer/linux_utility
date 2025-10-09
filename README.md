# linux\_utility
Command line utilities made for linux, and in theory, also for other POSIX-compliant operating systems.
## Compilation
Most files are built individually.

Quick compilation script, replace clang with gcc if preferred.
```sh
clang -std=c99 -O3 -c afile.c byteedit.c char.c chmap.c cpbar.c cpick.c cxso.c ffill.c floattoy.c fromfloat.c fromhexdec.c ifile.c jhash.c jhashrev.c jrand.c keycode.c lfg.c lnc.c lnc++.c loin.c lsinc.c mft.c ofile.c pidx.c preturn.c return.c reverse.c runc.c runc++.c separate.c sharg.c sshdl.c sshul.c tempc.c tofloat.c tohexdec.c tpcsv.c ttylog.c txthtml.c unic.c zero.c
clang -O3 -c clock.c lscmd.c pause.c
clang -o lnc.out lnc.o
rm lnc.o
for i in *.o; do i=${i::-2}; ./lnc.out $i; done
rm *.o
```
Installation, may require root privilages.
```sh
./loin.out ./loin.out
rm loin.out
for i in *.out; do loin $i; done
rm *.out
```
## Utilities
[afile](#afile)
[alarm](#alarm)
[byteedit](#byteedit)
[char](#char)
[chmap](#chmap)
[clock](#clock)
[cpbar](#cpbar)
[cpick](#cpick)
[cxso](#cxso)
[fexec](#fexec)
[ffill](#ffill)
[floattoy](#floattoy)
[fromfloat](#fromfloat)
[fromhexdec](#fromhexdec)
[ifile](#ifile)
[indent](#indent)
[jhash](#jhash)
[jhashrev](#jhashrev)
[jrand](#jrand)
[keycode](#keycode)
[lfg](#lfg)
[linewrap](#linewrap)
[lnc](#lnc)
[lnc++](#lnc++)
[loin](#loin)
[lscmd](#lscmd)
[lsinc](#lsinc)
[mft](#mft)
[ofile](#ofile)
[parg](#parg)
[pause](#pause)
[pidx](#pidx)
[preturn](#preturn)
[remind](#remind)
[return](#return)
[reverse](#reverse)
[runc](#runc)
[runc++](#runc++)
[separate](#separate)
[sharg](#sharg)
[sshdl](#sshdl)
[sshul](#sshul)
[stopwatch](#stopwatch)
[tempc](#tempc)
[tofloat](#tofloat)
[tohexdec](#tohexdec)
[tpcsv](#tpcsv)
[ttylog](#ttylog)
[txthtml](#txthtml)
[unic](#unic)
[zero](#zero)

### afile
Append data from stdin to a file, the first argument is the file to write to.
```
afile a.txt
```
Appends data written to stdin to a.txt.
### alarm
Runs a command the next time a 24-hour clock hits a specific time.
```
alarm 23:29:31 rm file.txt
alarm 11 13 vlc ode-to-joy.ogg
```
The first command will delete file.txt when 23:29:31 hits. The second will play an audio file when 11:13:00 hits.

The alarm will only run once.

The program will fail silently if the program to be executed does not exist.
### byteedit
Edit bytes of a file, bytes will be shown in hexadecimal.
Uses environmental variable EDITOR to determine editor, defaulting to vi if it does not exist.
After editor is closed, converts the saved text from hexadecimal text into raw binary data.
Any character that is not 0-9, A-Z, a-z will be ignored.

Warning: Some graphical text editors will exit as soon as it is launched.
This could be because it sees an existing instance and opens it in that instead.
```
byteedit a.txt b.img c.c
```
Opens the editor for the three files a.txt, b.img, and c.c.
### char
Character, converts numbers into characters based on unicode value.
Each argument is a byte, given in hexadecimal.
```
char 21 22
```
Prints `!"` to the console.
### chmap
A character map.

First argument is starting row, second argument is number of rows to display at once.

Using up and down arrows to move by one row, page up and page down to move by all rows.

Exit using the escape key.

Requires the terminal to be at least sixteen by number of rows.
### clock
A clock that uses the command-line interface.

Shows a twenty-four hour clock, with seconds.
Exits automatically when typing next command for the shell.

No command line arguments.
### cpbar
Copies a file. The first argument is source and second argument is destination.
Or, the last argument is the directory to copy to, all other files will be copied to it.

Use option --barthresh to set the file size threshold for the progress bar to pop up.

Shows a progress bar for large files.
```
cpbar --barthresh 1073741824 A B
```
Copies file A to file B, overwriting file B if it existed before.
If file A is larger than 1 GiB, progress bar shows up.
### cpick
Colour picking tool.

Press lowercase r, g, or b to change the red, green, or blue by one.
Use uppercase to change by sixteen.

Press I to invert the colour.

Press X to reverse direction, default is to increase. Press lowercase q to exit.

Displays all the colours given as command line arguments in hexadecimal.
### cxso
C Executable Shared Object, createds a file that is both a shared library and an executable program.

If the first argument is ++, C++ is assumed to be the language.

SCRT environment variable will be used to determine the location of Scrt1.o, for calling main function.
If it is not set, then directories in ld.so.conf will be searched.
This file is responsible for calling the main function.
If it is still not found, main will be set to the program entry point instead.
This could mean a lot of C language features break, so try to ensure Scrt1.o is found.

heron.c
```c
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
// Might be different path on your computer, but will be named ld-linux-*.so.2
const char dl_loader[]__attribute__((section(".interp"))) = "/usr/lib64/ld-linux-x86-64.so.2";
float heron(float a, float b, float c)
{
    float s = (a + b + c) * 0.5;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}
int main(int argl, char *argv[])
{
    float num = atof(argv[1]);
    printf("%f\n", heron(num, num, num));
    return 0;
}
```
program.c
```c
#include<stdio.h>
#include<stdlib.h>
float heron(float a, float b, float c);
int main(int argl, char *argv[])
{
    printf("%f\n", heron(atof(argv[1]), atof(argv[2]), atof(argv[3])));
    return 0;
}
```
Compile both files, replace clang with gcc if preferred.
```
clang -O3 -c program.c
clang -O3 -c heron.c -fPIC
cxso -o libheron.so heron.o -lm
clang -o program.out program.o -L. -lheron
```
### fexec
Executes a file that has a list of command line arguments.

The file contains one command line argument per line, the first argument is also the program to run.
There are no escape sequences, the line feed character \n is the only separator.

This program takes one argument, the path to the file to be executed. Suppose cmd.txt contains
```
cc
-O3
-c
-std=c11
main.c
```
Then the command
```sh
fexec cmd.txt
```
or
```sh
fexec <cmd.txt
```
will run the program `cc` with the arguments `cc`, `-O3`, `-c` `-std=c11`, and `main.c`.
### ffill
Fills a file with a certain number of a specific byte.

First argument is the byte to fill with, in hexadecimal. The second is the number of bytes, in decimal.

All arguments after that are file names, those are the files that will be written to.
If any of the files do not already exist, they will be created.

This program was made for the sole purpose of testing [cpbar](#cpbar), but may still be useful elsewhere.
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
```
fromfloat 3.14159265 2.71828184
```
### fromhexdec
Reads hexadecimal numbers and prints every byte as a character.

First argument is file to read from, second is file to write to.
If any argument is -, then it uses stdin or stdout.

Uses stdin and stdout if no arguments are present.
```
fromhexdec a.txt b.txt
```
If a.txt has 696a6b, b.txt will contain ijk.

Can be compiled into a shared library that is also an executable, see [cxso](#cxso)
A default location for dl\_loader is defined, override it by defining the macro LOADER to the desired file.
### ifile
Write data from a file to stdout, the first argument is the file to read from.
```
ifile a.txt
```
Reads data in a.txt and writes to stdout.
### indent
Adds or removes a certain number of spaces to or from the beginning of each line in a file.

The first argument has three comma-separated numbers. The first number is how many spaces to indent, negative unindents.
The second and third number is the starting line number and ending line number, inclusive, the first line is line 1.

The second and third argument are the input and output files. They should not be the same file.
```
indent 4,2,10 a.txt b.txt
```
Indents lines 2 to 10 by 4 spaces.
### jhash
Applies the java hashing algorithm to given strings and outputs the hash.
```
jhash racecar
```
Outputs 967767907.
### jhashrev
Inverse of jhash, given a hash, output possible strings that can produce it.
```
jhashrev 967767907
```
Should output racecar as one of the strings.
### jrand
Shows how java.util.Random manipulates seeds.

The first argument is the seed given to the constructor, or `java.util.Random.setSeed(long)`.
The second is the number of iterations, if blank, enters interactive mode.

In interactive mode, press enter to see the next iteration.
```
jrand 13 12
```
Displays twelve iterations, starting with thirteen as the seed.

More precisely, first displays the initial seed.
Then, displays the seed after calling `java.util.Random.next(int)`, repeated ten more times.
### keycode
Displays byte sequences that correspond to terminal key strokes.

The command line argument is the single character sequence that terminates the program, must be a single character.
If this argument is not present, the default character is ESC.
```sh
keycode Q
```
Displays key codes until uppercase Q is pressed.
### lfg
List File Generator.

An old program returns! Generate a file with a list of numbers, one on each line.

First argument is the size of the list, the file will contain the numbers one through the size.

Second argument is the format of the numbers, it must begin with an f.
If it is just `f`, default format is used, otherwise, it is a single C printf formate specifier.

Remaining arguments are the files to create.
```
lfg 12 f%02u a.txt
```
a.txt will contain
```
01
02
03
04
05
06
07
08
09
10
11
12
```
The numbers are printed with %02u using printf, as specified in the second argument.
### linewrap
Wraps the lines of a file, ensuring no line has more than a certain amount of characters.

The first parameter is the maximum number of characters per line, call this number n.
There will be more more than n characters between the previous line break and the next, exception below.

The second and third parameters are the input and output files, respectively.

Line breaks will only replace spaces, character with unicode 0x20.

If a line contains no spaces for more than n characters, then that line will not be broken up.
```sh
linewrap 128 a.txt b.txt
```
Write a.txt to b.txt, where lines are wrapped at 128 characters.
### lnc
Link C, links an object file using a C compiler.

COMPILER environment variable is used to determine the C compiler.

The first argument of the program is the object file, without the .o.
If the object file is main.o, then main should be the first argument.
The executable file produced is ${1}.out, if first argument is main, then main.out.

All other arguments are passed to directly to the C compiler.
```
lnc program
```
Linkes program.o using the C compiler into program.out.
### lnc++
Link c++, same as lnc, but uses COMPILERPP variable for C++ compiler.
```
lnc++ program
```
Linkes program.o using the C++ compiler into program.out.
### loin
Local install, installs a program or shared library locally to /usr/local/bin or /usr/local/lib.

All command line arguments are programs or shared libraries to install.
```
loin code/program.out code/libshared.so
```
Copies code/program.out to /usr/local/bin/program and code/libshared.so to /usr/local/lib/libshared.so
### lscmd
List commands, lists available programs based on PATH variable.

Looks in the directories of PATH environment variable for executable programs.
If two programs has the same file name but in different directories, it is only listed once.
Only lists file name if the file is a regular file with execute permission or a symbolic link.

The first argument is the prefix, if present, program only lists commands with that prefix.
```
lscmd ssh
```
Lists all commands beginning with ssh.
### lsinc
List includes, lists the files included by a C or C++ source file.

Looks in /usr/include and /usr/local/include.
Can add additional include paths using arguments -I/path/to/include, relative paths allowed.
All other arguments are files to search through.
Each file listed will have preceeding spaces based on the inclusion depth.

A file directly included in a searched file will have one preceding space.
A file included by a file directly included will have two, so on and so forth.
```
lsinc program.c
```
### mft
Make from template.
Formerly mf.c, make file, requires a directory in home directory called .ftemplates.

In that directory a file called .c will be the template for c files.
The second argument to the program is the text after the dot in the file name of the template.
\_\_NAME\_\_ will be replaced with the first argument of the program.
\_\_TYPE\_\_ will be replaced with the second argument of the program.

So, use them as placeholders for where the name or type of the program should go in the newly created file.
```
mft program c
```
Will copy ~/.ftemplates/.c to program.c and do the replacements accordingly.
### ofile
Write data from stdin to a file, the first argument is the file to write to.
The file will be completely cleared before any writing.
```
ofile a.txt
```
Writes data written to stdin to a.txt.
### parg
Displays the command line arguments of the specified process.
```
parg 1729
```
Should display the command line arguments of process with identifier of 1729.
### pause
Waits a certain amount of time or for a key press.

The program waits for an input from stdin, in non-canonical mode, then exits.
Thus, pressing any key will terminate the program.

If the first argument is present, it will determine the number of seconds to pause instead.
Suffixes ms, us, and ns change the units to millisecond, microsecond, and nanosecond instead.
```
pause 750ms
```
Does nothing for 750 milliseconds and then exits.
### pidx
Process Identifier Execute, prints the process ID before exec(3)uting the program.

$1, or the first command line argument (first not counting program name), is the program to be executed, and all arguments following are arguments to that program.
```
pidx git
```
Prints the process identifier of git, before running the program git.
### preturn
Print Return, executes a program and prints exit status of the process.

First arguemnt is radix, b for binary, d for decimal, x for hexadecimal.
The remaining arguments are the program to run and its arguments.
```
preturn x return 91
```
Prints 5B
### remind
Executes a command after a specified duration.

If there is one duration argument, it is in seconds, if there are two, minutes and seconds.
Up to five duration arguments are supported, weeks, days, hours, minutes, seconds.

```
remind 10 vlc ode-to-joy.ogg
remind 1 30 0 brave https://github.com
```
The first command will play an audio file using VLC in ten seconds.
The second will open GitHub in Brave browser in a hour and thirty minutes.
### return
Program exits with specified exit status.

The first argument is the exit status, it is assumed to be in decimal.
However, prefixes 0b can be used for binary, 0 for octal, and 0x hexadecimal.
```
return 0x5b
```
Exits with status 91.
### reverse
Reverses the characters of its arguments.

All arguments are printed, one on each line, but with the characters in reverse order.
```
reverse racecar abcde
```
Prints
```
racecar
edcba
```
### runc
Runs C code, for [tempc](#tempc).

Uses value of COMPILER environment variable as C compiler, or cc if not found.

### runc++
Runs C++ code, for [tempc](#tempc).

Uses value of COMPILERPP environment variable as C++ compiler, or c++ if not found.

### separate
Runs a program and detaches the process from terminal.
Also redirects all standard streams to null device.

Looks at environment variable LU\_SEPARATE\_VERBOSE.
0 means no output.
1 means show process ID.
2 means show process ID in full sentence.
```
separate git push
```
Runs git push detached, prints out the process identifier.
### sharg
Show arguments, prints all command line arguments.

Displays all command line arguments, each preceding with its position.
This is to clearly display multi-line arguments.
This can be useful when command line arguments are shell expansions.
```sh
sharg ~ ${SHELL:1:3}
```
Displays home directory and a substring of the SHELL variable, on the shell.
### sshdl
If the environment varible `REMOTE_LOCATION` is set, use it as the directory to download to.
If there is a file called .remoteloc in the present working directory, then it is used as the source directory.
The local file overrides the environment variable if both are present.

The scp command is used for downloading, ensure it is available.

Note that the shell is used to run the command, beware of shell expansions in your file or environment variable.
```sh
export REMOTE_LOCATION='marysmith@13.17.19.23:\~/Documents'
sshdl document.pdf math-homework
```
Downloads document.pdf and math-homework from the Documents folder in the remote server to a new file document.pdf and new directory math-homework.

Equivalent to
```sh
scp -r marysmith@13.17.19.23:\~/Documents/document.pdf document.pdf
scp -r marysmith@13.17.19.23:\~/Documents/math-homework math-homework
```
### sshul
If the environment varible `REMOTE_LOCATION` is set, use it as the directory to upload to.
If there is a file called .remoteloc in the present working directory, then it is used as the destination directory.
The local file overrides the environment variable if both are present.

The scp command is used for uploading, ensure it is available.

Note that the shell is used to run the command, beware of shell expansions in your file or environment variable.
```sh
export REMOTE_LOCATION='marysmith@13.17.19.23:\~/Documents'
sshul document.pdf math-homework
```
Uploads document.pdf and math-homework to the Documents folder in the remote server.

Equivalent to
```sh
scp -r document.pdf marysmith@13.17.19.23:\~/Documents/document.pdf
scp -r math-homework marysmith@13.17.19.23:\~/Documents/math-homework
```
### stopwatch
A stop watch on the command line, press RETURN to record a time, press ESC to exit the program.

Millisecond precision.
### tempc
Temporary code, is for running code without manually creating a file and compiling.

There should be a directory called .tempc in the home directory.
The program runs the program specified by the enviornment variable EDITOR or vi is no such variable.
After the editor exits, the program attempts to compile and run the file.

The program looks for an executable file with exact same name as the first argument to the program, inside .tempc.
If found, the program will execute the file to compile and run the temporary program.
That program will take in the path to the temporary source code as the first argument.
That program shall run the temporary program and exit after the process exits with same exit status.

For example, to run C programs, one would need a script at .tempc/c, this script should compile and run the file.
It will be executed like this `~/.tempc/c temporary.c`, so write the script accordingly.

The second argument is optional, and specifies the name of the temporary file, excluding the file extension.

If the .ftemplates directory exits in the home directory, mft will be used to initialize the template code.
```
tempc c prog
```
Creates a temporary program prog.c, opens editor, compiles and runs it after closing editor.
### tofloat
Shows decimal value of the bytes of a single-precision IEEE-754 floating-point number.

Each argument is the bits of a single number.
If an argument has prefix 0x, it is assumed to be in hexadecimal, binary otherwise.
```
tofloat 0b01000000010010010000111111011011 0b01000000001011011111100001010100
```
### tohexdec
Reads characters and prints their unicode values in hexadecimal.

First argument is file to read from, second is file to write to.
If any argument is -, then it uses stdin or stdout.

Uses stdin and stdout if no arguments are present.
```
tohexdec a.txt b.txt
```
If a.txt has ijk, b.txt will contain 696a6b.

Can be compiled into a shared library that is also an executable, see [cxso](#cxso)
A default location for dl\_loader is defined, override it by defining the macro LOADER to the desired file.
### tpcsv
Transpose Comma Separated Values
Reads CSV files, and transposes the matrix, and writes back into the CSV file.

Entries cannot have commas, there are no options to escape them.

### ttylog
Teletype log, logs data displayed by a virtual terminal.

Spawns a new virtual terminal, running /usr/bin/bash if SHELL environment variable is empty.
If the first argument is present, it shall be used as the log file, otherwise, ttylog.log is used.

### txthtml
Converts a txt file to a partial HTML file, that is, without head and body tags.
```sh
txthtml input.txt output.html
```
If input or output is omitted, standard input or output will be used.
### unic
Unicode, displays the unicode values of characters.

Each argument has its values printed on a new line, in hexadecimal.
```
unic abc
```
Prints 616263, the hexadecimal values of abc.
### zero
Fills a file with zeros.

File size may be expanded, all bytes are overwritten with zero.
Each argument is a file to clear with zeros.
```
zero file.txt
```
