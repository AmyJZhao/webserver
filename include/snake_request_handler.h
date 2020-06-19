//
// snake_request_handler.h
// ~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef SNAKE_REQUEST_HANDLER_H
#define SNAKE_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"
#include "response.h"

class Request;

class SnakeRequestHandler : public request_handler {
  public:
    request_handler* Init(const std::string& location_prefix, const NginxConfig& config);
    Response handle_request(const Request& request);
  private:
    // Directory to serve files from
    std::string directory_;
};

#endif