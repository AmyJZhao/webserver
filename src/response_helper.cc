#include "response_helper.h"

void response_helper::push_header(Response& res, std::string key, std::string value)
{
    res.headers_[key] = value;
}

std::string response_helper::get_header(Response& res, std::string header)
{
    std::map<std::string, std::string>::iterator it;
    it = res.headers_.find(header);

    if (it != res.headers_.end())
    {
        return it->second;
    }
    return "";
}

Response response_helper::make_response(int status_code, std::string body)
{
    Response r;
    r.code_ = static_cast<StatusCode>(status_code);
    r.body_ = body;
    return r;
}

std::unordered_map<StatusCode, std::string> response_helper::status_phrase = {
    {StatusCode::OK, "OK"},
    {StatusCode::Created, "Created"},
    {StatusCode::Accepted, "Accepted"},
    {StatusCode::NoContent, "No Content"},
    {StatusCode::MultipleChoices, "Multiple Choices"},
    {StatusCode::MovedPermanently, "Moved Permanently"},
    {StatusCode::MovedTemporarily, "Moved Temporarily"},
    {StatusCode::NotModified, "Not Modified"},
    {StatusCode::BadRequest, "Bad Request"},
    {StatusCode::Unauthorized, "Unauthorized"},
    {StatusCode::Forbidden, "Forbidden"},
    {StatusCode::NotFound, "Not Found"},
    {StatusCode::InternalServerError, "Internal Server Error"},
    {StatusCode::NotImplemented, "Not Implemented"},
    {StatusCode::BadGateway, "Bad Gateway"},
    {StatusCode::ServiceUnavailable, "Service Unavailable"}
};

std::string response_helper::to_string(const Response& res)
{
    return "HTTP/1.1 " + std::to_string(static_cast<int>(res.code_))  + " "
         + response_helper::status_phrase[res.code_] + "\r\n" + headers_to_string(res)
         + "\r\n" + res.body_;
}

std::string response_helper::headers_to_string(const Response& res)
{
    std::string str;
    for (auto header : res.headers_)
    {
        str = str + header.first + ": " + header.second +  "\r\n";
    }

    return str;
}

std::string response_helper::get_mime_type(MimeType mt) const
{
    switch (mt)
    {
        case MimeType::TEXT_PLAIN:
            return "text/plain";
        case MimeType::TEXT_HTML:
            return "text/html";
        case MimeType::IMAGE_JPEG:
            return "image/jpeg";
        case MimeType::APPLICATION_ZIP:
            return "application/zip";
        case MimeType::TEXT_CSS:
            return "text/css";
        case MimeType::TEXT_JAVASCRIPT:
            return "text/javascript";
    }
}