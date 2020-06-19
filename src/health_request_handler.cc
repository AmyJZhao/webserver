//
// health_request_handler.cc
// ~~~~~~~~~~~~~~~~~
//

#include "health_request_handler.h"
#include "server_information.h"
#include "response_helper.h"
#include "logger.h"

request_handler* HealthRequestHandler::Init(const std::string& location_path, const NginxConfig& config  /* unused */) 
{
  setLocationPath(location_path);
  return this;
}

Response HealthRequestHandler::handle_request(const Request &request /* unused */)
{  
  response_helper resp_helper;
  Response response = resp_helper.make_response(200, "OK");
  resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));

  ServerInformation* info = ServerInformation::getInstance();
  info->incrementResponseCodeCountByCode(200);

  Logger *logger = Logger::getLogger();
  logger->logDebug("handle_request in HealthHandler", "Response Code: 200");
  return response;
}
