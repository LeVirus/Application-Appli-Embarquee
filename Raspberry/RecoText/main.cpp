#include "RecoProcess.h"

int main()
{
    RecoProcess rp;
    char** debug;
    debug = new char*[1];
    debug[0] = new char;
        debug[0] = "";
    
    rp.process(0,debug);
    delete debug[0];
        delete debug;
}
