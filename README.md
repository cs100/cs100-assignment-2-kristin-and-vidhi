# Completed and archived in Winter 2018

We created this Rshell project for CS100 Winter 2018. 
Rshell is a command shell written in C++. 
It is able to take in commands and connectors and execute them.
Our design was based off the composite design pattern where the connector class and command class
correspond with the composite class and leaf, respectively.


## Installation
------------
Run these  commands for the rshell to work
```

$ cd rshell

$ make

$ bin/rshell
```

## Tasks Distribution For Assignment 3

KH:

    Task 1: Github Setup (10%)
    
    Task 2: "Test" command 1 (for the case "test -e test/file/path", 20%);
    
    Task 3: "Test" command 2 (for the case "[ -e test/file/path ]", 15%);
    
    
    Total: 45%
    
VJ:

    Task 4: The Precedence Operators (30%)
    
    Task 5: construct test scripts (enough test cases, 15%)
    
    Task 6: construct json and README files, and git commit (that means, all the works meaned from the part "Submission" to "Coding Conventions" in assignment 3 description)(10%)
            
    
    Total: 55%
    

## Bugs

While testing our code, we experienced some bugs. One bug occurred where a child process would 
not properly exit on a failed call to execvp.We attempted to fix this issue by changing the
wait system call to waitpid and use the WEXISTATUS macro to determine the success of the 
child process. The child process was killed using the exit command.

Also, the rshell does not support the cd command.
The second bug occured when we were testing for a nonexistent file or directory. The system would
output "stat: No such file or directory" instead of "(False)".
