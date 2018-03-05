#ifndef __AND_CPP__
#define __AND_CPP__

#include "Connector.h"
#include "AND.h"

AND::AND (Base* lhs, Base* rhs): Connector (lhs,rhs){}
AND::AND (): Connector (){}

bool AND::execute () {
    if (lhs->execute ()){
       if (rhs->execute ()){
           return true;
       }
       else {
           return false;
       }
    }
    else {
        return false;
     }
}
#endif
