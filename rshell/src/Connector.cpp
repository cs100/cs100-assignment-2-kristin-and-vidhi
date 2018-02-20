#ifndef __CONNECTOR_CPP__
#define __CONNECTOR_CPP__

#include "Connector.h"

Connector::Connector(Base* lhs, Base* rhs){
    this -> rhs = rhs;
    this -> lhs = lhs;
    
}

Connector::Connector(){
    this -> rhs = NULL;
    this -> lhs = NULL;
    
}

#endif