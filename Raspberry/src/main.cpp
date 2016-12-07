#include <iostream>
#include "WebsocketServer.h"


int main(int argc, char* argv[])
{
	WebsocketServer::init();
	WebsocketServer::run();
	return 0;
}