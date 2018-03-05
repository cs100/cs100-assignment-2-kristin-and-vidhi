Vidhi Jain and Kristin Hsu created this Rshell project for CS100 Winter 2018. 
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


## Bugs

While testing our code, we experienced some bugs. One bug occurred where a child process would 
not properly exit on a failed call to execvp.We attempted to fix this issue by changing the
wait system call to waitpid and use the WEXISTATUS macro to determine the success of the 
child process. The child process was killed using the exit command.

Also, the rshell does not support the cd command.
The second bug occured when we were testing for a nonexistent file or directory. The system would
output "stat: No such file or directory" instead of "(False)".