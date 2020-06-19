#include "gtest/gtest.h"
#include "server.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include "logger.h"
#include <vector>

using boost::asio::ip::tcp;

class NginxConfig;
/**
 * Used to test server state without dependency on
 * Boost's async_accept
 **/
class MockServer : public server {
 public:
  // Constructor
  MockServer(boost::asio::io_service& io_service, unsigned short port,
            std::vector<std::shared_ptr<NginxConfig>> config_blocks)
    : server(io_service, port, config_blocks)
  { 
    accept_status_response_ = accept_status::READY;
    is_boost_connection_error_present_ = false;
  }
  // Mock methods
  server::accept_status get_accept_status_response() const
  {
    return accept_status_response_;
  }
  void connectWithSuccess() 
  { 
    is_boost_connection_error_present_ = false;
    handleAccept();
  }
  void connectWithError() 
  {
    is_boost_connection_error_present_ = true;
    handleAccept();
  }
  
private:
  void handleAccept()
  {
    if (!is_boost_connection_error_present_)
    {
      accept_status_response_ = accept_status::ACCEPT_SUCCESS;
    }
    else
    {
      accept_status_response_ = accept_status::ACCEPT_ERROR_ENCOUNTERED;
    }
  }

  server::accept_status accept_status_response_;
  bool is_boost_connection_error_present_;
};

class ServerTest : public ::testing::Test {
  protected:
    boost::asio::io_service         io_service_;
    unsigned short                  port_;
    std::vector<std::shared_ptr<NginxConfig>> config_blocks_;
};

TEST_F(ServerTest, ConstructorTest) {
  port_ = 8080;
  MockServer s(io_service_, port_, config_blocks_);
  
  EXPECT_EQ(&io_service_, &s.get_io_service());
  EXPECT_EQ(s.get_accept_status_response(), server::accept_status::READY);
}

TEST_F(ServerTest, HandleAccept) {
  port_ = 8080;
  MockServer s(io_service_, port_, config_blocks_);
  s.connectWithSuccess();
  EXPECT_EQ(s.get_accept_status_response(), server::accept_status::ACCEPT_SUCCESS);
  s.connectWithError();
  EXPECT_EQ(s.get_accept_status_response(), server::accept_status::ACCEPT_ERROR_ENCOUNTERED);
}