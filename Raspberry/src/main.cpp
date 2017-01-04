#include <iostream>
#include "WebsocketServer.h"
//#include "RecognitionSystem.h"

int main(int argc, char* argv[])
{
	WebsocketServer::init();
	WebsocketServer::run();
	/*RecognitionSystem rs;
	rs.recognizeFromPic( "./chapeau.jpg");*/
	return 0;
}
