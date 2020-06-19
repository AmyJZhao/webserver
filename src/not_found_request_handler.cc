//
// not_found_request_handler.cc
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include "not_found_request_handler.h"
#include "response_helper.h"
#include "logger.h"

class NginxConfig; // implementation not needed

request_handler* NotFoundRequestHandler::Init(const std::string& location_prefix, const NginxConfig& config){
    return this;
}

Response NotFoundRequestHandler::handle_request(const Request& request){
    response_helper resp_helper;
    Response response = resp_helper.make_response(404, "The requested URL was not found on this server.");
    resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));
    resp_helper.push_header(response, "Content-Length", std::to_string(response.body_.length()));
    Logger *logger = Logger::getLogger();
    logger->logDebug("handle_request in 404Handler", "Response Code: 404");
    return response;
}