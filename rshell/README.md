# Assignment 2 - Rshell

## Project Introduction/Summary

Vidhi Jain and Kristin Hsu created this Rshell project for CS100 Winter 2018. 
Rshell is a command shell written in C++. 
It is able to take in commands and connectors and execute them.
Our design was based off the composite design pattern where the connector class and command class
correspond with the composite class and leaf, respectively.

## Installation

Run these commands

```
$ git clone https://github.com/cs100/cs100-assignment-2-kristin-and-vidhi.git
$ cd rshell
$ make
$ echo Now enter your commands
```

## Bugs

While testing our code, we did experience a bug. This bug occurred where a child process would not properly exit on a failed call to execvp.
We attempted to fix this issue by changing the wait system call to waitpid and use the WEXISTATUS macro to determine the success of the child process.
The child process was killed using the exit command.