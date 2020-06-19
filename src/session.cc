//
// session.cc
// ~~~~~~~~~~
//

#include <boost/bind.hpp>
#include "session.h"

#include "logger.h"
#include <iterator>
#include <iostream>
#include "request_handler.h"
#include "common_request_handler.h"
#include "static_request_handler.h"
#include "server_information.h"
#include "logger.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service &io_service,
                 std::map<std::string, request_handler *> path_map)
    : socket_(io_service), path_map_(path_map)
{
}

tcp::socket &session::socket()
{
  return socket_;
}

void session::start()
{
  do_read();
}

void session::do_read()
{
  socket_.async_read_some(boost::asio::buffer(data_),
                          boost::bind(&session::handle_read, this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code &error,
                          size_t bytes_transferred)
{
  Logger *logger = Logger::getLogger();
  if (!error)
  {
    // Feed request into parser and store the parsed
    // request in request_
    request_parser::result_type result;
    request_parser_.parse(request_, data_.data(), data_.data() + bytes_transferred);
    if (request_.getIndeterminate())
    {
      do_read();
    }
    else
    {
      std::string request_url;
      if (url_decode(request_.getUri(), request_url) && request_.getProper())
      {
        std::string request_path = get_request_path(request_url);
        logger->logDebug("handle_read in Session", "Request Path: " + request_path);
        if (path_map_.count(request_path))
        {
          ServerInformation *info = ServerInformation::getInstance();
          info->incrementRequestCountByUrl(request_path);
          logger->logDebug("handle_read in Session", "Getting response for request to " + request_path);
          response_ = path_map_[request_path]->handle_request(request_);
        }
        else
        {
          //404 handler
          logger->logDebug("handle_read in Session", "Request path not found in config file.");
          logger->logDebug("handle_read in Session", "Response code: 404");
          ServerInformation *info = ServerInformation::getInstance();
          info->incrementResponseCodeCountByCode(404);
          response_ = path_map_["/"]->handle_request(request_);
        }
      }
      else
      {

        logger->logError("handle_read in Session", "Request not properly formatted.");
        logger->logDebug("handle_read in Session", "Response code: 400");
        response_ = resp_helper.make_response(400, "");
        resp_helper.push_header(response_, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));

        ServerInformation *info = ServerInformation::getInstance();
        info->incrementResponseCodeCountByCode(400);
      }

      resp_string = resp_helper.to_string(response_);
      boost::asio::async_write(socket_,
                               boost::asio::buffer(resp_string, resp_string.size() + 1),
                               boost::bind(&session::handle_write, this,
                                           boost::asio::placeholders::error));
    }
  }
  else
  {
    logger->logError("handle_read in Session", "handle_read error: " + error.message());
    delete this;
  }
}

void session::handle_write(const boost::system::error_code &error)
{
  Logger *logger = Logger::getLogger();
  if (!error)
  {
    logger->logDebug("handle_write in Session", "Disconnected from IP: " + socket_.remote_endpoint().address().to_string());
    socket_.close();
    delete this;
  }
  else
  {
    logger->logError("handle_write in Session", "Unable to disconnect. handle_write error: " + error.message());
    delete this;
  }
}

bool session::url_decode(const std::string &in, std::string &out)
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

std::string session::get_request_path(std::string request_url)
{
  char cur;
  std::string req_path = "";
  for (int i = 0; i < request_url.length(); i++)
  {
    cur = request_url[i];
    // If first character not a backslash, invalid
    if (i == 0 && cur != '/')
      break;
    // Build up request path
    else if (isalnum(cur) || (i == 0 && cur == '/'))
    {
      req_path += cur;
    }

    else // If we find another slash
    {
      break;
    }
  }
  return req_path;
}
