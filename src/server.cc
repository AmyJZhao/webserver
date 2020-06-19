//
// server.cc
// ~~~~~~~~~
//

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <string>
#include <iostream>
#include <thread>
#include "logger.h"
#include "server_information.h"
#include "leaderboard.h"
#include "server.h"
#include "static_request_handler.h"
#include "common_request_handler.h"
#include "not_found_request_handler.h"
#include "status_request_handler.h"
#include "proxy_request_handler.h"
#include "health_request_handler.h"
#include "leaderboard_request_handler.h"
#include "snake_request_handler.h"

using boost::asio::ip::tcp;

server::server(boost::asio::io_service& io_service, unsigned short port, 
  std::vector<std::shared_ptr<NginxConfig>> config_blocks)
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    accept_status_response_(accept_status::READY)
{
  create_all_handlers(config_blocks);
  start_accept();
}

void server::create_all_handlers(std::vector<std::shared_ptr<NginxConfig>> config_blocks)
{
  for (int i = 0; i < config_blocks.size(); i++) 
  {
    if (config_blocks[i]->getStatementType()) 
    {
      request_handler* handler = CreateHandlerByHandlerType(*config_blocks[i]);
      if (handler != nullptr) {
          path_map_[config_blocks[i]->getUrl()] = handler;
      }
    }
  }
  // Create 404 handler
  NginxConfigStatement s;
  s.setStatementType(NginxConfigStatement::NOT_FOUND_HANDLER);
  NginxConfig config(s);
  request_handler* handler = CreateHandlerByHandlerType(config);
  path_map_["/"] = handler;

  ServerInformation *info = ServerInformation::setInstance(path_map_);

  Leaderboard* lb = Leaderboard::setleaderDB();
}

request_handler* server::CreateHandlerByHandlerType(NginxConfig config) 
{
  if (config.getStatementType() == NginxConfigStatement::STATIC_HANDLER) 
  {
    static_request_handler* srh = new static_request_handler;
    return (srh->Init(config.getUrl(), config));
  }
  if (config.getStatementType() == NginxConfigStatement::ECHO_HANDLER) 
  {
    common_request_handler* crh = new common_request_handler;
    return (crh->Init(config.getUrl(), config));
  }
  if (config.getStatementType() == NginxConfigStatement::NOT_FOUND_HANDLER)
  {
    NotFoundRequestHandler* nfrh = new NotFoundRequestHandler;
    return (nfrh->Init(config.getUrl(), config));
  }
  if (config.getStatementType() == NginxConfigStatement::STATUS_HANDLER)
  {
    StatusRequestHandler* srh = new StatusRequestHandler;
    return (srh->Init(config.getUrl(), config));
  }
  if (config.getStatementType() == NginxConfigStatement::PROXY_HANDLER)
  {
    ProxyRequestHandler* prh = new ProxyRequestHandler;
    return (prh->Init(config.getUrl(), config));
  }
  if (config.getStatementType() == NginxConfigStatement::HEALTH_HANDLER)
  {
    HealthRequestHandler* hrh = new HealthRequestHandler;
    return (hrh->Init(config.getUrl(), config));
  }
  if (config.getStatementType() == NginxConfigStatement::LEADERBOARD_HANDLER)
  {
    LeaderboardRequestHandler* lrh = new LeaderboardRequestHandler;
    return (lrh->Init(config.getUrl(), config));
  }
  if (config.getStatementType() == NginxConfigStatement::SNAKE_HANDLER)
  {
    SnakeRequestHandler* srh = new SnakeRequestHandler;
    return (srh->Init(config.getUrl(), config));
  }
  return nullptr;
}

server::~server()
{
  for (std::pair<std::string, request_handler*> e : path_map_) 
  {
		delete e.second;
	}
}

boost::asio::io_service& server::get_io_service() const
{
  return io_service_;
}

server::accept_status server::get_accept_status_response() const
{
  return accept_status_response_;
}

void server::start_accept()
{
  session* new_session = new session(io_service_, path_map_);
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
}

void server::handle_accept(session* new_session,
    const boost::system::error_code& error)
{
  Logger * logger = Logger::getLogger();
  if (!error)
  {
    logger->logDebug("handle_accept in Server", "Incoming request from IP: " + new_session->socket().remote_endpoint().address().to_string());
    boost::thread t(boost::bind(&session::start, new_session));
    accept_status_response_ = accept_status::ACCEPT_SUCCESS;
  }
  else
  {
    logger->logError("handle_accept in Server", "Unable to make a connection.");
    delete new_session;
    accept_status_response_ = accept_status::ACCEPT_ERROR_ENCOUNTERED;
  }

  start_accept();
}