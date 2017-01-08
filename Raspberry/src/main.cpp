#include <iostream>
#include "WebsocketServer.h"
#include "RecoProcess.h"

int main(int argc, char* argv[])
{
	//EXEMPLE
	RecoProcess rp;
    char** debug;
    debug = new char*[1];
    debug[0] = new char;
        debug[0] = "";
    
    rp.process(0,debug);
    delete debug[0];
        delete debug;
	WebsocketServer::init();
	WebsocketServer::run();
	/*RecognitionSystem rs;
	rs.recognizeFromPic( "./chapeau.jpg");*/
	return 0;
}
