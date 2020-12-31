#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <sstream>
#include <iterator>
#include <vector>

#include "HttpRequest.h"
#include "HTTPServerException.h"

using namespace std;


class SocketState
{
public:
	enum class RecvState { EMPTY, LISTEN, RECEIVE };
	enum class SendState { EMPTY, IDLE, SEND };

private:
	SOCKET      socket_id;
	RecvState   recv_state;
	SendState   send_state;
	HttpRequest *http_request;      ///Does request come in one packet? can it be divided into many?


public:
	SocketState();
	void initializeSocket(SOCKET socket_id, RecvState recv_state);
	void sendMessage();
	SOCKET getSocketID()		        const;
	RecvState getRecvState()	        const;
	SendState getSendState()	        const;
	const HttpRequest *getHttpRequest() const;
	void setReceiveState(SocketState::RecvState newState);
	void setSendState(SocketState::SendState newState);
	void remove();
	void receiveMessage()               throw (SocketException);

private:
	void handleRequest(char* sendBuffer);
	void options( char* sendBuffer);
	void get(char* sendBuffer);
	void head( char* sendBuffer);
	void delete_( char* sendBuffer);
	void trace( char* sendBuffer);
	void connect( char* sendBuffer);
	void post( char* sendBuffer);
	void put( char* sendBuffer);
	

	

};
