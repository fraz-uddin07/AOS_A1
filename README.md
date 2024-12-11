# AOS_Assignment 1
#
## A1_Q1 Overview

In this question we are provided with an input file name and we need to  reverse the contents of this file and store it in a new file in the directory named "Assignment 1". A flag is also provided whose value can be either 0 or 1, and depending on the flag the operation is perdformed.
- If flag supplied is 0, then the entire file contents are reversed
- If flag is 1, then the file is reversed in 3 parts, from start to start_index and from end_index to the EOF, leaving the portion between start and end index unchanged. 
- Output file should be stored in the directory "Assignment1/" with the output file name as <flag>_<input_file_name>

## Explanation of the Code

1. The main() function takes command line arguments (including flag and input file name), opens them(using open(), gets info about the files (using fstat()), decides on the optimal buffer size and start and end indices. main() also handles edge cases with arguments, flags, indexes as well as file I/O errors.
2. The file_reverse() function takes the open file descriptors, start index, end index and Buffer Size and reverses the contents of the file from start to end index (both inclusive) by splitting them in chunks of size equal to Buffer Size.
3. The contents of the input buffer are copied in reverse order in the output buffer which are then written to the output file, hence reversing the file.
4. Two buffers are created namely "input\_buff" and "output\_buff" which store the original contents of the file and the reversed contents. "lseek()" command is used to point to the beginning of each chunk (stored in curr\_seek\_idx) and data starting from that point upto the size of the buffer is read in each iteration. Also the lseek() for new file starts from zero.
5. After each iteration the old file pointer is decremented by "buffer\_size" to point to the previous chunk and the new file pointer is incremented by buffer\_size to point to the next chunk. This continues until the contents remaining are less than buffer size.
6. In case a flag of 1 is provided then the file_copy() function simply copies the contents from start index to end index (both inclusive) after splitting them into chunks and puts it in the new file.   
6. The last chunk is handled separately using the same logic by reading the appropriate remaining characters at the end.
7. Finally after the reversed file has been written, both files are closed using the close() system call.

### Shell commad to build and run the file :-
```sh
g++ 2024201064_A1_Q1.cpp
./a.out <input_file_name> <flag> <start_index> <end_index>
```


## A1_Q2 Overview

In this question, we are asked to check the permissions for the two files and directory, and also verify whether the new file is the reverse of the old file or not.
- The new file path, old file path and Directory path are provided as a command line argument
- Message showing whether file contents are reversed or not should be displayed
- Permissions for all 3 (old file, new file and Directory) for User, Group and Others should be displayed

## Explanation of the Code

1. The main() function takes in command line arguments and opens the files and directory in read\_only mode, validates whether files and directory exist or not, decides on the optimal buffer size, then checks whether the files are reversed or not using identical\_file\_check() function.
2. The identical\_file\_check() function takes two files (namely new and old file) reads newfile from the last and oldfile from the start, puts their contents in their respective buffers and then checks whether the buffer contents are reverse of each other or not.
3.The last chunk which might possibly be of a different size is handled separately usign the same logic after calculating the remaining size of file to read.
4. The print permission prints the permsssions for newfile, oldfile and directory by taking information from the st_mode value in their respective fstat() structures.

### Shell command to build and run the file :-
```sh
g++ 2024201064_A1_Q2.cpp
./a.out <input_file_path> <output_file_path> <Directory_path>
```

