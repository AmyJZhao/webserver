#include "gtest/gtest.h"
#include "proxy_request_handler.h"
#include "config_parser.h"

class ProxyRequestHandlerTest : public ::testing::Test
{
  protected:
    request_handler* handler;
    Request request;
    Response response;

    ProxyRequestHandlerTest()
    {
        ProxyRequestHandler* prh = new ProxyRequestHandler;
        NginxConfig config;
        NginxConfigStatement statement;
        statement.setHost("www.ucla.edu");
        statement.setPort(80);
        config.statements_.push_back(std::make_shared<NginxConfigStatement>(statement));
        handler = prh->Init("/proxy", config);

        request.setMethod("GET");
    }

    ~ProxyRequestHandlerTest()
    {
        delete handler;
    }
};

TEST_F(ProxyRequestHandlerTest, DestinationWithoutUri)
{
  request.setUri("/ucla/");
  response = handler->handle_request(request);
  EXPECT_EQ(response.code_, StatusCode::InternalServerError);
}

TEST_F(ProxyRequestHandlerTest, SendingGoodRequest)
{
  request.setUri("/proxy/");
  response = handler->handle_request(request);
  EXPECT_EQ(response.code_, StatusCode::OK);

  std::string content_type = response.headers_["content-type"];
  std::cout << content_type.substr(0, 9) << std::endl;
  EXPECT_EQ(content_type.substr(0, 9), "text/html");
}

// www.ucla.edu/admissions redirect to www.ucla.edu/admission
TEST_F(ProxyRequestHandlerTest, SendingRedirectRequest)
{
  request.setUri("/proxy/admissions");
  response = handler->handle_request(request);
  EXPECT_EQ(response.code_, StatusCode::OK);
    
  std::string content_type = response.headers_["content-type"];
  EXPECT_EQ(content_type.substr(0, 9), "text/html");
}