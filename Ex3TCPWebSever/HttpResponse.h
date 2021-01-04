#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>

#include "HttpRequest.h"
#include "ResponseExceptions.h"

#define ROOT_DIR ".\\resources\\"
#define PAGE_404 strcat(ROOT_DIR, "404-not-found.html")
#define PAGE_300 strcat(ROOT_DIR, "300-bad-request.html")

#define OK_200          "200 OK";
#define NO_CONTENT_204  "204 NO CONTENT"
#define BAD_REQUEST_300 "300 BAD REQUEST";
#define NOT_FOUND_404   "404 NOT FOUND";

using namespace std;

class HttpResponse {
private:

    time_t response_time;
    string status_code;
    string url;
    string http_version;
    string language;
    string content;

public:
    HttpResponse(const HttpRequest& request);
    const string& getContent();

private:

    void options()         throw (ResponseException);
    void get()             throw (ResponseException);
    void head()            throw (ResponseException);
    void delete_()         throw (ResponseException);
    void trace(const string& originalRequest)           throw (ResponseException);
    void post()            throw (ResponseException);
    void put()             throw (ResponseException);
    void handleResponseException(const string& page_dir);
};
