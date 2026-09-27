Lab 4 Assignment

This is the concatenation program.

To run the file, we do

gcc <file_name.c> -o <file_name>

NOTE: Make sure the second file above does not have a .c extension otherwise it won't compile properly

Description.

The program concatenates the contents of the 2nd file to the end of the first file. 

The contents of the second file are not altered, they remain the same. The first file now just has new information appended to it.

Conditions:
There needs to be two filename arguments.
    File names cannot be identical (including error check).
    File 1 must exist and be writable.
    File 2 must exist and be readable.

For example:
$ cat file1
    Hi guys! This is first file!
    $ cat file2
    Hello everyone! This is the second file!
    $ ./lab4 file1 file2 $ cat file1
    Hello everyone! This is the first file!
    Hello everyone! This is the other file!
    $ cat file2
    Hello everyone! This is the 2nd file!