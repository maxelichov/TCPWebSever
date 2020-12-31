#pragma warning(disable : 4996)
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <string>
#include <vector>
#include "SocketState.h"
#include "HTTPServerException.h"

#define QUEUE_SIZE              5
#define HTTP_PORT			    80
#define HTTP_REPLACEMENT_PORT   8080

using namespace std;


class HTTPServer
{
private:
	SocketState* sockets;
	SOCKET listen_socket;
	int max_sockets;
	int sockets_count;

	WSAData wsa_data;
	sockaddr_in server_service;

public:
	explicit HTTPServer(int max_sockets)    throw (HTTPServerException);
	virtual ~HTTPServer();

	void run();

private:
	void initWSAData()                      throw (WSADataException);
	void initListenSocket()                 throw (HTTPServerException);

	void receiveMessage(int index)          throw (SocketException);
	bool addSocket(SOCKET id, SocketState::RecvState recv_state);
	bool removeSocket(int index);
	void acceptConnection(int index);
	void sendMessage(int index);
	void changeToSendState(int index);
};
