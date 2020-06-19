#include "gtest/gtest.h"
#include "request_handler.h"
#include <string>
#include <iostream>
#include <map>
#include "request.h"

using namespace std;

class RequestHandlerTest : public ::testing::Test {
  protected:
    request_handler test_request_handler;
    string request;
    size_t bytes_transferred;
    std::string response;
    /**
     * TODO(!) - we want to define these strings as constants somewhere in source code
     * and pull from there in tests--this way tests are kept in sync w the responses
     * our request handler are programmed to be returning
     */
    string good_request = 
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "\r\n";
    string bad_request = 
      "HTTP/1.1 400 Bad Request\r\n"
      "Content-Type: text/plain\r\n"
      "\r\n"
      "Bad Request\r\n\r\n";
};

TEST_F(RequestHandlerTest, Simple_Bad_Request) {
  response = test_request_handler.handle_bad_request();

  EXPECT_TRUE(response.compare(bad_request) == 0);
}

TEST_F(RequestHandlerTest, Simple_Good_Request) {
  std::map<std::string, std::string> handler_table;
  std::map<std::string, std::string> location_table;
  
  handler_table.insert(std::pair <std::string, std::string> ("/echo", "echo"));

  request_handler test_request_handler(handler_table, location_table);

  request = "GET /echo HTTP/1.1\r\n\r\nHello";
  array<char, 8192> data{};
  copy(request.begin(), request.end(), data.data());
  bytes_transferred = data.size() + 1;
  
  Request request_;
  request_.setUri("/echo");

  response = test_request_handler.handle_good_request(request_, data);

  EXPECT_TRUE(response.compare(good_request + request) == 0);

}

/**
 * TODO(aniewiadomski) -- add unit tests for handling request types to echo/static.
 */