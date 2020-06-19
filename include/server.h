//
// server.h
// ~~~~~~~~
//

#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <string>
#include <map>
#include <vector>
#include "session.h"
#include "config_parser.h"
#include "request_handler.h"

using boost::asio::ip::tcp;

class session;
class server
{
public:
  enum accept_status
  {
    READY,
    ACCEPT_SUCCESS,
    ACCEPT_ERROR_ENCOUNTERED,
  };
  
  server(boost::asio::io_service& io_service, unsigned short port,
          std::vector<std::shared_ptr<NginxConfig>> config_blocks);
  ~server();
  boost::asio::io_service& get_io_service() const;
  server::accept_status get_accept_status_response() const;

private:
  void start_accept();
  void create_all_handlers(std::vector<std::shared_ptr<NginxConfig>> config_blocks);
  request_handler* CreateHandlerByHandlerType(NginxConfig config);
  void handle_accept(session* new_session,
      const boost::system::error_code& error);

  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  server::accept_status accept_status_response_;

  std::map<std::string, request_handler*> path_map_;
};

#endif