#ifndef __AND_H__
#define __AND_H__

#include "Connector.h"

class AND: public Connector {
    public:
        AND(Base* lhs, Base* rhs);
        AND();
        virtual bool execute(int in, int out);
};

#endif
