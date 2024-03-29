//
// Created by Nadav Suliman on 31/12/20.
//
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include "HttpResponse.h"

HttpResponse::HttpResponse(const HttpRequest &request) {
    time(&response_time);

    this->url = request.getUrl();
    this->http_version = request.getHttpVersionAsString();
    this->language = request.getLanguage();

    try {
        switch (request.getRequestType()) {
            case (HttpRequest::RequestType::OPTIONS):
                options();
                break;
            case (HttpRequest::RequestType::GET):
                get();
                break;
            case (HttpRequest::RequestType::POST):
                //request.getMessageBody() goes here.
                post();
                break;
            case (HttpRequest::RequestType::HEAD):
                head();
                break;
            case (HttpRequest::RequestType::DELETE_):
                delete_();
                break;
            case (HttpRequest::RequestType::PUT):
                put();
                break;
            case (HttpRequest::RequestType::TRACE):
                trace(request.toString());
                break;
            default:
                throw BadRequestException();
        }
    }
    catch (ResponseException &e) {
        handleResponseException(e.getPageDir());
    }
}

void HttpResponse::options() throw (ResponseException) {
    ostringstream oss;
    string response_data;

    if (url == "/*") {
        response_data = "Allow: OPTIONS, GET, HEAD, POST, PUT, TRACE, DELETE";
    } else {
        ifstream file(ROOT_DIR + language + "\\" + url);
		cout << ROOT_DIR + url << endl; // DEBUG
        if (file.good()) {
            response_data = "Allowed: OPTIONS, GET, HEAD, DELETE, TRACE";
        } else {
            status_code = NOT_FOUND_404;
            throw NotFoundException();
        }
        file.close();
    }

    oss << http_version << " " << status_code << "\r\n"
        << "Date: " << ctime(&response_time) 
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << "0" << "\r\n"
        << "Content-Type: " << "text/html" << "\r\n"
        << response_data
        << "\r\n"
        << "\r\n";
    content = oss.str();
}

void HttpResponse::get() throw (ResponseException) {
    string response_data;
    ostringstream oss;

    ifstream file(ROOT_DIR + language + "\\" + url);
	cout << ROOT_DIR + language + "\\" + url << endl; // DEBUG
    if (file.good()) {
        status_code = OK_200;
        response_data = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    } else {
        status_code = NOT_FOUND_404;
        throw NotFoundException();
    }
    file.close();

    oss << http_version << " " << status_code << "\r\n"
        << "Date: " << ctime(&response_time) 
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << response_data.length() << "\r\n"
        << "Content-Type: " << "text/html"
        << "\r\n"
        << "\r\n"
        << response_data;

    content = oss.str();
}

void HttpResponse::head() throw (ResponseException) {
    string response_data;
    ostringstream oss;

    ifstream file(ROOT_DIR + language + "\\" + url);
    if (file.good()) {
        status_code = OK_200;
        response_data = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    } else {
        status_code = NOT_FOUND_404;
        throw NotFoundException();
    }
    file.close();

    oss << http_version << " " << status_code << "\r\n"
        << "Date: " << ctime(&response_time) 
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << response_data.length() << "\r\n"
        << "Content-Type: " << "text/html"
        << "\r\n"
        << "\r\n";
    content = oss.str();
}

void HttpResponse::delete_() throw (ResponseException){
    ostringstream oss;
    string remove_dir = ROOT_DIR + language + "\\" + url;
    
    if (remove(remove_dir.c_str()) != 0) {
        status_code = NOT_FOUND_404;
        throw NotFoundException();
    } else {
        status_code = NO_CONTENT_204;
    }

	oss << http_version << " " << status_code << "\r\n"
		<< "Date: " << ctime(&response_time)
		<< "Server: Nadav and Max's server" << "\r\n"
		<< "Content-Length: " << 0 << "\r\n"
		<< "Content-Type: " << "http/message" << "\r\n"
		<< "File-Deleted: " << url 
        << "\r\n"
        << "\r\n";

    content = oss.str();
}

void HttpResponse::trace(const string& originalRequest) throw (ResponseException)
{
	string response_data = originalRequest;
	ostringstream oss;

	ifstream file(ROOT_DIR + language + "\\" + url);
	cout << ROOT_DIR + language + "\\" + url; // DEBUG
	if (file.good()) {
		status_code = OK_200;
	}
	else {
		status_code = NOT_FOUND_404;
		throw NotFoundException();
	}
	
	file.close();
	

	oss << http_version << " " << status_code << "\r\n"
		<< "Date: " << ctime(&response_time)
		<< "Server: Nadav and Max's server" << "\r\n"
		<< "Content-Length: " << response_data.length() << "\r\n"
		<< "Content-Type: " << "http/message" << "\r\n"
		<< "\r\n"
		<< "\r\n"
		<< response_data << "\r\n";

	content = oss.str();
}

void HttpResponse::post() throw (ResponseException) 
{
	
}

void HttpResponse::put() throw (ResponseException)
{

}

void HttpResponse::handleResponseException(const string& page_dir) {
    string response_data;
    ostringstream oss;

    ifstream file(page_dir);
    if (file.good()) {
        response_data = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    }
    file.close();

    oss << http_version << " " << status_code << "\r\n"
        << "Date: " << ctime(&response_time) 
        << "Server: Nadav and Max's server" << "\r\n"
        << "Content-Length: " << response_data.length() << "\r\n"
        << "Content-Type: " << "text/html"
        << "\r\n"
        << "\r\n"
        << response_data;

    content = oss.str();
}

const string &HttpResponse::getContent() {
    return content;
}

