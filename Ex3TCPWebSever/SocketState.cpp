#include "SocketState.h"
#include <iterator>
#include <fstream>
using namespace std;

SocketState::SocketState() {
	this->recv_state = RecvState::EMPTY;
	this->send_state = SendState::EMPTY;
	this->socket_id = 0;
	this->http_request = nullptr;
}

/*
 * Initializes an empty socket which has already been created.
 */
void SocketState::initializeSocket(SOCKET socket_id, SocketState::RecvState recv_state) {
	this->socket_id = socket_id;
	this->recv_state = recv_state;
	this->send_state = SocketState::SendState::IDLE;
	delete http_request;
	this->http_request = nullptr;
}

SOCKET SocketState::getSocketID() const {
	return socket_id;
}

SocketState::RecvState SocketState::getRecvState() const {
	return recv_state;
}

SocketState::SendState SocketState::getSendState() const {
	return send_state;
}

const HttpRequest *SocketState::getHttpRequest() const {
	return http_request;
}

void SocketState::remove() {
	this->recv_state = RecvState::EMPTY;
	this->send_state = SendState::EMPTY;
	delete http_request;
	this->http_request = nullptr;
}

void SocketState::receiveMessage() throw (SocketException) {
	char buffer[1024];
	
	
	int bytes_recv = recv(socket_id, buffer, sizeof(buffer), 0);

	if (SOCKET_ERROR == bytes_recv) {
		throw SocketReceiveException(socket_id);
	}
	if (bytes_recv == 0) {
		throw ConnectionClosedException(socket_id);
	}
	else 
	{
		buffer[bytes_recv] = '\0';
		cout << "HTTP Server: Received: " << bytes_recv << " bytes of \"" << buffer << "\" message.\n";

		delete http_request;
		http_request = new HttpRequest(buffer);

		
	}
}


// Could make HttpRequest class abstract with a virtual function called handle().
// Each request type will inherit from HttpRequest ie. PutHttpRequest and will implement handle() differently.
// Then call http_request.handle().
void SocketState::handleRequest(const char* sendBuffer)
{
	if (http_request) {
		switch (http_request->getRequestType()) {
		case (HttpRequest::RequestType::OPTIONS):
			options( sendBuffer);
			break;
		case (HttpRequest::RequestType::GET):
			get( sendBuffer);
			break;
		case (HttpRequest::RequestType::POST):
			post( sendBuffer);
			break;
		case (HttpRequest::RequestType::HEAD):
			head( sendBuffer);
			break;
		case (HttpRequest::RequestType::DELETE_):
			delete_( sendBuffer);
			break;
		case (HttpRequest::RequestType::PUT):
			put( sendBuffer);
			break;
		case (HttpRequest::RequestType::CONNECT):
			connect( sendBuffer);
			break;
		case (HttpRequest::RequestType::TRACE):
			trace( sendBuffer);
			break;
		case (HttpRequest::RequestType::UNDEFINED):
			///throw an exception?
			return;
		}
	}
}

void SocketState::sendMessage()
{
	int bytesSent = 0;
	char sendBuffer[255];
	handleRequest(sendBuffer);
	
	bytesSent = send(socket_id, sendBuffer, (int)strlen(sendBuffer), 0);
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "HTTP Server: Error at send(): " << WSAGetLastError() << endl;
		return;
	}

	cout << "HTTP Server: Sent: " << bytesSent << "\\" << strlen(sendBuffer) << " bytes of \"" << sendBuffer << "\" message.\n";

	send_state = SendState::IDLE;
}





void SocketState::options(const char* sendBuffer)
{

}

void SocketState::get(const char* message)
{
	int clientSocket = socket_id;
	
	
	
		istringstream iss(message);
		vector<string> parsed((istream_iterator<string>(iss)), istream_iterator<string>()); // vector of strings. each string until sp.
		ifstream file(".\\wwwroot\\index.html");
		string content = "<h1>404 Not Found</h1>";
		int errorcode = 404;

		
			if (file.good()) // if the file is good we wil return the content of the file.
			{
				string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); 
				content = str;
			}


			file.close();
		

		ostringstream oss;
		//HTTP headers
		oss << "HTTP/1.1 200 ok\r\n";
		oss << "Cache-Control: no cache, private\r\n";
		oss << "Content-Type: text/html\r\n";
		 oss << "Allow: " / // TODO
			oss << "Content-Length " << content.size() << "\r\n"; // content consists of all the names of the methods this server can perform.
		oss << "\r\n";
		oss << content;
		string output = oss.str();
		int size = output.size() + 1;

		
	
}

void SocketState::head(const char* sendBuffer) {

}

void SocketState::delete_(const char* sendBuffer) {

}

void SocketState::trace(const char* sendBuffer) {

}

void SocketState::connect(const char* sendBuffer) 
{
	
}

void SocketState::post(const char* sendBuffer) {

}

void SocketState::put(const char* sendBuffer) {

}
