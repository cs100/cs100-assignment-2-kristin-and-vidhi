#ifndef __DOUBLEOUTRED_CPP__
#define __DOUBLEOUTRED_CPP__

#include <fcntl.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "Connector.h"
#include "DoubleOutRed.h"

using namespace std;

DoubleOutRed::DoubleOutRed(Base* lhs, Base* rhs): Connector(lhs, rhs) {
    
}

DoubleOutRed::DoubleOutRed(): Connector() {
    
}

bool DoubleOutRed::execute(int in, int out) {
    string outfile = rhs -> get_data();
    out = open(outfile.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    return lhs -> execute(0, out);
}

#endif