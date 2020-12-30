#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <sstream>
#include <iterator>
#include <vector>
#include <unordered_map>

using namespace std;

class HttpRequest 
{
private:
	enum HeaderIndex {
	    REQUEST_TYPE,
	    URL,
	    HTTP_VERSION,
	};

	enum StatusCode {
	    OK                      = 200,
	    BAD_REQUEST             = 400,
	    NOT_FOUND               = 404,
	    INTERNAL_SERVER_ERROR   = 500
	};

public:
	enum class RequestType {
	    UNDEFINED,
	    OPTIONS,
	    GET,
	    HEAD,
	    POST,
	    DELETE_,
	    PUT,
	    TRACE,
	    CONNECT
	};
	enum class HttpVersion {
	    UNDEFINED,
	    HTTP_1_0,
	    HTTP_1_1
	};

private:
    RequestType     request_type;
	string          url;
	HttpVersion     http_version;
	StatusCode      request_status;

	unordered_map<string, string> header_lines;

public:
	explicit HttpRequest(const char *buffer);

	RequestType     getRequestType()    const;
	const string&   getUrl()            const;
	HttpVersion     getHttpVersion()    const;

private:
    void parseHeaderLines(std::istringstream& message_stream);
    void parseMandatoryHeaders(std::istringstream& message_stream);
    RequestType parseRequestType(const string& request_type);
	HttpVersion parseHttpVersion(const string& http_version);
};
