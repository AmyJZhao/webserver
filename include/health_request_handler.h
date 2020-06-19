//
// health_request_handler.h
// ~~~~~~~~~~~~~~~~~
//

#ifndef HEALTH_REQUEST_HANDLER_H
#define HEALTH_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"
#include "response.h"
#include "request.h"

class HealthRequestHandler : public request_handler
{
  public:
    request_handler* Init(const std::string& location_path /* unused */, const NginxConfig& config /* unused */);
    Response handle_request(const Request& request /* unused */);
};

#endif
