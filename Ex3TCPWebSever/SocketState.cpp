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

/*!
 * Initializes an empty socket.
 * @param socket_id - The ID of the new socket.
 * @param recv_state - The recieve state of the socket.
 */
void SocketState::initializeSocket(SOCKET socket_id, SocketState::RecvState recv_state) {
    this->socket_id = socket_id;
    this->recv_state = recv_state;
    this->send_state = SocketState::SendState::IDLE;
    delete http_request;
    this->http_request = nullptr;
}

void SocketState::removeSocket() {
    this->recv_state = RecvState::EMPTY;
    this->send_state = SendState::EMPTY;
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

void SocketState::setReceiveState(SocketState::RecvState newState) {
    recv_state = newState;
}

void SocketState::setSendState(SocketState::SendState newState) {
    send_state = newState;
}

void SocketState::receiveMessage() throw(SocketException) {
    char buffer[1024];

    int bytes_recv = recv(socket_id, buffer, sizeof(buffer), 0);

    if (SOCKET_ERROR == bytes_recv) {
        throw SocketReceiveException(socket_id);
    }
    if (bytes_recv == 0) {
        throw ConnectionClosedException(socket_id);
    } else {
        buffer[bytes_recv] = '\0';
        cout << "HTTP Server: Received: " << bytes_recv << " bytes of \"" << buffer << "\" message.\n";

        delete http_request;
        http_request = new HttpRequest(buffer);
    }
}

void SocketState::sendMessage() {
    int bytesSent = 0;

    HttpResponse response(*http_request);
    bytesSent = send(socket_id, response.getContent().c_str(), response.getContent().length(), 0);

    if (SOCKET_ERROR == bytesSent) {
        cout << "HTTP Server: Error at send(): " << WSAGetLastError() << endl;
        return;
    }

    cout << "HTTP Server: Sent: " << bytesSent << "\\" << response.getContent().length() << " bytes of \"" << response.getContent().c_str()
         << "\" message.\n";
}


/*
void SocketState::handleRequest(char *sendBuffer) {
    if (http_request) {
        try {
            switch (http_request->getRequestType()) {
                case (HttpRequest::RequestType::OPTIONS):
                    options(sendBuffer);
                    break;
                case (HttpRequest::RequestType::GET):
                    get(sendBuffer);
                    break;
                case (HttpRequest::RequestType::POST):
                    post(sendBuffer);
                    break;
                case (HttpRequest::RequestType::HEAD):
                    head(sendBuffer);
                    break;
                case (HttpRequest::RequestType::DELETE_):
                    delete_(sendBuffer);
                    break;
                case (HttpRequest::RequestType::PUT):
                    put(sendBuffer);
                    break;
                case (HttpRequest::RequestType::TRACE):
                    trace(sendBuffer);
                    break;
                default:
                    throw BadRequestException();
            }
        }
        catch (ResponseException& e) {
            handleResponseException(e.getPageDir(), e.what());
        }
    }
}


void SocketState::options(char *message) throw (ResponseException) {
    string content;
    ostringstream oss;

    if (http_request->getUrl() == "*") {
        content = "Allow: OPTIONS, GET, HEAD, POST, PUT, TRACE, DELETE";
    } else {
        ifstream file(ROOT_DIR + http_request->getUrl());
        if (file.good()) {
            content = "Allow: OPTIONS, GET, HEAD, DELETE, TRACE";
        } else {
            throw NotFoundException();
        }
        file.close();
    }

    oss << http_request->getHttpVersionAsString() << "200 OK\r\n"
        << "Date: " << timeStr << "\r\n"
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << "0" << "\r\n"
        << "Content-Type: " << "text/html" << "\r\n"
        << content
        << "\r\n"
        << "\r\n";
    string output = oss.str();
    strcpy(message, output.c_str());
}

void SocketState::get(char *message) throw (ResponseException) {
    string content;
    ostringstream oss;

    ifstream file(ROOT_DIR + http_request->getUrl());
    if (file.good()) {
        content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    } else {
        throw NotFoundException();
    }
    file.close();

    oss << http_request->getHttpVersionAsString() << " " << "200 OK\r\n"
        << "Date: " << timeStr << "\r\n"
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << content.length() << "\r\n"
        << "Content-Type: " << "text/html"
        << "\r\n"
        << "\r\n"
        << content;

    string output = oss.str();
    strcpy(message, output.c_str());
}

void SocketState::head(char *message) throw (ResponseException) {
    string content;
    ostringstream oss;

    ifstream file(ROOT_DIR + http_request->getUrl());
    if (file.good()) {
        content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    } else {
        throw NotFoundException();
    }
    file.close();

    oss << http_request->getHttpVersionAsString() << " " << "200 OK\r\n"
        << "Date: " << timeStr << "\r\n"
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << content.length() << "\r\n"
        << "Content-Type: " << "text/html"
        << "\r\n"
        << "\r\n";
    string output = oss.str();
    strcpy(message, output.c_str());
}

void SocketState::delete_(char *message) throw (ResponseException){
    ostringstream oss;
    char *remove_dir;

    remove_dir = strcat(ROOT_DIR, http_request->getUrl().c_str());
    if (remove(remove_dir) != 0) {
        throw NotFoundException();
    }

    oss << http_request->getHttpVersionAsString() << " " << "204 NO CONTENT\r\n"
        << "Date: " << timeStr << "\r\n"
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << 0 << "\r\n"
        << "Content-Type: " << "http/message"
        << "\r\n"
        << "\r\n";

    string output = oss.str();
    strcpy(message, output.c_str());
}

void SocketState::trace(char *sendBuffer) throw (ResponseException) {
    ostringstream oss;
}

void SocketState::post(char *sendBuffer) throw (ResponseException) {
}

void SocketState::put(char *sendBuffer) throw (ResponseException) {

}

void SocketState::handleResponseException(const string& page_dir, const string& status) {
    string content;
    ostringstream oss;

    ifstream file(page_dir);
    if (file.good()) {
        content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    }
    file.close();

    oss << http_request->getHttpVersionAsString() << " " << status << "\r\n"
        << "Date: " << timeStr << "\r\n"
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << content.length() << "\r\n"
        << "Content-Type: " << "text/html"
        << "\r\n"
        << "\r\n"
        << content;

    string output = oss.str();
    strcpy(message, output.c_str());
}
*/
