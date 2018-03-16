#ifndef __OR_H__
#define __OR_H__

#include "Connector.h"

class OR: public Connector{
    public:
    OR(Base* lhs, Base* rhs);
    OR();
    virtual bool execute(int in, int out);
};

#endif