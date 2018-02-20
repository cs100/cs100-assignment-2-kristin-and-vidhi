#ifndef __CMD_H__
#define __CMD_H__


#include <vector>
#include "Base.h"

using namespace std;


class Cmd: public Base{
    
    private:
        queue<char*> flags;
        char *command;
    
    
    public:
    Cmd(char* command, queue <char*> flags);
    Cmd(char* command);
    Cmd();
    
    void add_flag(char *a);
    virtual bool execute();
    string getCommand();
    
};

#endif