#ifndef RESPONSE_HELPER_H
#define RESPONSE_HELPER_H

#include "response.h"
#include <unordered_map>
#include <string>

class response_helper {
    public:
        void push_header(Response& res, std::string key, std::string value);
        std::string get_header(Response& res, std::string header);
        Response make_response(int status_code, std::string body);
        //std::string get_status_phrase(StatusCode code);
        static std::unordered_map<StatusCode, std::string> status_phrase;
        static std::unordered_map<int, StatusCode> status_code_enum; 
        std::string to_string(const Response& res);
        std::string headers_to_string(const Response& res);
        std::string get_mime_type(MimeType mt) const;
        
};

#endif