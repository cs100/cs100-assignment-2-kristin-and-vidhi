#ifndef __SINGLEOUTRED_CPP__
#define __SINGLEOUTRED_CPP__

#include "Connector.h"
#include "SingleOutRed.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>

using namespace std;

SingleOutRed::SingleOutRed(Base* lhs, Base* rhs): Connector(lhs, rhs){}
SingleOutRed::SingleOutRed(): Connector(){}

bool SingleOutRed::execute(int n, int out){
    string outfs = rhs->get_data();
    out = open(outfs.c_str(),O_WRONLY| O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    
    return lhs->execute(0, out);
}

#endif