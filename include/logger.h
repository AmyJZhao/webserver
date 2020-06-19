#ifndef LOG_H
#define LOG_H
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/signals2.hpp>

using boost::asio::ip::tcp;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

class Logger
{
    // log output format
    // [time stamp] [thread ID] [severity] [location] message
    // client provides location in capital 
    // and message string
    // chose function baed on severity
public:
    src::severity_logger<severity_level> loggy;
    Logger();
    void init();
    static Logger *logger;

    void logHello();//call during beginning of server
    
    void logFatal(std::string location, std::string fatal_message);
    void logError(std::string location, std::string error_message);
    void logDebug(std::string location, std::string info_message);

    void logBye(); //call during ending of server

    static Logger *getLogger()
    {
        if (Logger::logger == 0)
            Logger::logger = new Logger();
        return Logger::logger;
    }
};
#endif