#include "gtest/gtest.h"
#include "logger.h"
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include "session.h"

class LoggerTest : public ::testing::Test
{
protected:
    Logger *logger = Logger::getLogger();
    char c;
    std::ifstream logfile;
    std::string Message = "test";
};

TEST_F(LoggerTest, logInit)
{
    std::string expected_msg = "Server is started.\n";
    logger->logHello();

    logfile.open("../log/LOG_FILE_0.log");
    std::string body;
    while (logfile.get(c))
        body += c;
    logfile.close();

    int start_idx = body.rfind("]") + 1;
    std::string real_msg = body.substr(start_idx);

    EXPECT_TRUE(real_msg.find(expected_msg,0));
}

TEST_F(LoggerTest, LogFormatDateTime)
{
    logger->logError("in logger test. ", Message);

    logfile.open("../log/LOG_FILE_0.log");
    std::string body;
    while (logfile.get(c))
        body += c;
    logfile.close();

    EXPECT_TRUE(body.find("2020",0));
    //change this next year ^
}

TEST_F(LoggerTest, LogFormatThreadID)
{
    logger->logError("in logger test. ", Message);

    logfile.open("../log/LOG_FILE_0.log");
    std::string body;
    while (logfile.get(c))
        body += c;
    logfile.close();

    EXPECT_TRUE(body.find("[0x0",0));
    //apparently all thread IDs start like so ^
}

TEST_F(LoggerTest, logError)
{
    logger->logError("in logger test. ", Message);

    logfile.open("../log/LOG_FILE_0.log");
    std::string body;
    while (logfile.get(c))
        body += c;
    logfile.close();

    int start_idx = body.rfind("]") + 1;
    std::string real_msg = body.substr(start_idx);

    EXPECT_TRUE(real_msg.find("[ERROR]",0));
}

TEST_F(LoggerTest, logDebug)
{
    logger->logDebug("in logger test. ", Message);

    logfile.open("../log/LOG_FILE_0.log");
    std::string body;
    while (logfile.get(c))
        body += c;
    logfile.close();

    int start_idx = body.rfind("]") + 1;
    std::string real_msg = body.substr(start_idx);

    EXPECT_TRUE(real_msg.find("[DEBUG]",0));
}

TEST_F(LoggerTest, logFatal)
{
    logger->logFatal("in logger test. ", Message);

    logfile.open("../log/LOG_FILE_0.log");
    std::string body;
    while (logfile.get(c))
        body += c;
    logfile.close();

    int start_idx = body.rfind("]") + 1;
    std::string real_msg = body.substr(start_idx);

    EXPECT_TRUE(real_msg.find("[FATAL]",0));
}

TEST_F(LoggerTest, logBye)
{
    logger->logBye();

    logfile.open("../log/LOG_FILE_0.log");
    std::string body;
    while (logfile.get(c))
        body += c;
    logfile.close();

    int start_idx = body.rfind("]") + 1;
    std::string real_msg = body.substr(start_idx);

    EXPECT_TRUE(real_msg.find("Shutting-down server.",0));
}
