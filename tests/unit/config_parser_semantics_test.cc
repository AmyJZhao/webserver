#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <iterator>

class ConfigParserSemanticsTest : public ::testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;

    std::string handler_text = // TODO: add quotes to url and root argument
    "listen 8000;\n"
    "location \"/echo\" EchoHandler {\n"
    "}\n"
    "location /static1 StaticHandler {\n"
    "  root /static_files1;\n"
    "}\n"
    "location \"/static 2\" StaticHandler {\n"
    "  root \"/static files 2\";\n"
    "}\n"
    "location \"/status\" StatusHandler {\n"
    "}\n"
    "location \"/health\" HealthHandler {\n"
    "}\n"
    "location \"/leaderboard\" LeaderboardHandler {\n"
    "}\n"
    "location \"/snake\" SnakeHandler {\n"
    "  root \"/game\";\n"
    "}";
};

//
// unit tests for NginxConfigStatements
//

TEST(ConfigStatementTest, BasicStatement){
  NginxConfigStatement statement;
  EXPECT_TRUE(statement.getUrl() == "");
  EXPECT_TRUE(statement.getStatementType() == NginxConfigStatement::Statement_Type::UNSPECIFIED);
  EXPECT_TRUE(statement.getRoot() == "");
  EXPECT_TRUE(statement.getPort() == 0);
}

TEST(ConfigStatementTest, BuildStatement){
  NginxConfigStatement statement;
  statement.setUrl("my url").setStatementType(NginxConfigStatement::Statement_Type::ECHO_HANDLER).setRoot("my root").setPort(8);
  EXPECT_TRUE(statement.getUrl() == "my url");
  EXPECT_TRUE(statement.getStatementType() == NginxConfigStatement::Statement_Type::ECHO_HANDLER);
  EXPECT_TRUE(statement.getRoot() == "my root");
  EXPECT_TRUE(statement.getPort() == 8);
}

TEST(ConfigStatementTest, CopyStatement){
  NginxConfigStatement statementA;
  statementA.setUrl("my url").setStatementType(NginxConfigStatement::Statement_Type::ECHO_HANDLER).setRoot("my root").setPort(8);
  NginxConfigStatement statementB(statementA);
  EXPECT_TRUE(statementB.getUrl() == "my url");
  EXPECT_TRUE(statementB.getStatementType() == NginxConfigStatement::Statement_Type::ECHO_HANDLER);
  EXPECT_TRUE(statementB.getRoot() == "my root");
  EXPECT_TRUE(statementB.getPort() == 8);
}

//
// unit tests for parsing handler information from config file
//

TEST_F(ConfigParserSemanticsTest, HandlerConfig1) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[1];
  EXPECT_TRUE(block->getUrl() == "/echo");
  EXPECT_TRUE(block->getStatementType() == NginxConfigStatement::Statement_Type::ECHO_HANDLER);
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig2) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[1];
  EXPECT_TRUE(block->getUrl() == "/echo");
  EXPECT_TRUE(block->getRoot() == "");
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig3) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[2];
  EXPECT_TRUE(block->getUrl() == "/static1");
  EXPECT_TRUE(block->getStatementType() == NginxConfigStatement::Statement_Type::STATIC_HANDLER);
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig4) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[2];
  EXPECT_TRUE(block->getUrl() == "/static1");
  EXPECT_TRUE(block->getRoot() == "/static_files1");
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig5) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[3];
  EXPECT_TRUE(block->getUrl() == "/static 2");
  EXPECT_TRUE(block->getStatementType() == NginxConfigStatement::Statement_Type::STATIC_HANDLER);
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig6) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[3];
  EXPECT_TRUE(block->getUrl() == "/static 2");
  EXPECT_TRUE(block->getRoot() == "/static files 2");
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig7) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[4];
  EXPECT_TRUE(block->getUrl() == "/status");
  EXPECT_TRUE(block->getStatementType() == NginxConfigStatement::Statement_Type::STATUS_HANDLER);
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig8) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();

  for (const auto &block : config_blocks){
    EXPECT_TRUE(block->getUrl() != "/nonexistent");
  }
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig9) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  for (const auto &block : config_blocks){
    EXPECT_TRUE(block->getUrl() != "/nonexistent");
  }
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig10) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[5];
  EXPECT_TRUE(block->getUrl() == "/health");
  EXPECT_TRUE(block->getStatementType() == NginxConfigStatement::Statement_Type::HEALTH_HANDLER);
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig11) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[6];
  EXPECT_TRUE(block->getUrl() == "/leaderboard");
  EXPECT_TRUE(block->getStatementType() == NginxConfigStatement::Statement_Type::LEADERBOARD_HANDLER);
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig12) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[7];
  EXPECT_TRUE(block->getUrl() == "/snake");
  EXPECT_TRUE(block->getStatementType() == NginxConfigStatement::Statement_Type::SNAKE_HANDLER);
}

TEST_F(ConfigParserSemanticsTest, HandlerConfig13) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  const auto &block = config_blocks[7];
  EXPECT_TRUE(block->getUrl() == "/snake");
  EXPECT_TRUE(block->getRoot() == "/game");
}

//
// unit tests for parsing port number from config file
//

TEST_F(ConfigParserSemanticsTest, PortConfig1) {
  std::stringstream handler_config(handler_text);
  parser.Parse(&handler_config, &out_config);
  std::map<std::string, std::string>::iterator it;
  std::vector<std::shared_ptr<NginxConfig>> config_blocks = out_config.parse_handlers();
  
  const auto &statement = config_blocks[0]->statements_[0];
  EXPECT_TRUE(statement->getPort() == 8000);
  EXPECT_TRUE(statement->getStatementType() == NginxConfigStatement::Statement_Type::PORT);
}

TEST_F(ConfigParserSemanticsTest, PortConfig2) {
  std::stringstream port_config(handler_text);
  parser.Parse(&port_config, &out_config);
  unsigned short port = 0;
  out_config.get_port_num(port);
  EXPECT_TRUE(port == 8000);
}

TEST_F(ConfigParserSemanticsTest, PortConfig3) {
  std::string port_config_text = 
  "listen 4189129;\n"
  "location / EchoHandler {\n"
  "}\n";

  std::stringstream port_config(port_config_text);
  parser.Parse(&port_config, &out_config);
  unsigned short port = 0;
  out_config.get_port_num(port);
  EXPECT_TRUE(port == 0);
}

TEST_F(ConfigParserSemanticsTest, PortConfig4) {
  std::string port_config_text = 
  "location /static1 StaticHandler {\n"
  "  root /static_files1;\n"
  "}\n";

  std::stringstream port_config(port_config_text);
  parser.Parse(&port_config, &out_config);
  unsigned short port = 0;
  out_config.get_port_num(port);
  EXPECT_TRUE(port == 0);
}

TEST_F(ConfigParserSemanticsTest, PortConfig5) {
  std::string port_config_text = 
  "location /static1 StaticHandler {\n"
  "  root /static_files1;\n"
  "  listen 8000;"
  "}\n";

  std::stringstream port_config(port_config_text);
  parser.Parse(&port_config, &out_config);
  unsigned short port = 0;
  out_config.get_port_num(port);
  EXPECT_TRUE(port == 0);
}
