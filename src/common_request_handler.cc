//
// common_request_handler.cc
// ~~~~~~~~~~~~~~~~~
//

#include <string>
#include "common_request_handler.h"
#include "response.h"
#include "server_information.h"
#include "response_helper.h"
#include "logger.h"

request_handler* common_request_handler::Init(const std::string& location_path, const NginxConfig& config) 
{
    setLocationPath(location_path);
    return this;
}

Response common_request_handler::handle_request(const Request &request)
{
  response_helper resp_helper;
  Response response = resp_helper.make_response(200, construct_content(request));
  resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));

  ServerInformation* info = ServerInformation::getInstance();
  info->incrementResponseCodeCountByCode(200);
  Logger *logger = Logger::getLogger();
  logger->logDebug("handle_request in EchoHandler", "Response Code: 200");
  return response;
}

std::string common_request_handler::construct_content(Request request)
{
  std::string result = "";
  // First line of request
  result.append(request.getMethod() + " " + request.getUri() + " ");
  result.append(request.getVersion());
  result.append("\r\n");
  // Headers
  result.append(request.getAllHeadersAsString());
  // Trailing CRLF
  result.append("\r\n");

  return result;
}