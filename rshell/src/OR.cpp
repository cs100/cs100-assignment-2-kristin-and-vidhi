#ifndef __OR_CPP__
#define __OR_CPP__

#include "Connector.h"
#include "OR.h"

OR::OR(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
OR::OR(): Connector() {}

bool OR::execute(int in, int out) {
    if (!lhs->execute(in,out)) {
        if (rhs->execute(in, out)) {
            return true;
        }
        
        else {
            return false;
        }
    }
    
    return true;
}


#endif
