#ifndef __OR_CPP__
#define __OR_CPP__

#include "Connector.h"
#include "OR.h"

OR::OR(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
OR::OR(): Connector() {}

bool OR::execute(int n, int out) {
    if (!lhs->execute(n,out)) {
        if (rhs->execute(n, out)) {
            return true;
        }
        
        else {
            return false;
        }
    }
    
    return true;
}


#endif
