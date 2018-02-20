#ifndef __CMD_CPP__
#define __CMD_CPP__

#include <queue>
#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include "Base.h"
#include "Cmd.h"

using namespace std;

Cmd::Cmd (char* command, queue<char *> flags) {
    this -> command = command;
    this -> flags = flags;
}

Cmd::Cmd(char* command) {
    this->command = command;
    queue<char*> temp;
    this->flags = temp;
}

Cmd::Cmd() {
    
}

void Cmd::add_flag(char* a) {
    flags.push(a);
}

string Cmd::getCommand() {
    return command;
}

bool Cmd::execute () {
    flags.push(NULL);
    int ctr = 1;
    char *args[500];
    while (flags.size() != 0) {
        args[ctr] = flags.front();
        flags.pop();
        ctr++;
    }
    args[0] = command;
    
    bool ret = true;
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
    }
    else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            ret = false;
            perror("execvp");
            exit(1);
        }
    }
    else if (pid > 0) {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("wait");
        }
        if (WEXITSTATUS(status) == 1) {
            ret = false;
        }
    }
    return ret;
}
#endif