// Amy Zhao
// An nginx config file parser.
//
// See:
//   http://wiki.nginx.org/Configuration
//   http://blog.martinfjordvald.com/2010/07/nginx-primer/
//
// How Nginx does it:
//   http://lxr.nginx.org/source/src/core/ngx_conf_file.c

#include <cstdio>
#include <fstream>
#include <stack>

#include "config_parser.h"

std::string removeQuotesIfAny(std::string item)
{
  if (item.size() > 1 && item[0] == '"')
  {
    return std::string(item.begin() + 1, item.end() - 1);
  }
  return item;
}

//
// NginxConfigStatement
//

NginxConfigStatement::NginxConfigStatement() : url_(""), statement_type_(Statement_Type::UNSPECIFIED), root_(""), port_(0), host_("")
{
}

NginxConfigStatement::NginxConfigStatement(NginxConfigStatement &other)
{
  url_ = other.getUrl();
  statement_type_ = other.getStatementType();
  root_ = other.getRoot();
  port_ = other.getPort();
  host_ = other.getHost();
}

std::string NginxConfigStatement::getInfo()
{
  return "URL: " + url_ + "\nStatement Type: " + std::to_string(statement_type_) + "\nRoot Location: " + root_ + "\nPort Number: " + std::to_string(port_) + "\nHost Location: " + host_ + "\n";
}

std::string NginxConfigStatement::getUrl() { return url_; }
NginxConfigStatement &NginxConfigStatement::setUrl(std::string url)
{
  url_ = url;
  return *this;
}

NginxConfigStatement::Statement_Type NginxConfigStatement::getStatementType() { return statement_type_; }
NginxConfigStatement &NginxConfigStatement::setStatementType(NginxConfigStatement::Statement_Type statement_type)
{
  statement_type_ = statement_type;
  return *this;
}

std::string NginxConfigStatement::getRoot() { return root_; }
NginxConfigStatement &NginxConfigStatement::setRoot(std::string root)
{
  root_ = root;
  return *this;
}

unsigned short NginxConfigStatement::getPort() { return port_; }
NginxConfigStatement &NginxConfigStatement::setPort(unsigned short port)
{
  port_ = port;
  return *this;
}

std::string NginxConfigStatement::getHost() { return host_; }
NginxConfigStatement &NginxConfigStatement::setHost(std::string host)
{
  host_ = host;
  return *this;
}

std::string NginxConfigStatement::ToString(int depth)
{
  std::string serialized_statement;
  for (int i = 0; i < depth; ++i)
  {
    serialized_statement.append("  ");
  }
  for (unsigned int i = 0; i < tokens_.size(); ++i)
  {
    if (i != 0)
    {
      serialized_statement.append(" ");
    }
    serialized_statement.append(tokens_[i]);
  }
  if (child_block_.get() != nullptr)
  {
    serialized_statement.append(" {\n");
    serialized_statement.append(child_block_->ToString(depth + 1));
    for (int i = 0; i < depth; ++i)
    {
      serialized_statement.append("  ");
    }
    serialized_statement.append("}");
  }
  else
  {
    serialized_statement.append(";");
  }
  serialized_statement.append("\n");
  return serialized_statement;
}

NginxConfigStatement::Statement_Type NginxConfig::inferStatementType(std::string statement_token)
{
  if (statement_token == "EchoHandler")
  {
    return NginxConfigStatement::Statement_Type::ECHO_HANDLER;
  }
  else if (statement_token == "StaticHandler")
  {
    return NginxConfigStatement::Statement_Type::STATIC_HANDLER;
  }
  else if (statement_token == "StatusHandler")
  {
    return NginxConfigStatement::Statement_Type::STATUS_HANDLER;
  }
  else if (statement_token == "ProxyHandler")
  {
    return NginxConfigStatement::Statement_Type::PROXY_HANDLER;
  }
  else if (statement_token == "HealthHandler")
  {
    return NginxConfigStatement::Statement_Type::HEALTH_HANDLER;
  }
  else if (statement_token == "LeaderboardHandler")
  {
    return NginxConfigStatement::Statement_Type::LEADERBOARD_HANDLER;
  }
  else if (statement_token == "SnakeHandler")
  {
    return NginxConfigStatement::Statement_Type::SNAKE_HANDLER;
  }
  else
  {
    return NginxConfigStatement::Statement_Type::UNSPECIFIED;
  }
}

//
// NginxConfig
//

NginxConfig::NginxConfig()
{
}

NginxConfig::NginxConfig(NginxConfigStatement statement)
{
  statements_.emplace_back(std::shared_ptr<NginxConfigStatement>(new NginxConfigStatement(statement)));
}

std::string NginxConfig::ToString(int depth)
{
  std::string serialized_config;
  for (const auto &statement : statements_)
  {
    serialized_config.append(statement->ToString(depth));
  }
  return serialized_config;
}

std::vector<std::shared_ptr<NginxConfig>> NginxConfig::parse_handlers()
{
  for (const auto &statement : statements_)
  {
    if (statement->tokens_[0] == "listen")
    {
      if (statement->child_block_.get() != nullptr)
      {
        // return an error
        printf("Error: listen directive should not have a child block.\n");
        return {};
      }
      statement->setPort(stoi(statement->tokens_[1])).setStatementType(NginxConfigStatement::Statement_Type::PORT);
    }
    else if (statement->tokens_[0] == "location")
    {
      if (statement->child_block_.get() == nullptr) // location requires child block
      {
        // return an error
        printf("Error: location directive requires a child block, even if empty.\n");
        return {};
      }
      std::string url = removeQuotesIfAny(statement->tokens_[1]);
      NginxConfigStatement::Statement_Type statement_type = inferStatementType(statement->tokens_[2]);
      if (statement_type == NginxConfigStatement::Statement_Type::UNSPECIFIED)
      {
        // return an error
        printf("Error: location directive does not specify a proper handler type.\n");
        return {};
      }
      if (url == "")
      {
        // return an error
        printf("Error: location directive does not specify a URL path.\n");
        return {};
      }
      if (statement_type == NginxConfigStatement::Statement_Type::STATIC_HANDLER)
      {
        std::string root = removeQuotesIfAny(statement->child_block_->parseForRoot());
        if (root != "")
        {
          statement->setRoot(root);
        }
        else
        {
          // return an error
          printf("Error: StaticHandler requires a root directive specifying the internal location of static files.\n");
          return {};
        }
      }
      if (statement_type == NginxConfigStatement::Statement_Type::PROXY_HANDLER)
      {
        std::string host = removeQuotesIfAny(statement->child_block_->parseForHost());
        int port = statement->child_block_->getProxyPort();

        // Host must be present
        if (host != "")
        {
          statement->setHost(host);
        }
        else
        {
          printf("Error: ProxyHandler requires a host directive specifiying the host of the proxy.");
          return {};
        }

        // Post may or may not be present
        if (port != -1) 
        {
          statement->setPort((unsigned short) port);  
        }
      }
      if (statement_type == NginxConfigStatement::Statement_Type::SNAKE_HANDLER)
      {
        std::string root = removeQuotesIfAny(statement->child_block_->parseForRoot());
        if (root != "")
        {
          statement->setRoot(root);
        }
        else
        {
          // return an error
          printf("Error: SnakeHandler requires a root directive specifying the internal location of game files.\n");
          return {};
        }
      }
      statement->setStatementType(statement_type).setUrl(url);
    }
  }

  std::vector<std::shared_ptr<NginxConfig>> retval;
  for (const auto &statement : statements_)
  {
    retval.emplace_back(new NginxConfig(*statement));
  }
  return retval;
}

std::string NginxConfig::getUrl()
{
  if (!statements_.empty())
  {
    return statements_[0]->getUrl();
  }
  return "";
}

std::string NginxConfig::getRoot() const
{
  if (!statements_.empty())
  {
    return statements_[0]->getRoot();
  }
  return "";
}

void NginxConfig::getProxyVariables(std::string &host, unsigned short &port) const
{
  if (!statements_.empty())
  {
    host = statements_[0]->getHost();
    port = statements_[0]->getPort();
    return;
  }
  host = "";
  port = 0;
  return;
}

NginxConfigStatement::Statement_Type NginxConfig::getStatementType()
{
  if (!statements_.empty())
  {
    return statements_[0]->getStatementType();
  }
  return NginxConfigStatement::Statement_Type::UNSPECIFIED;
}

std::string NginxConfig::parseForRoot()
{
  std::string root = "";
  for (const auto &statement : statements_)
  {
    if (statement->tokens_[0] == "root")
    {
      root = statement->tokens_[1];
    }
  }
  return root;
}

std::string NginxConfig::parseForHost()
{
  std::string host = "";
  for (const auto &statement : statements_)
  {
    if (statement->tokens_[0] == "host")
    {
      host = statement->tokens_[1];
    }
  }
  return host;
}

int NginxConfig::getProxyPort()
{
  int port = -1;
  for (const auto &statement : statements_)
  {
    if (statement->tokens_[0] == "port")
    {
      try
      {
        port = std::stoi(statement->tokens_[1]);
      }
      catch (...)
      {
        return -1;
      }
      if (port <= 0 || port >= 65535)
      {
        return -1;
      }
    }
  }

  return port;
}

void NginxConfig::get_port_num(unsigned short &port)
{
  for (const auto &statement : statements_)
  {
    if (statement->tokens_[0] == "listen")
    {
      int config_port = 0;
      try
      {
        config_port = std::stoi(statement->tokens_[1]);
      }
      catch (...)
      {
        return;
      }
      if (config_port <= 0 || config_port >= 65535)
      {
        return;
      }
      port = (unsigned short)config_port;
    }
  }
}

std::string NginxConfig::getInfo()
{
  std::string s = "";
  for (const auto &statement : statements_)
  {
    s += statement->getInfo();
    s += "\n";
  }
  return s;
}

//
// NginxConfigParser
//

const char *NginxConfigParser::TokenTypeAsString(TokenType type)
{
  switch (type)
  {
  case TOKEN_TYPE_START:
    return "TOKEN_TYPE_START";
  case TOKEN_TYPE_NORMAL:
    return "TOKEN_TYPE_NORMAL";
  case TOKEN_TYPE_START_BLOCK:
    return "TOKEN_TYPE_START_BLOCK";
  case TOKEN_TYPE_END_BLOCK:
    return "TOKEN_TYPE_END_BLOCK";
  case TOKEN_TYPE_COMMENT:
    return "TOKEN_TYPE_COMMENT";
  case TOKEN_TYPE_STATEMENT_END:
    return "TOKEN_TYPE_STATEMENT_END";
  case TOKEN_TYPE_EOF:
    return "TOKEN_TYPE_EOF";
  case TOKEN_TYPE_ERROR:
    return "TOKEN_TYPE_ERROR";
  case TOKEN_TYPE_QUOTE:
    return "TOKEN_TYPE_QUOTE";
  default:
    return "Unknown token type";
  }
}

NginxConfigParser::TokenType NginxConfigParser::ParseToken(std::istream *input,
                                                           std::string *value)
{
  TokenParserState state = TOKEN_STATE_INITIAL_WHITESPACE;
  while (input->good())
  {
    const char c = input->get();
    if (!input->good())
    {
      break;
    }
    switch (state)
    {
    case TOKEN_STATE_INITIAL_WHITESPACE:
      switch (c)
      {
      case '{':
        *value = c;
        return TOKEN_TYPE_START_BLOCK;
      case '}':
        *value = c;
        return TOKEN_TYPE_END_BLOCK;
      case '#':
        *value = c;
        state = TOKEN_STATE_TOKEN_TYPE_COMMENT;
        continue;
      case '"':
        *value = c;
        state = TOKEN_STATE_DOUBLE_QUOTE;
        continue;
      case '\'':
        *value = c;
        state = TOKEN_STATE_SINGLE_QUOTE;
        continue;
      case ';':
        *value = c;
        return TOKEN_TYPE_STATEMENT_END;
      case ' ':
      case '\t':
      case '\n':
      case '\r':
        continue;
      default:
        *value += c;
        state = TOKEN_STATE_TOKEN_TYPE_NORMAL;
        continue;
      }
    case TOKEN_STATE_SINGLE_QUOTE:
      *value += c;
      if (c == '\'')
      {
        return TOKEN_TYPE_NORMAL;
      }
      continue;
    case TOKEN_STATE_DOUBLE_QUOTE:
      *value += c;
      if (c == '"')
      {
        const char c_next = input->get();
        if (c_next == ' ' || c_next == '\t' || c_next == '\n' ||
            c_next == ';' || c_next == '{' || c_next == '}')
          input->unget();
        else
          return TOKEN_TYPE_ERROR;

        return TOKEN_TYPE_NORMAL;
      }
      else if (c == '\\')
      {
        value->pop_back();
        state = TOKEN_STATE_end_DOUBLE_QUOTE;
      }
      continue;
    case TOKEN_STATE_TOKEN_TYPE_COMMENT:
      if (c == '\n' || c == '\r')
      {
        return TOKEN_TYPE_COMMENT;
      }
      *value += c;
      continue;
    case TOKEN_STATE_TOKEN_TYPE_NORMAL:
      if (c == ' ' || c == '\t' || c == '\n' || c == '\t' ||
          c == ';' || c == '{' || c == '}')
      {
        input->unget();
        return TOKEN_TYPE_NORMAL;
      }
      if (c == '"')
        return TOKEN_TYPE_ERROR;
      *value += c;

      continue;
    }
  }

  if (state == TOKEN_STATE_SINGLE_QUOTE ||
      state == TOKEN_STATE_DOUBLE_QUOTE)
  {
    return TOKEN_TYPE_ERROR;
  }

  return TOKEN_TYPE_EOF;
}

bool NginxConfigParser::Parse(std::istream *config_file, NginxConfig *config)
{
  std::stack<NginxConfig *> config_stack;
  config_stack.push(config);
  TokenType last_token_type = TOKEN_TYPE_START;
  TokenType token_type;
  int brackets_diff = 0;
  int normal_tokens = 0;
  while (true)
  {
    std::string token;
    token_type = ParseToken(config_file, &token);
    //printf ("%s: %s\n", TokenTypeAsString(token_type), token.c_str());
    if (token_type == TOKEN_TYPE_ERROR)
    {
      break;
    }

    if (token_type == TOKEN_TYPE_COMMENT)
    {
      // Skip comments.
      continue;
    }

    if (token_type == TOKEN_TYPE_START)
    {
      // Error.
      break;
    }
    else if (token_type == TOKEN_TYPE_NORMAL)
    {
      //increment normal tokens
      normal_tokens += 1;
      if (last_token_type == TOKEN_TYPE_START ||
          last_token_type == TOKEN_TYPE_STATEMENT_END ||
          last_token_type == TOKEN_TYPE_START_BLOCK ||
          last_token_type == TOKEN_TYPE_END_BLOCK ||
          last_token_type == TOKEN_TYPE_NORMAL)
      {
        if (last_token_type != TOKEN_TYPE_NORMAL)
        {
          config_stack.top()->statements_.emplace_back(
              new NginxConfigStatement);
        }
        config_stack.top()->statements_.back().get()->tokens_.push_back(
            token);
      }
      else
        break;
    }
    else if (token_type == TOKEN_TYPE_STATEMENT_END)
    {
      if (last_token_type != TOKEN_TYPE_NORMAL)
      {
        // Error.
        break;
      }
      //normal directives should have two normal tokens
      if (normal_tokens != 2)
        break;
      normal_tokens = 0;
    }
    else if (token_type == TOKEN_TYPE_START_BLOCK)
    {
      if (last_token_type != TOKEN_TYPE_NORMAL)
      {
        // Error.
        break;
      }
      //reset number of normal tokens back to 0
      normal_tokens = 0;
      //increment bracket count
      brackets_diff += 1;
      NginxConfig *const new_config = new NginxConfig;
      config_stack.top()->statements_.back().get()->child_block_.reset(
          new_config);
      config_stack.push(new_config);
    }
    else if (token_type == TOKEN_TYPE_END_BLOCK)
    {
      //expand if statement so nested blocks (contexts) and empty blocks don't return an error
      if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_END_BLOCK &&
          last_token_type != TOKEN_TYPE_START_BLOCK)
      {
        // Error.
        break;
      }
      //decrement bracket count
      brackets_diff -= 1;
      //if end bracket has no start bracket, return an error
      if (brackets_diff < 0)
        break;
      config_stack.pop();
    }
    else if (token_type == TOKEN_TYPE_EOF)
    {
      //the bracket count should be 0 since
      //the number of start and end brackets should be the same
      if (brackets_diff != 0)
        break;
      if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_END_BLOCK)
      {
        // Error.
        break;
      }
      return true;
    }
    else
    {
      // Error. Unknown token.
      break;
    }
    last_token_type = token_type;
  }

  printf("Bad transition from %s to %s\n",
         TokenTypeAsString(last_token_type),
         TokenTypeAsString(token_type));

  return false;
}

bool NginxConfigParser::Parse(const char *file_name, NginxConfig *config)
{
  std::ifstream config_file;
  config_file.open(file_name);
  if (!config_file.good())
  {
    printf("Failed to open config file: %s\n", file_name);
    return false;
  }

  const bool return_value =
      Parse(dynamic_cast<std::istream *>(&config_file), config);
  config_file.close();
  return return_value;
}
