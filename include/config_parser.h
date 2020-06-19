// An nginx config file parser.

#ifndef CONFIG_STATEMENT_H
#define CONFIG_STATEMENT_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class NginxConfig;

// The parsed representation of a single config statement.
class NginxConfigStatement
{
public:
  NginxConfigStatement();
  NginxConfigStatement(NginxConfigStatement &other);
  std::string ToString(int depth);
  std::vector<std::string> tokens_;
  std::unique_ptr<NginxConfig> child_block_;
  std::string getInfo();
  enum Statement_Type
  {
    UNSPECIFIED,
    PORT,
    ECHO_HANDLER,
    STATIC_HANDLER,
    STATUS_HANDLER,
    NOT_FOUND_HANDLER,
    PROXY_HANDLER,
    HEALTH_HANDLER,
    LEADERBOARD_HANDLER,
    SNAKE_HANDLER
  };

  // getters and setters
  std::string getUrl();
  NginxConfigStatement &setUrl(std::string url);
  Statement_Type getStatementType();
  NginxConfigStatement &setStatementType(Statement_Type statement_type);
  std::string getRoot();
  NginxConfigStatement &setRoot(std::string root);
  unsigned short getPort();
  NginxConfigStatement &setPort(unsigned short port);
  std::string getHost();
  NginxConfigStatement &setHost(std::string host);

private:
  std::string url_;
  Statement_Type statement_type_;
  std::string root_;
  unsigned short port_;
  std::string host_;
};

// The parsed representation of the entire config.
class NginxConfig
{
public:
  NginxConfig();
  NginxConfig(NginxConfigStatement statement);
  std::string ToString(int depth = 0);
  void get_port_num(unsigned short &port);
  std::vector<std::shared_ptr<NginxConfig>> parse_handlers();
  std::vector<std::shared_ptr<NginxConfigStatement>> statements_;
  std::string getInfo();
  std::string getUrl();
  std::string getRoot() const;
  void getProxyVariables(std::string &host, unsigned short &port) const;
  NginxConfigStatement::Statement_Type getStatementType();

private:
  std::string parseForRoot();
  int getProxyPort();
  std::string parseForHost();
  NginxConfigStatement::Statement_Type inferStatementType(std::string statement_token);
};

// The driver that parses a config file and generates an NginxConfig.
class NginxConfigParser
{
public:
  NginxConfigParser() {}

  // Take a opened config file or file name (respectively) and store the
  // parsed config in the provided NginxConfig out-param.  Returns true
  // iff the input config file is valid.
  bool Parse(std::istream *config_file, NginxConfig *config);
  bool Parse(const char *file_name, NginxConfig *config);

private:
  enum TokenType
  {
    TOKEN_TYPE_START = 0,
    TOKEN_TYPE_NORMAL = 1,
    TOKEN_TYPE_START_BLOCK = 2,
    TOKEN_TYPE_END_BLOCK = 3,
    TOKEN_TYPE_COMMENT = 4,
    TOKEN_TYPE_STATEMENT_END = 5,
    TOKEN_TYPE_EOF = 6,
    TOKEN_TYPE_ERROR = 7,
    TOKEN_TYPE_QUOTE = 8
  };
  const char *TokenTypeAsString(TokenType type);

  enum TokenParserState
  {
    TOKEN_STATE_INITIAL_WHITESPACE = 0,
    TOKEN_STATE_SINGLE_QUOTE = 1,
    TOKEN_STATE_DOUBLE_QUOTE = 2,
    TOKEN_STATE_TOKEN_TYPE_COMMENT = 3,
    TOKEN_STATE_TOKEN_TYPE_NORMAL = 4,
    TOKEN_STATE_end_DOUBLE_QUOTE = 5
  };

  TokenType ParseToken(std::istream *input, std::string *value);
};

#endif
