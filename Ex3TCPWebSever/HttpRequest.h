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

	enum class AcceptLanguage {
	    EN,
	    HE
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

	string          lang;
    string          message_body;
    unordered_map<string, string> header_lines;

public:
	explicit HttpRequest(const char *buffer);

	RequestType getRequestType()    const;
	HttpVersion getHttpVersion()    const;
    const string& getUrl()          const;
    string getRequestTypeAsString() const;
    string getHttpVersionAsString() const;
    string getMessageBody()         const;
    const unordered_map<string, string>& getHeaderLines() const;

    const string &getLanguage() const;

private:
    void parseHeaderLines(std::istringstream& message_stream);
    void parseMandatoryHeaders(std::istringstream& message_stream);
    void parseMessageBody(std::istringstream& message_stream);
    void parseUrl();
    RequestType parseRequestType(const string& request_type);
	HttpVersion parseHttpVersion(const string& http_version);
};
