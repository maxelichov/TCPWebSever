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

	void remove();
	void receiveMessage()               throw (SocketException);

private:
	void handleRequest(const char* sendBuffer);
	void options(const char* sendBuffer);
	void get(const char* sendBuffer);
	void head(const char* sendBuffer);
	void delete_(const char* sendBuffer);
	void trace(const char* sendBuffer);
	void connect(const char* sendBuffer);
	void post(const char* sendBuffer);
	void put(const char* sendBuffer);
	

};
