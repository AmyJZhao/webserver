//
// request_parser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_PARSER_H
#define HTTP_REQUEST_PARSER_H

#include <tuple>
#include <string>
#include <vector>

#include "request.h"

/// Parser for incoming requests.
class request_parser
{
    public:
    /// Construct ready to parse the request method.
    request_parser();

    /// Reset to initial parser state.
    void reset();

    /// Result of parse.
    enum result_type { good, bad, indeterminate };

    template <typename InputIterator>
    void parse(Request& request_, InputIterator begin, InputIterator end) {
        request req_struct;
        result_type result;
        InputIterator bodyStart;
        std::tie(result, bodyStart) = parse_request(req_struct, begin, end);
        //if result type is good then set request_ fields 
        if(result == good) {
            request_.setProper(true);
            request_.setIndeterminate(false);
            request_.setMethod(req_struct.method);
            request_.setUri(req_struct.uri);
            request_.setVersion("HTTP/" + std::to_string(req_struct.http_version_major) + "." + std::to_string(req_struct.http_version_minor));
            for(const auto header_ : req_struct.headers) {
                request_.pushHeader(header_.name, header_.value);
            }
            parse_body(req_struct, bodyStart, end);
            request_.setBody(req_struct.body);
        }
        else if(result == bad) {
            request_.setIndeterminate(false);
            request_.setProper(false);
        } else {
            request_.setIndeterminate(true);
        }
    }



    private:
    // header structure
    struct header
    {
        std::string name;
        std::string value;
    };

    // request structure
    struct request
    {
        std::string method;
        std::string uri;
        int http_version_major;
        int http_version_minor;
        std::vector<header> headers;
        std::string body;
    };
    /// Parse some data. The enum return value is good when a complete request has
    /// been parsed, bad if the data is invalid, indeterminate when more data is
    /// required. The InputIterator return value indicates how much of the input
    /// has been consumed.
    template <typename InputIterator>
    std::tuple<result_type, InputIterator> parse_request(request& req,
        InputIterator begin, InputIterator end)
    {
        
        while (begin != end)
        {
        result_type result = consume(req, *begin++);
        if (result == good || result == bad)
            return std::make_tuple(result, begin);
        }
        return std::make_tuple(indeterminate, begin);
    }

    template <typename InputIterator>
    void parse_body(request& req,
        InputIterator begin, InputIterator end)
    {
        while (begin != end)
        {
            consumeBody(req, *begin++);
        }
    }

    void consumeBody(request& req, char input);

    /// Handle the next character of input.
    result_type consume(request& req, char input);

    /// Check if a byte is an HTTP character.
    static bool is_char(int c);

    /// Check if a byte is an HTTP control character.
    static bool is_ctl(int c);

    /// Check if a byte is defined as an HTTP tspecial character.
    static bool is_tspecial(int c);

    /// Check if a byte is a digit.
    static bool is_digit(int c);

    /// The current state of the parser.
    enum state
    {
        method_start,
        method,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3
    } state_;
};

#endif // HTTP_REQUEST_PARSER_HPP
