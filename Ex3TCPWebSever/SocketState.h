#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <sstream>
#include <iterator>
#include <vector>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HTTPServerException.h"
#include "ResponseExceptions.h"

using namespace std;


class SocketState
{
public:
	enum class RecvState { EMPTY, LISTEN, RECEIVE };
	enum class SendState { EMPTY, IDLE, SEND };

private:
    const static int SEND_BUFFER_SIZE = 4096;

    SOCKET      socket_id;
	RecvState   recv_state;
	SendState   send_state;
	HttpRequest *http_request;

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
	void removeSocket();
	void receiveMessage()               throw (SocketException);

private:
/*	void handleRequest(char* sendBuffer);
	void options( char* sendBuffer)         throw (ResponseException);
	void get(char* sendBuffer)              throw (ResponseException);
	void head( char* sendBuffer)            throw (ResponseException);
	void delete_( char* sendBuffer)         throw (ResponseException);
	void trace( char* sendBuffer)           throw (ResponseException);
	void post( char* sendBuffer)            throw (ResponseException);
	void put( char* sendBuffer)             throw (ResponseException);

	void handleResponseException(const string& page_dir, const string& status);*/

	

	

};
