#ifndef __AND_H__
#define __AND_H__

#include "Connector.h"


//kinda the same as the otherss
class AND:public Connector{
    public:
    AND(Base* lhs, Base* rhs);
    AND();
    
    
    
    virtual bool execute();
    
    
    
    
};

#endif