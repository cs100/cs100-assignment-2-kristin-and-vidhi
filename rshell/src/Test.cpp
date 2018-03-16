#ifndef __TEST_CPP__
#define __TEST_CPP__

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <sstream>
#include <dirent.h>
#include <iostream>

using namespace std;

#include "Test.h"
#include "Base.h"

Test::Test (char* command, queue<char *> flags) {
    this->command = command;
    this->flags = flags;
}

Test::Test (queue<char*> flags) {
    this->flags = flags;
}

Test::Test () {}

void Test::add_flag (char*a) {
    flags.push (a);
}

string Test::get_data() {
    return command;
}

bool Test::execute (int n, int out) {
    struct stat buf;
    string flag;
    string path;
    bool exists = false;

    if (flags.size () == 1) {
        flag = "-e";
        path = flags.front ();
    }
    
    else {
        flag = flags.front ();
        flags.pop ();
        path = flags.front ();
    }

    int statret = stat (path.c_str (),&buf);
    
    if  (statret == 0) {
        exists = true;
    }
    
    if (dup2(out,1) == -1) {
        perror ("dup2");
        return false;
    }
    
    if (statret == -1) {
        perror ("stat");
        return false;
    }

    if (flag == "-e") {
        cout << " (True)" << endl;
        return exists;
    }
    
    else if (flag == "-d") {
        if (S_ISDIR (buf.st_mode)) {
            cout << " (True)" << endl;
            return true;
        }
        else {
            cout << " (False)" << endl;
            return false;
        }
    }
    
    else if (flag == "-f") {
        
         if (S_ISREG (buf.st_mode)) {
             cout << " (True)" << endl;
             return true;
         }
         
        else {
            cout << " (False)" << endl;
            return false;
        }
    }
    
    else {
        cout << "Error: This flag, " << flag << ", is not valid."  << endl;
        return false;
    }
}

#endif