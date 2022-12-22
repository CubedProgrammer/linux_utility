# linux\_utility
Command line utilities made for linux, and in theory, also for other POSIX-compliant operating systems.
## Compilation
Most files are built individually.

mft.c, make from template, formerly mf.c, make file, requires a directory in home directory called .ftemplates.
In that directory a file called .c will be the template for c files.
The second argument to the program is the text after the dot in the file name of the template.
\_\_NAME\_\_ will be replaced with the first argument of the program.
\_\_TYPE\_\_ will be replaced with the second argument of the program.

fromhexdec.c and tohexdec.c are meant to be compiled as shared libraries.

tempc.c, temporary code, is for running code without manually creating a file and compiling.
There should be a directory called .tempc in the home directory.
The program runs the program specified by the enviornment variable EDITOR or vim is no such variable.
After the editor exits, the program attempts to compile and run the file.
The program looks for an executable file with exact same name as the first argument to the program.
If found, the program will execute the file to compile and run the temporary program.
The second argument is optional, and specifies the name of the temporary file, excluding the file extension.
