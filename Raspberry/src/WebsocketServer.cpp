#include "WebsocketServer.h"

// static initialisations
websocketpp::server<websocketpp::config::asio> WebsocketServer::server;
map<string, websocketpp::connection_hdl> WebsocketServer::websockets;
//pthread_rwlock_t WebsocketServer::websocketsLock = PTHREAD_RWLOCK_INITIALIZER;
boost::shared_mutex WebsocketServer::websocketsLock;
LogStream WebsocketServer::ls;
ostream WebsocketServer::os(&ls);
     
// namespace merging
using websocketpp::connection_hdl;

bool WebsocketServer::init()
{
    // Initialising WebsocketServer.
    server.init_asio();
	
	// Set custom logger (ostream-based).
	server.get_alog().set_ostream(&os);
	server.get_elog().set_ostream(&os);
	
	// Register the message handlers.
	server.set_validate_handler(&WebsocketServer::on_validate);
	server.set_fail_handler(&WebsocketServer::on_fail);
	server.set_close_handler(&WebsocketServer::on_close);
	server.set_message_handler(&WebsocketServer::on_message);
	
	// Listen on port.
	int port = 8082;
	try {
		server.listen(port);
	} catch(websocketpp::exception const &e) {
		// Websocket exception on listen. Get char string via e.what().
	}
	
	// Starting Websocket accept.
    websocketpp::lib::error_code ec;
    server.start_accept(ec);
    if (ec) {
        // Can log an error message with the contents of ec.message() here.
        return false;
    }
     
    return true;
}

void WebsocketServer::run()
{
    try {
        server.run();
    } catch(websocketpp::exception const &e) {
        // Websocket exception. Get message via e.what().
    }
}

void WebsocketServer::stop()
{
    // Stopping the Websocket listener and closing outstanding connections.
    websocketpp::lib::error_code ec;
    server.stop_listening(ec);
    if (ec) {
        // Failed to stop listening. Log reason using ec.message().
        return;
    }
    
	boost::shared_lock<boost::shared_mutex> lock(websocketsLock);
    // Close all existing websocket connections.
    string data = "Terminating connection...";
    map<string, connection_hdl>::iterator it;
    for (it = websockets.begin(); it != websockets.end(); ++it) {
        websocketpp::lib::error_code ec;
        server.close(it->second, websocketpp::close::status::normal, data, ec); // send text message.
        if (ec)
		{ 
			// we got an error
            // Error closing websocket. Log reason using ec.message().
        }
    }
     
    // Stop the endpoint.
    server.stop();
}

bool WebsocketServer::on_validate(connection_hdl hdl)
{
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = server.get_con_from_hdl(hdl);
    websocketpp::uri_ptr uri = con->get_uri();
    string query = uri->get_query(); // returns empty string if no query string set.
	string id = "0";
    /*if (!query.empty()) {
        // Split the query parameter string here, if desired.
        // We assume we extracted a string called 'id' here.
    }
    else {
        // Reject if no query parameter provided, for example.
        return false;
    }*/
     
    /*if (pthread_rwlock_wrlock(&websocketsLock) != 0) {
        // Failed to write-lock websocketsLock.
    }*/
	
	// get upgradable access
	boost::upgrade_lock<boost::shared_mutex> lock(websocketsLock);
	// get exclusive access
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
     
    websockets.insert(std::pair<string, connection_hdl>(id, hdl));
    /*if (pthread_rwlock_unlock(&websocketsLock) != 0) {
        // Failed to unlock websocketsLock.
    }*/
 
    return true;
}

void WebsocketServer::on_fail(connection_hdl hdl)
{
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = server.get_con_from_hdl(hdl);
    websocketpp::lib::error_code ec = con->get_ec();
    // Websocket connection attempt by client failed. Log reason using ec.message().
}
 
void WebsocketServer::on_close(connection_hdl hdl)
{
    // Websocket connection closed.
}

void WebsocketServer::on_message(connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr message)
{
    std::string msg = message->get_payload();
    std::cout << msg << std::endl;
    RequestAnalyser::analyse(msg);
}

bool WebsocketServer::sendData(string id, string data)
{
    connection_hdl hdl;
    if (!getWebsocket(id, hdl)) {
        // Sending to non-existing websocket failed.
        return false;
    }
     
    websocketpp::lib::error_code ec;
    server.send(hdl, data, websocketpp::frame::opcode::text, ec); // send text message.
    if (ec) { // we got an error
        // Error sending on websocket. Log reason using ec.message().
        return false;
    }
     
    return true;
}

bool WebsocketServer::getWebsocket(const string &id, websocketpp::connection_hdl &hdl)
{
	// get upgradable access
	boost::upgrade_lock<boost::shared_mutex> lock(websocketsLock);
	// get exclusive access
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	//pthread_rwlock_wrlock(&websocketsLock);
	map<string, connection_hdl>::iterator it = websockets.find(id);
	//pthread_rwlock_unlock(&websocketsLock);
	if (it == websockets.end()) { return false; }
	return true;
}

bool WebsocketServer::sendClose(string id)
{
    connection_hdl hdl;
    if (!getWebsocket(id, hdl)) {
        // Closing non-existing websocket failed.
        return false;
    }
     
    string data = "Terminating connection...";
    websocketpp::lib::error_code ec;
    server.close(hdl, websocketpp::close::status::normal, data, ec); // send close message.
    if (ec) { // we got an error
        // Error closing websocket. Log reason using ec.message().
        return false;
    }
    
	// get upgradable access
	boost::upgrade_lock<boost::shared_mutex> lock(websocketsLock);
	// get exclusive access
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	
    // Remove websocket from the map.
    //pthread_rwlock_rdlock(&websocketsLock);
    websockets.erase(id);
    //pthread_rwlock_unlock(&websocketsLock);
     
    return true;
}
