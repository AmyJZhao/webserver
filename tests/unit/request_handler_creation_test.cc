#include "gtest/gtest.h"
#include "config_parser.h"
#include "static_request_handler.h"
#include "common_request_handler.h"
#include "health_request_handler.h"
#include <sstream>
#include <iterator>
#include <string>

class RequestHandlerCreationTest : public ::testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
};

//
// unit tests for handler creation 
//

TEST_F(RequestHandlerCreationTest, EchoHandler){
  NginxConfigStatement statement;
  statement.setRoot("");
  out_config = NginxConfig(statement);
  common_request_handler testEchoHandler;
  common_request_handler* echoHandler = dynamic_cast<common_request_handler*>(testEchoHandler.Init("/echo", out_config));
  EXPECT_TRUE(echoHandler->getLocationPath() == "/echo");
}

TEST_F(RequestHandlerCreationTest, StaticHandler){
  NginxConfigStatement statement;
  statement.setRoot("/static_files");
  out_config = NginxConfig(statement);
  static_request_handler testStaticHandler;
  static_request_handler* staticHandler = dynamic_cast<static_request_handler*>(testStaticHandler.Init("/static", out_config));
  EXPECT_TRUE(staticHandler->getLocationPath() == "/static" && staticHandler->getDirectory() == "/static_files");
}

TEST_F(RequestHandlerCreationTest, HealthHandler){
  NginxConfigStatement statement;
  statement.setRoot("");
  out_config = NginxConfig(statement);
  HealthRequestHandler testHealthHandler;
  HealthRequestHandler* healthHandler = dynamic_cast<HealthRequestHandler*>(testHealthHandler.Init("/health", out_config));
  EXPECT_TRUE(healthHandler->getLocationPath() == "/health");
}
