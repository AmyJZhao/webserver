//
// status_request_handler.cc
// ~~~~~~~~~~~~~~~~~
//

#include "status_request_handler.h"
#include "server_information.h"
#include "response_helper.h"
#include "logger.h"

request_handler* StatusRequestHandler::Init(const std::string& location_path, const NginxConfig& config) 
{
  return this;
}

Response StatusRequestHandler::handle_request(const Request &request)
{  
  response_helper resp_helper;
  Response response = resp_helper.make_response(200, construct_content());
  resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));

  ServerInformation* info = ServerInformation::getInstance();
  info->incrementResponseCodeCountByCode(200);
  Logger *logger = Logger::getLogger();
  logger->logDebug("handle_request in EchoHandler", "Response Code: 200");
  
  return response;
}

std::string StatusRequestHandler::construct_content()
{
  std::string result = "SERVER STATUS\n=======================\n";
  ServerInformation* info = ServerInformation::getInstance();
  
  for (std::pair<std::string, request_handler*> pair : info->path_map_) {
    result.append(pair.first + ": " + std::to_string(info->getRequestCountForUrl(pair.first)));
    result.append("\n");
	}
  result.append("\n");

  result.append("200: " + std::to_string(info->getResponseCodeCountForCode(200)) + "\n");
  result.append("400: " + std::to_string(info->getResponseCodeCountForCode(400)) + "\n");
  result.append("404: " + std::to_string(info->getResponseCodeCountForCode(404)) + "\n");

  return result;
}