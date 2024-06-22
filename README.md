Author: Keshav Dave
NetID: khd47

Important design notes
    This program detects and reports spelling errors in one or more text files.
    The first argument to spchk will be a path to a dictionary file. Subsequent arguments will be paths to text files or directories
    A binary search is used to navigate through the dictionary.
    Any files or directories whose names begin with “.” are ignored.
    This is an example of how an error will be reported: file1 (35,8): foom
    The method read() is used, not fread().
    In spchk, only the method traverseDirectory needs to be called in main, it will call all the other methods.
    Some specific words may not work as intended due to the fact the the dictionary file has unsupported charecters by ASCII. For example, "AA's" cannot be found.
    A make file will not be included in this project as I do not find it neccessay and it is not required
    Some test files and directories are included in the project

How to compile and run project files
    gcc -o <file name> <file name>.c <textfile or directory> 
        (there can be more than 1 test file or directory)
    ./<file name>
    ex.
    gcc -o spchk spchk.c  /usr/share/dict/words testingDirectory test.txt .test.txt
    ./spchk

Test plan
    The testing for this project is split into 3 parts:
        Finding and opening all the specified files, including directory traversal
        Reading the file and generating a sequence of position-annotated words
        Checking whether a word is contained in the dictionary
    spchk.c puts together all three of these to create a spell-checking program

Test programs
    part_one_test.c
        Argument(s): directory(s) or filename(s)
        This test file is designed to test the first part of the project: Finding and opening all the specified files, including directory traversal. This test file focuses on the capabilities of the function traverse_directory On success, all files that should be opened will have their name printed. 

    part_two_test.c
        Argument: filename (.txt file)
        This test file is designed to test the second part of the project:
        Reading the file and generating a sequence of position-annotated words. The words_in_file function will read every word from a particular file and calculate a position for each word. Also, this function will account for trailing punctuation in a word.

    part_three_test.c
        Arguments: none
        This test file is designed to test the third part of the project: Checking whether a word is contained in the dictionary. This file uses various functions to create a dictionary file, take in a word at a time, and compare it to the dictionary file using a binary search. Along with this, these function account for hyphens and capitialization. 

spchk.c
    Argument(s): dictionary path, directory(s) or filename(s)
    This is the main file of the project and it puts all three of these testing files together for a comprehensive program. This file will include comments for easier understanding 
