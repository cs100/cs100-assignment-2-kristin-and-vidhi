//Semicolon.cpp

#ifndef __SEMICOLON_CPP__
#define __SEMICOLON_CPP__
#include "Connector.h"
#include "Semicolon.h"

Semicolon::Semicolon(Base* lhs, Base* rhs): Connector(lhs, rhs) 
{}

Semicolon::Semicolon(): Connector()
{}


//execute
bool Semicolon::execute(){
lhs -> execute();
if (rhs->execute()){
return true;
}
else
{
return false;
}
}


#endif

//****************************