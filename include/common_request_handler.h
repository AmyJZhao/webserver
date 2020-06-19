//
// common_request_handler.h
// ~~~~~~~~~~~~~~~~~
//

#ifndef COMMON_REQUEST_HANDLER_H
#define COMMON_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"
#include "request.h"

class common_request_handler : public request_handler
{
  public:
    request_handler* Init(const std::string& location_path, const NginxConfig& config);
    Response handle_request(const Request& request);
  private:
    std::string construct_content(Request request);
};

#endif