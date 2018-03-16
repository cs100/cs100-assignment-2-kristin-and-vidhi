#ifndef __CMD_CPP__
#define __CMD_CPP__

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

#include "Base.h"
#include "Cmd.h"

Cmd::Cmd(char* command, queue<char *> flags) {
    this->command = command;
    this->flags = flags;
}

Cmd::Cmd(queue<char*> flags) {
    this->flags = flags;
}

Cmd::Cmd(char* command) {
    this->command = command;
    queue<char*> temp;
    this->flags = temp;
}

Cmd::Cmd() {}

void Cmd::add_flag(char*a) {
    flags.push(a);
}

string Cmd::get_data() {
    return command;
}

bool Cmd::execute(int in, int out) {

    flags.push(NULL);
    int ctr = 1;
    char *args[500];
    while(flags.size() != 0) {
        args[ctr] = flags.front();
        flags.pop();
        ctr++;
    }

    args[0] = command;
    bool ret = true;

    pid_t pid = fork();
    if(pid == -1) {
        perror("fork");
    }
    else if (pid == 0) {

        if(dup2(in,0) == -1) {
            perror("dup2");
            return false;
        }
        if(dup2(out,1) == -1) {
            perror("dup2");
            return false;
        }

        if(execvp(args[0], args) == -1) {
            ret = false;
            perror("execvp");
            exit(1);
        }
    }
    else if(pid > 0) {
       int status;
       if(waitpid(pid,&status,0) == -1) {
           perror("wait");
       }
       if(WEXITSTATUS(status) != 0) {
           ret = false;
       }

    }
    return ret;
}
#endif