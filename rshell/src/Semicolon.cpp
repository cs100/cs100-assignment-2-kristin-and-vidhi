#ifndef __SEMICOLON_CPP__
#define __SEMICOLON_CPP__

#include "Connector.h"
#include "Semicolon.h"

Semicolon::Semicolon(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
Semicolon::Semicolon(): Connector() {}

bool Semicolon::execute(int n, int out) {
    lhs->execute(n, out);
    if (rhs->execute(n, out)) {
        return true;
    }
    
    else {
        return false;
   }
}

#endif
