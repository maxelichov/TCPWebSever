#include "HttpRequest.h"

HttpRequest::HttpRequest(const char *buffer) {
    istringstream message_stream(buffer);

    this->request_status = StatusCode::OK;         //default status is 200 OK.
    parseMandatoryHeaders(message_stream);
    parseHeaderLines(message_stream);
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

    while (std::getline(message_stream, header_line) && header_line != "\r") {
        index = header_line.find_first_of(':');
        //TODO: Need to trim header_line.substr somehow.
        if(index != std::string::npos) {
            this->header_lines.insert(std::make_pair(
                    string(header_line.substr(0, index)),
                    string(header_line.substr(index + 2, header_line.length()))
            ));
        } else {
            this->request_status = StatusCode::BAD_REQUEST;
            return;
        }
    }
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
        this->request_status = StatusCode::BAD_REQUEST;
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
        this->request_status = StatusCode::BAD_REQUEST;
        return HttpVersion::UNDEFINED;
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

