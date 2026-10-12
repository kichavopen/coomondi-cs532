README.txt for search 

A program that allows you to traverse through a file system much like find does in unix.

-S will list out files and display size permission and time accessed.
-s <int> will list out all files that have a size less than or equal to <int>.
-f <string> <int> will list out all files that contain the string and are within <int> amount of depth.
-t <char> if set to f it will list out all files if set to d it will only list out directories.

These can all be used together in any order. The program will list out all files that
fulfill those requirements. 

compile: 

make 

run: 

./search <arguments> <directory path> 

./search -S -s 1024 -f "txt" 2 

./search -t f -S 

./search -t d -s 500 <directory> 


Example commands I have used and work effectively are

1)Lists all files

./search /home/chris/homework2/projects/

2)Lists files with more information

./search -S /home/chris/homework2/projects/ 

3)Lists files with sies less than or equal to 1000 bytes
./search -s 1000 /home/chris/homework2/projects/

4) Lists files with details and sies
./search -S -s 500 /home/chris/homework2/projects

5)Lists c files with depth less than or equal to 3
./search -f "c" 3 /home/chris/homework2/projects/
