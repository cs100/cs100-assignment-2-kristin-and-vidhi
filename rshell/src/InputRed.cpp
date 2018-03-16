#ifndef __INPUTRED_CPP__
#define __INPUTRED_CPP__

#include "Connector.h"
#include "InputRed.h"

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>

using namespace std;

InputRed::InputRed(Base* lhs, Base* rhs): Connector(lhs, rhs){}
InputRed::InputRed(): Connector(){}

bool InputRed::execute(int in, int out){
    string instream = rhs->get_data();
    in = open(instream.c_str(), O_RDONLY);
    return lhs->execute(in,1);
}
#endif