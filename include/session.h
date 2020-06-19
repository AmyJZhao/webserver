//
// session.h
// ~~~~~~~~~
//

#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include "request_parser.h"
#include <vector>
#include "response.h"
#include "request.h"
#include "request_handler.h"
#include "response_helper.h"
#include <map>
#include <string>

using boost::asio::ip::tcp;

class session
{
public:
  session(boost::asio::io_service& io_service,
    std::map<std::string, request_handler*>);
  tcp::socket& socket();
  void start();

private:
  void do_read();
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);
  bool url_decode(const std::string& in, std::string& out);
  std::string get_request_path(std::string request_url);

  tcp::socket socket_;
  std::array<char, 8192> data_;

  //request_parser::request   request_;
  Request                   request_;
  request_parser            request_parser_;
  Response                  response_;
  response_helper           resp_helper;
  std::string               resp_string;
  std::map<std::string, request_handler*> path_map_;
};

#endif
