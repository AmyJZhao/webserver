#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <map>
#include <boost/asio.hpp>

enum StatusCode
{
    OK = 200,
    Created = 201,
    Accepted = 202,
    NoContent = 204,
    MultipleChoices = 300,
    MovedPermanently = 301,
    MovedTemporarily = 302,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnavailable = 503
};

enum MimeType
{
    TEXT_PLAIN,
    TEXT_HTML,
    IMAGE_JPEG,
    APPLICATION_ZIP,
    TEXT_CSS,
    TEXT_JAVASCRIPT
};

class Response
{
    public:
        StatusCode code_;
        std::map<std::string, std::string> headers_;
        std::string body_;   
};

#endif