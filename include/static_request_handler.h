//
// static_request_handler.h
// ~~~~~~~~~~~~~~~~~
//

#ifndef STATIC_REQUEST_HANDLER_H
#define STATIC_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"
#include "response.h"
#include <map>
#include "request.h"

class static_request_handler : public request_handler
{
  public:
    request_handler* Init(const std::string& location_path, const NginxConfig& config);
    Response handle_request(const Request& request);
    std::string getDirectory();
  private:
    // Directory to serve files from
    std::string directory_;
    bool url_decode(const std::string& in, std::string& out);
};

#endif