#ifndef __CMD_H__
#define __CMD_H__

#include <queue>
#include <string>
using namespace std;
#include "Base.h"

class Cmd: public Base {
    protected:
        char *command; 
        queue<char*> flags;//holds flags prvided
        public:
        Cmd(char *command, queue<char*> flags);
        Cmd(queue<char*> flags);
        Cmd(char *command);
        Cmd();
        
        

        void add_flag(char *a);
        virtual bool execute();
        string getCommand();

};
#endif
