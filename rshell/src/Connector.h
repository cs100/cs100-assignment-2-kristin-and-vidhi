#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__
#include <cstdlib>
#include "Base.h"

using namespace std;

class Connector: public Base {
    protected:
        Base* lhs;
        Base* rhs;
    public:
        Connector(Base* lhs, Base* rhs);
        Connector();
        virtual bool execute() = 0;
};

#endif