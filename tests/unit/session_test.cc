#include <boost/asio.hpp>
#include <string>
#include "gtest/gtest.h"
#include "session.h"
#include <fstream>
#include "logger.h"
#include "request_handler.h"
#include "request_parser.h"
#include <map>
#include "request.h"

using boost::asio::ip::tcp;

class MockSession : public session 
{
public:
  MockSession(boost::asio::io_service& io_service, std::ifstream& is, 
  std::map<std::string, std::string> handler_table, std::map<std::string, std::string> location_table, std::map<std::string, request_handler*> path_map)
    : session(io_service, handler_table, location_table, path_map),
      written_data_(""),
      read_data_(""),
      bytes_read_(0),
      inputBuffer(is) {}
  ~MockSession() {}
  void start() 
  { 
    do_read();
  }

  void handle_read()
  {
    std::array<char, 8192> data_array;
    std::copy(std::begin(read_data_), std::end(read_data_), std::begin(data_array));

    request_parser::result_type result;
    std::tie(result, std::ignore) = request_parser_.parse(
        request_, data_array.data(), data_array.data() + bytes_read_);

    if (result == request_parser::good)
    {
      std::string response = request_handler_.handle_good_request(request_, data_array);
      do_write(response);
    }
    else if (result == request_parser::bad)
    {
      std::string response = request_handler_.handle_bad_request(); 
      do_write(response);
    }

  }
  
  std::string get_request() { return read_data_; }
  std::string get_response() { return written_data_; }

private:
  void do_read()
  {
    try
    {
      // Read from input
      inputBuffer.seekg(0, inputBuffer.end);
      int size = inputBuffer.tellg();
      inputBuffer.seekg(0, inputBuffer.beg);
      char* data = new char[size];

      inputBuffer.read(data, size);
      
      // Save read data
      read_data_ = std::string(data);
      bytes_read_ = size;

      // Close buffer and delete dynamic char array
      inputBuffer.clear();
      inputBuffer.close();
      delete[] data;
    } 
    catch (const std::exception& e)
    {
      std::cout << "Error reading from input\n";
    }
  }

  void do_write(std::string data)
  {
    written_data_ = data;
  }

  std::ifstream& inputBuffer;
  std::string read_data_;
  size_t bytes_read_;
  std::string written_data_;
  Request request_;
  request_parser            request_parser_;
};

class SessionTest : public ::testing::Test {
  protected:
    boost::asio::io_service io_service;
    std::map<std::string, std::string> handler_table;
    std::map<std::string, std::string> location_table;
    std::map<std::string, request_handler*> path_map_;
    session test_session = session(io_service, handler_table, location_table, path_map_);

    std::string good_request = "GET /echo HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nConnection: Keep-Alive\r\n\r\n";
    std::string good_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET /echo HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nConnection: Keep-Alive\r\n\r\n";
    std::string bad_response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad Request";
};

TEST_F(SessionTest, ConstructorTest) {
  EXPECT_EQ(&io_service, &test_session.socket().get_io_service());
}

// TEST_F(SessionTest, GoodRequestTest) {
//   // TODO(!): change relative paths to absolute paths?
//   // Currently assuming running from /build
//   std::ifstream is("../tests/request_examples/good_request.txt");
//   handler_table.insert(std::pair <std::string, std::string> ("/echo", "echo"));

//   MockSession s(io_service, is, handler_table, location_table);
//   s.start();
//   EXPECT_EQ(s.get_request(), good_request);
//   s.handle_read();
//   EXPECT_EQ(s.get_response(), good_response);
// }

// TEST_F(SessionTest, ReadBadRequestTest) {
//   // TODO(!): change relative paths to absolute paths?
//   // Currently assuming running from /build
//   std::ifstream is("../tests/request_examples/bad_request.txt");
//   MockSession s(io_service, is, handler_table, location_table);
//   s.start();
//   EXPECT_EQ(s.get_request(), "hi\nanother line\n\n\nhi");
//   s.handle_read();
//   EXPECT_EQ(s.get_response(), bad_response);
// }