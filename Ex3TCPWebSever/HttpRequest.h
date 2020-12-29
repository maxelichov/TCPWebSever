#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

class HttpRequest 
{
private:
	enum HeaderIndex { REQUEST_TYPE = 0, URL, HTTP_VERSION, HOST = 4, USER_AGENT = 6, CONNECTION = 8, ACCEPT_LANGUAGE = 9 };

public:
	enum class RequestType { UNDEFINED, OPTIONS, GET, HEAD, POST, DELETE_, PUT, TRACE, CONNECT };
	enum class HttpVersion { UNDEFINED, HTTP_1_0, HTTP_1_1 };
	enum class ConnectionType { UNDEFINED, KEEP_ALIVE, CLOSE };
	enum class AcceptLanguage { UNDEFINED, EN, HE };

private:
	RequestType     request_type;
	string          url;
	HttpVersion     http_version;
	string          host;
	string          user_agent;
	ConnectionType  connection_type;
	AcceptLanguage  accept_language;
	

public:
	HttpRequest(const char *buffer);

	RequestType     getRequestType()    const;
	const string&   getUrl()            const;
	HttpVersion     getHttpVersion()    const;
	const string&   getHost()           const;
	const string&   getUserAgent()      const;
	ConnectionType  getConnectionType() const;
	AcceptLanguage  getAcceptLanguage() const;

private:
	void parseRequestType(const string& request_type);
	void parseHttpVersion(const string& http_version);
	void parseConnectionType(const string& connection_type);
	void parseLanguage(const string& accept_language);
};
