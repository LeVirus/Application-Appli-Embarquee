#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "websocketpp/server.hpp"
#include "websocketpp/config/asio_no_tls.hpp"
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include "LogStream.h"

using namespace std;

class WebsocketServer
{
	public:
		static bool init();
		static void run();
		static void stop();
	 
		static bool sendClose(string id);
		static bool sendData(string id, string data);
         
	private:
		static bool getWebsocket(const string &id, websocketpp::connection_hdl &hdl);
		 
		static websocketpp::server<websocketpp::config::asio> server;
		//static pthread_rwlock_t websocketsLock;
		static boost::shared_mutex websocketsLock;
		static map<string, websocketpp::connection_hdl> websockets;
		static LogStream ls;
		static ostream os;
		 
		// callbacks
		static bool on_validate(websocketpp::connection_hdl hdl);
		static void on_fail(websocketpp::connection_hdl hdl);
		static void on_close(websocketpp::connection_hdl hdl);
		static void on_message(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr message);
};

#endif