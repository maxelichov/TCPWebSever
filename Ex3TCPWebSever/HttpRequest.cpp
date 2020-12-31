#include "HttpRequest.h"

HttpRequest::HttpRequest(const char *buffer) {
    this->lang = "en";
    istringstream message_stream(buffer);
    parseMandatoryHeaders(message_stream);
    parseHeaderLines(message_stream);
    parseMessageBody(message_stream);
    parseUrl();
}

/*!
 * Initialises url, request_type and http_version based on the first line of the request message.
 * @param message_stream - The request message as a stream
 */
void HttpRequest::parseMandatoryHeaders(std::istringstream& message_stream) {
    string mandatory_header;
    getline(message_stream, mandatory_header);

    istringstream iss(mandatory_header);
    vector<string> parsed_message((istream_iterator<string>(iss)), istream_iterator<string>());

    this->url = parsed_message[HeaderIndex::URL];
    this->request_type = parseRequestType(parsed_message[HeaderIndex::REQUEST_TYPE]);
    this->http_version = parseHttpVersion(parsed_message[HeaderIndex::HTTP_VERSION]);

}

/*!
 * Initialises all additional header lines provided in the request. Puts all header line into the unordered map
 * header_lines.
 * @param message_stream - The request message as a string stream, after the first line has been read.
 */
void HttpRequest::parseHeaderLines(std::istringstream& message_stream) {
    string header_line;
    int index;

    while (std::getline(message_stream, header_line) && header_line != "\r\n") {
        index = header_line.find_first_of(':');
        //TODO: Need to trim header_line.substr somehow.
        if (index != std::string::npos) {
            this->header_lines.insert(std::make_pair(
                    string(header_line.substr(0, index)),
                    string(header_line.substr(index + 2, header_line.length()))
            ));
        }
    }
}

void HttpRequest::parseMessageBody(std::istringstream& message_stream) {
    this->message_body = message_stream.str();
}

HttpRequest::RequestType HttpRequest::parseRequestType(const string& request_type) {
	if (request_type == "OPTIONS") {
		return RequestType::OPTIONS;
	}
	else if (request_type == "GET") {
		return RequestType::GET;
	}
	else if (request_type == "HEAD") {
		return RequestType::HEAD;
	}
	else if (request_type == "POST") {
		return RequestType::POST;
	}
	else if (request_type == "DELETE") {
		return RequestType::DELETE_;
	}
	else if (request_type == "PUT") {
		return RequestType::PUT;
	}
	else if (request_type == "TRACE") {
		return RequestType::TRACE;
	}
	else if (request_type == "CONNECT") {
		return RequestType::CONNECT;
	}
	else {
        return RequestType::UNDEFINED;
	}
}

/*!
 * Parses the HTTP version of the request based on the string retrieved from the message.
 * @param http_version - The HTTP version retreived from the mesage
 * @return the HTTP version of the request as HttpRequest::HttpVersion enum.
 */
HttpRequest::HttpVersion HttpRequest::parseHttpVersion(const string& http_version) {
	if (http_version == "HTTP/1.1") {
		return HttpVersion::HTTP_1_1;
	}
	else if (http_version == "HTTP/1.0") {
		return HttpVersion::HTTP_1_0;
	}
	else {
        return HttpVersion::UNDEFINED;
	}
}

void HttpRequest::parseUrl() {
    string url_delimiter = "?";
    string query_delimiter = "=";

    //If there are query parameters
    if (url.find(url_delimiter) != string::npos && url.find(query_delimiter) != string::npos) {
        string temp_url = url.substr(0, url.find(url_delimiter));
        url.erase(0, url.find(url_delimiter) + url_delimiter.length());
        url.erase(0, url.find(query_delimiter) + query_delimiter.length());

        lang = url;
        url = temp_url;
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

const string &HttpRequest::getLanguage() const {
    return lang;
}

string HttpRequest::getRequestTypeAsString() const {
    switch (request_type) {
        case RequestType::TRACE:
            return "TRACE";
        case RequestType::OPTIONS:
            return "OPTIONS";
        case RequestType::GET:
            return "GET";
        case RequestType::HEAD:
            return "HEAD";
        case RequestType::POST:
            return "POST";
        case RequestType::DELETE_:
            return "DELETE";
        case RequestType::PUT:
            return "PUT";
        case RequestType::CONNECT:
            return "CONNECT";
        case RequestType::UNDEFINED:
            return "UNDEFINED";
    }
}

string HttpRequest::getHttpVersionAsString() const {
    switch (http_version) {
        case HttpVersion::HTTP_1_0:
            return "HTTP/1.0";
        case HttpVersion::HTTP_1_1:
            return "HTTP/1.1";
        case HttpVersion::UNDEFINED:
            return "UNDEFINED";
    }
}

const unordered_map<string, string> &HttpRequest::getHeaderLines() const {
    return header_lines;
}

string HttpRequest::getMessageBody() const {
    return message_body;
}

