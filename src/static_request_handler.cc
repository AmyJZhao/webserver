// static_request_handler.cc
// ~~~~~~~~~~~~~~~~~

#include "static_request_handler.h"
#include <fstream>
#include <iostream>
#include "server_information.h"
#include "response_helper.h"
#include "not_found_request_handler.h"
#include "logger.h"

request_handler* static_request_handler::Init(const std::string& location_path, const NginxConfig& config) 
{
    setLocationPath(location_path);
    directory_ = config.getRoot();
    return this;
}

Response static_request_handler::handle_request(const Request &request)
{

  Response response;

  int size;
  std::string request_url;
  std::string raw_uri = request.getUri();
  url_decode(raw_uri, request_url);
  std::string req_path_ = request_url.substr(request_url.find_last_of("/\\") + 1);

  std::ifstream file(".." + directory_ + "/" + req_path_, std::ios::in | std::ios::binary);
  if (file.is_open())
  {
    file.seekg(0, std::ios::end);
    std::string data;
    size = file.tellg();
    data.resize(size);
    file.seekg(0, std::ios::beg);
    file.read(&data[0], data.size());
    file.close();

    response_helper resp_helper;
    Response response = resp_helper.make_response(200, data);

    std::size_t last_dot_pos = req_path_.find_last_of(".");
    std::string extension;
    extension = req_path_.substr(last_dot_pos + 1);

    if (extension == "html")
    {
      resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_HTML));
    }
    else if (extension == "jpg" || extension == "jpeg")
    {
      resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::IMAGE_JPEG));
    }
    else if (extension == "zip")
    {
      resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::APPLICATION_ZIP));
    }
    else if (extension == "txt")
    {
      resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));
    }
    else if (extension == "css")
    {
      resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_CSS));
    }
    else if (extension == "js")
    {
      resp_helper.push_header(response, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_JAVASCRIPT));
    }

    resp_helper.push_header(response, "Content-Length", std::to_string(response.body_.length()));

    ServerInformation* info = ServerInformation::getInstance();
    info->incrementResponseCodeCountByCode(200);

    Logger *logger = Logger::getLogger();
    logger->logDebug("handle_request in StaticHandler", "Response Code: 200");
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

std::string static_request_handler::getDirectory() {
    return directory_;
}

bool static_request_handler::url_decode(const std::string& in, std::string& out)
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