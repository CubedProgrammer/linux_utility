# linux\_utility
Command line utilities made for linux, and in theory, also for other POSIX-compliant operating systems.
## Compilation
Most files are built individually.

mft.c, make from template, formerly mf.c, make file, requires a directory in home directory called .ftemplates.
In that directory a file called .c will be the template for c files.
The second argument to the program is the text after the dot in the file name of the template.
\_\_NAME\_\_ will be replaced with the first argument of the program.
\_\_TYPE\_\_ will be replaced with the second argument of the program.
So, use them as placeholders for where the name or type of the program should go in the newly created file.

fromhexdec.c and tohexdec.c are meant to be compiled as shared libraries.

tempc.c, temporary code, is for running code without manually creating a file and compiling.
There should be a directory called .tempc in the home directory.
The program runs the program specified by the enviornment variable EDITOR or vi is no such variable.
After the editor exits, the program attempts to compile and run the file.
The program looks for an executable file with exact same name as the first argument to the program.
If found, the program will execute the file to compile and run the temporary program.
The second argument is optional, and specifies the name of the temporary file, excluding the file extension.

If the .ftemplates directory exits in the home directory, mft will be used to initialize the template code.

pidx.c, process identifier execute, prints the process ID before exec(3)uting the program.
$1, or the second command line argument (first not counting program name), is the program to be executed, and all arguments following are arguments to that program.

cxso.c, c executable shared object, createds a file that is both a shared library and an executable program.
If the second argument is ++, C++ is assumed to be the language.
SCRT environment variable will be used to determine the location of Scrt1.o, for calling main function.
If it is not set, then directories in ld.so.conf will be searched.
If it is still not found, main will be set to the program entry point instead.
This could mean a lot of C language features break, so try to ensure Scrt1.o is found.
