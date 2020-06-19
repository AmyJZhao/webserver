//
// request_handler.h
// ~~~~~~~~~~~~~~~~~
//

#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include "request_parser.h"
#include "config_parser.h"
#include "request.h"
#include "response.h"

class Response;

class request_handler 
{
  public:
    virtual request_handler* Init(const std::string& location_path, const NginxConfig& config) = 0;
    virtual Response handle_request(const Request& request) = 0;
    void setLocationPath(std::string location_path);
    std::string getLocationPath();
  protected:
    // The path that the request handler responds to 
    std::string path_;
};

#endif