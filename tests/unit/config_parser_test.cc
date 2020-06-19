#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>

class NginxConfigParserTest : public ::testing::Test
{
protected:
  NginxConfigParser parser;
  NginxConfig out_config;
};

TEST_F(NginxConfigParserTest, SimpleConfig)
{
  bool success = parser.Parse("example_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, HelloWorld)
{
  std::stringstream config("hello world;");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, EmptyFile)
{
  std::stringstream config("");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, NonexistentFile)
{
  bool success = parser.Parse("nonexistent_file", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingSemicolon)
{
  std::stringstream config("hello world");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, IncompleteDirective)
{
  std::stringstream config("world;");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CRLF)
{
  std::stringstream config("hello world;\r\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, SpacingIssues1)
{
  bool success = parser.Parse("newline_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, SpacingIssues2)
{
  bool success = parser.Parse("tab_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, MissingBracket1)
{
  bool success = parser.Parse("missingbracket1_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingBracket2)
{
  bool success = parser.Parse("missingbracket2_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingBracket3)
{
  bool success = parser.Parse("missingbracket3_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingBracket4)
{
  bool success = parser.Parse("missingbracket4_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingBracket5)
{
  bool success = parser.Parse("missingbracket5_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, Comments)
{
  bool success = parser.Parse("comments_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NestedBlocks)
{
  bool success = parser.Parse("nestedblocks_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NestedBlocks2)
{
  bool success = parser.Parse("nestedblocks2_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, MissingQuote1)
{
  bool success = parser.Parse("missingquote1_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MissingQuote2)
{
  bool success = parser.Parse("missingquote2_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, NewQuotes)
{
  std::stringstream config("listen 8000;\n"
                           "location \"/echo\" EchoHandler {\n"
                           "}\n"
                           "location \"/static1\" StaticHandler {\n"
                           "  root \"/static_files1\";\n"
                           "}\n"
                           "location \"/static2\" StaticHandler {\n"
                           "  root \"/static_files2\";\n"
                           "}");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewMissingQuotes)
{
  std::stringstream config("listen 8000;\n"
                           "location \"/echo EchoHandler {\n"
                           "}\n"
                           "location \"/static1\" StaticHandler {\n"
                           "  root \"/static_files1\";\n"
                           "}\n"
                           "location \"/static2\" StaticHandler {\n"
                           "  root \"/static_files2\";\n"
                           "}");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, NewMissingQuotesInRoot)
{
  std::stringstream config("listen 8000;\n"
                           "location \"/echo\" EchoHandler {\n"
                           "}\n"
                           "location \"/static1\" StaticHandler {\n"
                           "  root /static_files1\";\n"
                           "}\n"
                           "location \"/static2\" StaticHandler {\n"
                           "  root \"/static_files2\";\n"
                           "}");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes1)
{
  std::stringstream config(
      "listen 4189129;\n"
      "location \"/\" EchoHandler {\n"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes2)
{
  std::stringstream config(
      "location \"/static1\" StaticHandler {\n"
      "  root \"/static_files1\";\n"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes3)
{
  std::stringstream config(
      "location \"/static1\" StaticHandler {\n"
      "  root \"/static_files1\";\n"
      "  listen 8000;"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes4)
{
  std::stringstream config(
      "location \"/echo\" StaticHandler {\n"
      "  root \"/static_files1\";\n"
      "  listen 8000;"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes5)
{
  std::stringstream config(
      "location \"/echo\" EchoHandler {\n"
      "  root \"/static_files1\";\n"
      "  listen 8000;"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes6)
{
  std::stringstream config(
      "location \"/echo\" StaticHandler {\n"
      "  root \"/static_files1\";\n"
      "  root \"/static_files2\";\n"
      "  listen 8000;"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes7)
{
  std::stringstream config(
      "location \"/static\" EchoHandler {\n"
      "  root \"/static_files1\";\n"
      "  root \"/static_files2\";\n"
      "  listen 8000;"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NewPortQuotes8)
{
  std::stringstream config(
      "  listen 8000;"
      "location \"/static\" EchoHandler {\n"
      "  root \"/static_files1\";\n"
      "}\n");
  bool success = parser.Parse(&config, &out_config);
  EXPECT_TRUE(success);
}