#ifndef __SEMICOLON_CPP__
#define __SEMICOLON_CPP__

#include "Connector.h"
#include "Semicolon.h"

Semicolon::Semicolon(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
Semicolon::Semicolon(): Connector() {}

bool Semicolon::execute(int in, int out) {
    lhs->execute(in, out);
    if (rhs->execute(in, out)) {
        return true;
    }
    
    else {
        return false;
   }
}

#endif
