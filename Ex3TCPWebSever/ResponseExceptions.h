//
// Created by Nadav Suliman on 31/12/20.
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#define NOT_FOUND_PAGE   ".\\resources\\error-pages\\404-not-found.html"
#define BAD_REQUEST_PAGE ".\\resources\\error-pages\\300-bad-request.html"

using namespace std;

class ResponseException : public std::exception
{
private:
    int status_code;
    string page_dir;
public:
    ResponseException(int statusCode, const string& page_dir) : status_code(statusCode),page_dir(page_dir) {}
    int getStatusCode() { return status_code; }
    const string& getPageDir() { return page_dir; }
    const char *what() const noexcept override = 0;
};

class BadRequestException : public ResponseException {
public:
    BadRequestException() : ResponseException(300, BAD_REQUEST_PAGE) {}
    const char *what() const noexcept override { return "300 BAD REQUEST"; }
};

class NotFoundException : public ResponseException {
public:
    NotFoundException() : ResponseException(404, string(NOT_FOUND_PAGE)) {}
    const char *what() const noexcept override { return "404 NOT FOUND"; }
};
