#include "HttpRequest.h"

HttpRequest::HttpRequest(const char *buffer)
{
	istringstream iss(buffer);
	vector<string> parsed_message((istream_iterator<string>(iss)), istream_iterator<string>());

	this->url = parsed_message[HeaderIndex::URL];
	this->host = parsed_message[HeaderIndex::HOST];
	this->user_agent = parsed_message[HeaderIndex::USER_AGENT];
	parseRequestType(parsed_message[HeaderIndex::REQUEST_TYPE]);
	parseHttpVersion(parsed_message[HeaderIndex::HTTP_VERSION]);
	parseConnectionType(parsed_message[HeaderIndex::CONNECTION]);
	parseLanguage(parsed_message[HeaderIndex::ACCEPT_LANGUAGE]);
}

void HttpRequest::parseRequestType(const string& request_type) {
	if (request_type == "OPTIONS") {
		this->request_type = RequestType::OPTIONS;
	}
	else if (request_type == "GET") {
		this->request_type = RequestType::GET;
	}
	else if (request_type == "HEAD") {
		this->request_type = RequestType::HEAD;
	}
	else if (request_type == "POST") {
		this->request_type = RequestType::POST;
	}
	else if (request_type == "DELETE") {
		this->request_type = RequestType::DELETE_;
	}
	else if (request_type == "PUT") {
		this->request_type = RequestType::PUT;
	}
	else if (request_type == "TRACE") {
		this->request_type = RequestType::TRACE;
	}
	else if (request_type == "CONNECT") {
		this->request_type = RequestType::CONNECT;
	}
	else {
		this->request_type = RequestType::UNDEFINED;
	}
}

void HttpRequest::parseHttpVersion(const string& http_version) {
	if (http_version == "HTTP/1.1") {
		this->http_version = HttpVersion::HTTP_1_1;
	}
	else if (http_version == "HTTP/1.0") {
		this->http_version = HttpVersion::HTTP_1_0;
	}
	else {
		this->http_version = HttpVersion::UNDEFINED;
	}
}

void HttpRequest::parseConnectionType(const string& connection_type) {
	if (connection_type == "close") {
		this->connection_type = ConnectionType::CLOSE;
	}
	else if (connection_type == "keep-alive") {
		this->connection_type = ConnectionType::KEEP_ALIVE;
	}
	else {
		this->connection_type = ConnectionType::UNDEFINED;
	}
}

void HttpRequest::parseLanguage(const string& accept_language) {
	if (accept_language == "en") {
		this->accept_language = AcceptLanguage::EN;
	}
	else if (accept_language == "he") {
		this->accept_language = AcceptLanguage::HE;
	}
	else {
		this->accept_language = AcceptLanguage::UNDEFINED;
	}
}

HttpRequest::RequestType HttpRequest::getRequestType() const {
	return request_type;
}

const string &HttpRequest::getUrl() const {
	return url;
}

HttpRequest::HttpVersion HttpRequest::getHttpVersion() const {
	return http_version;
}

const string &HttpRequest::getHost() const {
	return host;
}

const string &HttpRequest::getUserAgent() const {
	return user_agent;
}

HttpRequest::ConnectionType HttpRequest::getConnectionType() const {
	return connection_type;
}

HttpRequest::AcceptLanguage HttpRequest::getAcceptLanguage() const {
	return accept_language;
}


