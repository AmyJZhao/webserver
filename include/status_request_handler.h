//
// status_request_handler.h
// ~~~~~~~~~~~~~~~~~
//

#ifndef STATUS_REQUEST_HANDLER_H
#define STATUS_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"
#include <map>
#include "request.h"
#include "response.h"

class StatusRequestHandler : public request_handler
{
  public:
    request_handler* Init(const std::string& location_path, const NginxConfig& config);
    Response handle_request(const Request& request);
  private:
    std::string construct_content();
};

#endif