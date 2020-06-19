//
// snake_request_handler.cc
// ~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <fstream>
#include <iostream>
#include "snake_request_handler.h"
#include "not_found_request_handler.h"
#include "server_information.h"
#include "response_helper.h"
#include "logger.h"

class NginxConfig; // implementation not needed

request_handler* SnakeRequestHandler::Init(const std::string& location_prefix, const NginxConfig& config)
{
    setLocationPath(location_prefix);
    directory_ = config.getRoot();
    return this;
}

Response SnakeRequestHandler::handle_request(const Request& request){

    Response response;

    std::ifstream file(".." + directory_ + "/snake.html", std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        std::string data;
        int size = file.tellg();
        data.resize(size);
        file.seekg(0, std::ios::beg);
        file.read(&data[0], data.size());
        file.close();

        response_helper resp_helper;
        Response response = resp_helper.make_response(200, data);

        resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_HTML));
        resp_helper.push_header(response, "Content-Length", std::to_string(response.body_.length()));

        ServerInformation* info = ServerInformation::getInstance();
        info->incrementResponseCodeCountByCode(200);

        Logger *logger = Logger::getLogger();
        logger->logDebug("handle_request in SnakeHandler", "Response Code: 200");
        return response;
    }
    else
    {
        ServerInformation* info = ServerInformation::getInstance();
        info->incrementResponseCodeCountByCode(404);
        NotFoundRequestHandler nfrh;
        return nfrh.handle_request(request);
    }

    return response;

}