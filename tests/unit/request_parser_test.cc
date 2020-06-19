#include "gtest/gtest.h"
#include "request_parser.h"
#include <sstream>
#include <array>

// tests are modified from: https://www.tutorialspoint.com/http/http_requests.htm

class RequestParserTest : public ::testing::Test {
  protected:
    request_parser                rp;
    Request                       request;
    request_parser::result_type   result;
};

TEST_F(RequestParserTest, SimpleRequest) {
  const char* cstr = "GET /hello.htm HTTP/1.1\r\n\r\n";
  std::array<char,28> data_;
  memcpy(data_.data(), cstr, 28);

  std::tie(result, std::ignore) = rp.parse(
    request, 
    data_.data(), 
    data_.data() + 28);

  EXPECT_TRUE(result == request_parser::good);
}

TEST_F(RequestParserTest, LongerRequest) {
  const char* cstr = "GET /hello.htm HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nConnection: Keep-Alive\r\n\r\n";
  std::array<char,82> data_;
  memcpy(data_.data(), cstr, 82);

  std::tie(result, std::ignore) = rp.parse(
    request, 
    data_.data(), 
    data_.data() + 82);

  EXPECT_TRUE(result == request_parser::good);
}

TEST_F(RequestParserTest, MissingGET) {
  const char* cstr = "User-Agent: Mozilla/4.0\r\n\r\n";
  std::array<char,28> data_;
  memcpy(data_.data(), cstr, 28);

  std::tie(result, std::ignore) = rp.parse(
    request, 
    data_.data(), 
    data_.data() + 28);

  EXPECT_TRUE(result == request_parser::bad);
}

TEST_F(RequestParserTest, ColonAfterGET) {
  const char* cstr = "GET: /hello.htm HTTP/1.1\r\n\r\n";
  std::array<char,28> data_;
  memcpy(data_.data(), cstr, 28);

  std::tie(result, std::ignore) = rp.parse(
    request, 
    data_.data(), 
    data_.data() + 28);

  EXPECT_TRUE(result == request_parser::bad);
}