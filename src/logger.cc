#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/thread/thread.hpp>
#include <string>
#include <algorithm>
#include <cctype>
#include "logger.h"
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
using boost::asio::ip::tcp; 

Logger::Logger()
{
    init();
    logging::add_common_attributes();
    logging::record rec = loggy.open_record();
    logging::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%]:[%ThreadID%]:%Message%");
}
void Logger::init()
{
    logging::add_file_log(
        keywords::file_name = "../log/LOG_FILE_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "[%TimeStamp%]:[%ThreadID%]:%Message%",
        keywords::auto_flush = true 
    );
}

void Logger::logHello()
{
    BOOST_LOG_SEV(loggy, trace) << "Server is started.\n";
}

std::string str_toupper(std::string s) 
{
    std::transform(s.begin(), s.end(), s.begin(), 
                   [](unsigned char c){return std::toupper(c);});
    return s;
}

void  Logger::logError(std::string location, std::string error_message)
{
    BOOST_LOG_SEV(loggy, error) << "[ERROR]" << "[" << location << "] " << error_message;
}

void  Logger::logDebug(std::string location, std::string info_message)
{
    BOOST_LOG_SEV(loggy, debug) << "[DEBUG]" << "[" <<  location << "] " << info_message;
}

void  Logger::logFatal(std::string location, std::string fatal_message)
{
    BOOST_LOG_SEV(loggy, fatal) << "[FATAL]" << "[" <<  location << "] " << fatal_message;
}

void Logger::logBye()
{
    BOOST_LOG_SEV(Logger::loggy, warning) << "Shutting-down server. \n";
}

Logger *Logger::logger = 0;