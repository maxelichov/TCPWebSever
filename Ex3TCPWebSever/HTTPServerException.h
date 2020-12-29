#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

class HTTPServerException : public std::exception 
{

public:
	const char *what() const noexcept override = 0;
};


class WSADataException : public HTTPServerException 
{
public:
	const char *what() const noexcept override { return "HTTP Server: Error at WSAStartup()\n"; }
};


class InvalidSocketException : public HTTPServerException 
{
public:
	const char *what() const noexcept override { return "HTTP Server: Error creating socket\n"; }
};

class SocketException : public HTTPServerException 
{
protected:
	SOCKET socket;
public:
	const char *what() const noexcept override { return "Socket error.\n"; }
	SocketException(const SOCKET &socket) : socket(socket) {}
	SOCKET getSocket() { return socket; }
};

class SocketBindException : public SocketException 
{
public:
	SocketBindException(const SOCKET& socket) : SocketException(socket) {}
	const char *what() const noexcept override { return "HTTP Server: Error at bind()\n"; }
};


class SocketListenException : public SocketException 
{
public:
	SocketListenException(const SOCKET& socket) : SocketException(socket) {}
	const char *what() const noexcept override { return "HTTP Server: Error at listen()\n"; }
};

class SocketReceiveException : public SocketException 
{
public:
	SocketReceiveException(const SOCKET& socket) : SocketException(socket) {}
	const char *what() const noexcept override { return "HTTP Server: Error at recv()\n"; };
};

class ConnectionClosedException : public SocketException 
{
public:
	ConnectionClosedException(const SOCKET& socket) : SocketException(socket) {}
	const char *what() const noexcept override { return "HTTP Server: connection at socket closed.\n"; }
};
