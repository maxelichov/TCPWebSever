#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include "HTTPServer.h"

int main()
{
	try {
		HTTPServer http_server(10);
		http_server.run();
	}
	catch (SocketException& e) {
		cout << e.what();
		closesocket(e.getSocket());
		WSACleanup();
	}
	catch (HTTPServerException& e) {
		cout << e.what();
		WSACleanup();
	}
	return 0;
}

