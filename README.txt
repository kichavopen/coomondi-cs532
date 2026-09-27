RECURSIVE DIRECTORY TRAVERSAL 
============================== 

Takes readdir_v2.c one step further by allowing it to search through
all directories and list out files. 

COMPILATION 
----------- 
gcc -o recursive_traversal recursive_traversal.c 

USAGE 
----- 
./recursive_traversal <path to directory> 

you can type any path into it. 
./recursive_traversal /home/yourusername 
./recursive_traversal . 
./recursive_traversal /etc 

OUTPUT 
------ 
Will output what files are in that directory and what type of file they
are. Will also show you what subdirectories there are.

file (normal file) 
folder (directory) 
another_file.txt (normal file) 
deeper_folder (directory) 
test.c (normal file) 

HOW IT WORKS 
------------ 
Has a function called traverse() which will call itself on subdirectories
and skip "." and ".." so it doesn't keep searching the same directory.

This satisfies the assignment because it modified the readdir_v2.c to
search through all directories using a function. And that function calls 
itself on subdirectories to find more sub directories.
