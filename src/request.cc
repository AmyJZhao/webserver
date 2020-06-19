#include "request.h"

Request::Request() {
    method_ = NOTSET;
    uri_ = "";
    version_ = "";
    body_ = "";
    proper_ = false;
    indeterminate_ = true;
}

void Request::setMethod(std::string method) {
    auto x = method_map.find(method);
    if(x != std::end(method_map)) {
        method_ = x->second;
        method_name_ = method;
    } else {
        setProper(false);
    }
}

void Request::setUri(std::string uri) {
    uri_ = uri;
}

void Request::setVersion(std::string version) {
    version_ = version;
}

void Request::pushHeader(std::string name, std::string value) {
    headers_.insert({name, value});
}

void Request::setBody(std::string body) {
    body_ = body;
}

void Request::setProper(bool proper) {
    proper_ = proper;
}

void Request::setIndeterminate(bool ind) {
    indeterminate_ = ind;
}

std::string Request::getMethod() const {
    return method_name_;
}

std::string Request::getUri() const {
    return uri_;
}

std::string Request::getVersion() const {
    return version_;
}

std::string Request::getHeader(std::string name) {
    return headers_[name];
}

std::string Request::getAllHeadersAsString() const {
    std::string headers = "";
    for(auto const& pair : headers_) {
        std::string header = pair.first + ": " + pair.second + "\r\n";
        headers += header;
    }
    return headers;
}

std::string Request::getBody() const {
    return body_;
}

bool Request::getProper() const {
    return proper_;
}

bool Request::getIndeterminate() const {
    return indeterminate_;
}
