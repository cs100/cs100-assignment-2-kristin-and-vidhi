#ifndef __TEST_H__
#define __TEST_H__

using namespace std;
#include "Base.h"

class Test: public Base {
    protected:
    char *command;
    queue<char*> flags;

	public:
        Test(char *command, queue<char*> flags);
        Test(queue<char*> flags);
        Test();
        void add_flag(char *a);
        virtual bool execute();
    
        //Debugging Func
        string getCommand();
};
#endif
