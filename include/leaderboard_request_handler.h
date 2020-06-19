//
// leaderboard_request_handler.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef LEADERBOARD_REQUEST_HANDLER_H
#define LEADERBOARD_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"
#include "response.h"

class Request;

class LeaderboardRequestHandler : public request_handler {
  public:
    request_handler* Init(const std::string& location_prefix, const NginxConfig& config);
    Response handle_request(const Request& request);
  private:
    bool url_decode(const std::string& in, std::string& out);
};

#endif