//
// proxy_request_handler.h
// ~~~~~~~~~~~~~~~~~
//

#ifndef PROXY_REQUEST_HANDLER_H
#define PROXY_REQUEST_HANDLER_H

#include "request_handler.h"
#include "response.h"
#include "request.h"
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <mutex>
#include <sstream>
#include <zlib.h>

#include <string>

using boost::asio::ip::tcp;

class ProxyRequestHandler : public request_handler
{
public:
    request_handler *Init(const std::string &location_path, const NginxConfig &config);
    Response handle_request(const Request &request);

private:
    // Host and port of the proxy location
    std::string host_;
    unsigned short port_;
    // Default response for errors
    Response default_response_;
    const int MAX_LENGTH = 1024;
    // Maximum number of redirect links to follow
    const int MAX_REDIRECT = 10;

    // Functions to do gzip compress/decompress (from https://panthema.net/2007/0328-ZLibString.html)
    static std::string gzip_decompress(const std::string &data);
    static std::string gzip_compress(const std::string &data);
    
    // Convert the original request into a proxy request string to send
    std::string construct_request(const Request &request, std::string new_url);

    // Sends a proxy request
    void send_proxy_request(std::string &new_request, tcp::resolver& resolver_, tcp::socket& socket_);

    // Recevies the proxy response and parses it into a response object
    Response parse_response(tcp::acceptor& acceptor, tcp::socket& socket);

    // Handles 302 responses, return true if the redirect is handled and false otherwise
    bool handle_redirect(const Request& request, Response& response, tcp::resolver& resolver_, tcp::acceptor& acceptor_, tcp::socket& socket_);

    // Parses HTTP header data into a vector, extracting the status code in the process
    std::map<std::string, std::string> parse_headers(std::string header_data, int& status_code);

    // Split a string into a vector of strings based on the delimiter string    
    std::vector<std::string> split_string(std::string str, std::string delimiter);
};

#endif