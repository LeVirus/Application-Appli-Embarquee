#include <iostream>
#include "WebsocketServer.h"
#include "RecoProcess.h"

int main(int argc, char* argv[])
{
	WebsocketServer::init();
	WebsocketServer::run();
	return 0;
	/* EXEMPLE
	RecoProcess rp;
    char** debug;
    debug = new char*[1];
    debug[0] = new char;
        debug[0] = "";
    
    rp.process(0,debug);
    delete debug[0];
        delete debug;*/
}
