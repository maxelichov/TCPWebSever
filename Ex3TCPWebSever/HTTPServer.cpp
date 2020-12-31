#include "HTTPServer.h"

HTTPServer::HTTPServer(int max_sockets) throw (HTTPServerException) : max_sockets(max_sockets), sockets_count(0) {
	this->sockets = new SocketState[max_sockets];
	initWSAData();
	this->listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->server_service.sin_family = AF_INET;
	this->server_service.sin_addr.s_addr = INADDR_ANY;
	this->server_service.sin_port = htons(HTTP_PORT);
	initListenSocket();
}

HTTPServer::~HTTPServer() {
	cout << "Time Server: Closing Connection.\n";
	closesocket(listen_socket);
	WSACleanup();
	delete[] sockets;
}

//TODO: make exceptions.
void HTTPServer::initListenSocket() throw (HTTPServerException) {
	if (INVALID_SOCKET == listen_socket) {
		throw InvalidSocketException();
	}

	if (SOCKET_ERROR == bind(listen_socket, (SOCKADDR *)&server_service, sizeof(server_service))) {
		throw SocketBindException(listen_socket);
	}

	if (SOCKET_ERROR == listen(listen_socket, QUEUE_SIZE)) {
		throw SocketListenException(listen_socket);
	}

	addSocket(listen_socket, SocketState::RecvState::LISTEN);
}

void HTTPServer::initWSAData() throw (WSADataException) {
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
		throw WSADataException();
	}
}

void HTTPServer::run() {
	while (true) {

		fd_set waitRecv;
		FD_ZERO(&waitRecv);
		for (int i = 0; i < max_sockets; i++) {
			if ((sockets[i].getRecvState() == SocketState::RecvState::LISTEN) ||
				(sockets[i].getRecvState() == SocketState::RecvState::RECEIVE)) {
				FD_SET(sockets[i].getSocketID(), &waitRecv);
			}
		}

		fd_set waitSend;
		FD_ZERO(&waitSend);
		for (int i = 0; i < max_sockets; i++)
		{
			if (sockets[i].getSendState() == SocketState::SendState::SEND)
				FD_SET(sockets[i].getSocketID(), &waitSend);
		}

		int nfd;
		nfd = select(0, &waitRecv, &waitSend, NULL, NULL);

		if (nfd == SOCKET_ERROR)
		{
			cout << "HTTP Server: Error at select(): " << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}

		for (int i = 0; i < max_sockets && nfd > 0; i++) {
			if (FD_ISSET(sockets[i].getSocketID(), &waitRecv)) {
				nfd--;
				switch (sockets[i].getRecvState()) {
				case SocketState::RecvState::LISTEN:
					acceptConnection(i);
					break;

				case SocketState::RecvState::RECEIVE:
					try 
					{
						receiveMessage(i);
						sockets[i].setReceiveState(SocketState::RecvState::EMPTY);
						sockets[i].setSendState(SocketState::SendState::SEND);
					}
					catch (SocketException& e) {
						cout << e.what();
						closesocket(e.getSocket());			
						removeSocket(i);
					}
					break;
				}
			}
		}

		for (int i = 0; i < max_sockets && nfd > 0; i++) {
			if (FD_ISSET(sockets[i].getSocketID(), &waitSend)) 
			{
				nfd--;
				switch (sockets[i].getSendState()) {
				case SocketState::SendState::SEND:
					sendMessage(i);
					sockets[i].setSendState(SocketState::SendState::IDLE);
					break;
				}
			}
		}
	}
}


void HTTPServer::changeToSendState(int i)
{
	sockets[i].setReceiveState(SocketState::RecvState::EMPTY);
	sockets[i].setSendState(SocketState::SendState::SEND);
}

void HTTPServer ::sendMessage(int i)
{
	sockets[i].sendMessage();
}

bool HTTPServer::addSocket(SOCKET id, SocketState::RecvState recv_state) {
	for (int i = 0; i < max_sockets; i++) {
		if (this->sockets[i].getRecvState() == SocketState::RecvState::EMPTY) {
			sockets[i].initializeSocket(id, recv_state);
			this->sockets_count++;
			return true;
		}
	}
	return false;
}

bool HTTPServer::removeSocket(int index) {
    sockets[index].removeSocket();
	sockets_count--;
	return true;
}

void HTTPServer::acceptConnection(int index) {
	SOCKET id = sockets[index].getSocketID();
	struct sockaddr_in from;		// Address of sending partner
	int from_len = sizeof(from);

	SOCKET msg_socket = accept(id, (struct sockaddr *)&from, &from_len);
	if (INVALID_SOCKET == msg_socket) {
		cout << "HTTP Server: Error at accept(): " << WSAGetLastError() << endl;
		return;
	}
	cout << "HTTP Server: Client " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port) << " is connected." << endl;

	// Set the socket to be in non-blocking mode.
	unsigned long flag = 1;
	if (ioctlsocket(msg_socket, FIONBIO, &flag) != 0) 
	{
		cout << "HTTP Server: Error at ioctlsocket(): " << WSAGetLastError() << endl;
	}

	if (!addSocket(msg_socket, SocketState::RecvState::RECEIVE))
	{
		cout << "\t\tToo many connections, dropped!\n";
		closesocket(id);
	}
}

void HTTPServer::receiveMessage(int index) throw (SocketException) 
{
	sockets[index].receiveMessage();
}