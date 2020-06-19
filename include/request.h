#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <map>

enum MethodEnum {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    NOTSET
};

static std::map<std::string,MethodEnum> method_map {
    { "GET", GET }, 
    { "HEAD", HEAD },
    { "POST", POST }, 
    { "PUT", PUT },
    { "DELETE", DELETE },
    { "CONNECT", CONNECT }, 
    { "OPTIONS", OPTIONS },
    { "TRACE", TRACE } 
};

class Request {
    public:
        Request();
        void setMethod(std::string method);
        void setUri(std::string uri);
        void setVersion(std::string version);
        void pushHeader(std::string name, std::string value);
        void setBody(std::string body);
        void setProper(bool proper);
        void setIndeterminate(bool ind);

        std::string getMethod() const;
        std::string getUri() const;
        std::string getVersion() const;
        std::string getHeader(std::string name);
        std::string getAllHeadersAsString() const;
        std::string getBody() const;
        bool getProper() const;
        bool getIndeterminate() const;


        // A map of header, for convenient lookup ("Content-Type", "Cookie", etc)
        std::map<std::string, std::string> headers_;

    private:
        // The HTML method (GET, PUT, POST, etc)
        MethodEnum method_;
        std::string method_name_;

        // The path of the request
        std::string uri_;

        // The HTTP version string as given in the request line, e.g. "HTTP/1.1"
        std::string version_;

        // The content of the request
        std::string body_;

        //if proper_ is true, request is properly formatted
        //if proper_ is false, bad request
        bool proper_;

        //incomplete, needs more parsing
        bool indeterminate_;
};

#endif
