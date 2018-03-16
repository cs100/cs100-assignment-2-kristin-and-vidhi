#ifndef __SEMICOLON_H__
#define __SEMICOLON_H__
#include "Connector.cpp"

class Semicolon: public Connector {
    public:
        Semicolon(Base* lhs, Base* rhs);
        Semicolon();
    
        virtual bool execute(int n, int out);
};

#endif
