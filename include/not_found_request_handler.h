//
// not_found_request_handler.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef NOT_FOUND_REQUEST_HANDLER_H
#define NOT_FOUND_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"
#include "response.h"

class Request;

class NotFoundRequestHandler : public request_handler {
  public:
    request_handler* Init(const std::string& location_prefix, const NginxConfig& config);
    Response handle_request(const Request& request);
};

#endif