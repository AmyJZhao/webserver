//
// leaderboard_request_handler.cc
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "leaderboard_request_handler.h"
#include "response_helper.h"
#include "logger.h"
#include "leaderboard.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
class NginxConfig; // implementation not needed

request_handler* LeaderboardRequestHandler::Init(const std::string& location_prefix, const NginxConfig& config)
{
    setLocationPath(location_prefix);
    return this;
}

Response LeaderboardRequestHandler::handle_request(const Request& request){
    response_helper resp_helper;
    Leaderboard* leaderDB = Leaderboard::getleaderDB();
    if(method_map[request.getMethod()] == MethodEnum::POST) {
        std::string body = request.getBody();
        std::string name;
        std::string scoreS;
        std::istringstream origStream(body);
        std::getline(origStream, name);
        std::getline(origStream, scoreS);
        int score = std::stoi(scoreS);
        leaderDB->insertNewScore(name, score);
        Response response = resp_helper.make_response(200, "");
        Logger *logger = Logger::getLogger();
        logger->logDebug("handle_request in LeaderboardHandler for /best", "Response Code: 200, inserted new entry into leaderboard");
        return response;
    } else {
        std::string raw_uri = request.getUri();
        std::string request_url = "";
        std::string req_path_ = "";
        url_decode(raw_uri, request_url);
        std::cout << request_url + " request url\n";
        int index = request_url.find("best");
        if (index != std::string::npos) {
          req_path_ = request_url.substr(index);
        }

        // if they are requesting /best, 
        if (req_path_.substr(0, 4) == "best")
        {
            // check if the user's score is higher than one of the scores on leaderboard
            std::string user_score = req_path_.substr(req_path_.find_last_of("/\\") + 1);
            Response response;
            Logger *logger = Logger::getLogger();
            if (leaderDB->GEQLowestScore(std::stoi(user_score))) { 
                response = resp_helper.make_response(200, "");
                logger->logDebug("handle_request in LeaderboardHandler for /best", "Response Code: 200, False");
            }
            else { 
                response = resp_helper.make_response(204, "");
                logger->logDebug("handle_request in LeaderboardHandler for /best", "Response Code: 204, False");
            }
            return response;
        }
        else
        {
            Response response = resp_helper.make_response(200, leaderDB->leaderboardHTML());
            resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_HTML));
            resp_helper.push_header(response, "Content-Length", std::to_string(response.body_.length()));
            Logger *logger = Logger::getLogger();
            logger->logDebug("handle_request in LeaderboardHandler for /dashboard", "Response Code: 200");
            return response;
        }
    }
}

bool LeaderboardRequestHandler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}