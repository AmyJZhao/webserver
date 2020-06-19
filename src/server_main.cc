//
// server_main.cc
// ~~~~~~~~~~~~~~
//

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include "session.h"
#include "server.h"
#include "config_parser.h"
#include "logger.h"
#include <csignal>
#include <vector>
#include <string>

using boost::asio::ip::tcp;

void signalHandler(int signum)
{
	std::cout << "Kill signal (" << signum << ") received.\n";
	exit(0);
}

int main(int argc, char *argv[])
{

	Logger *logger = Logger::getLogger();
	logger->logDebug("main in Server_Main", "Logger created.");

	// Signal handler
	signal(SIGTERM, signalHandler);

	try
	{
		//takes one command line parameter, path to config file
		if (argc != 2)
		{
			std::cerr << "Usage: ./server <path_to_config_file>\n";
			return 1;
		}

		//parse config file
		NginxConfigParser config_parser;
		NginxConfig config;
		config_parser.Parse(argv[1], &config);
		unsigned short port = 0;
		//get port number from config file
		config.get_port_num(port);
		if (port == 0)
		{
            logger->logFatal("main in Server Main", "Server unable to get port number from config file.\n");
			return 1;
		}

		std::vector<std::shared_ptr<NginxConfig>>	handler_configs = config.parse_handlers();
		
		logger->logDebug("main in Server Main", "Server has parsed the handlers.");

		boost::asio::io_service io_service;

		using namespace std; // For atoi.
		server s(io_service, port, handler_configs);

        logger->logDebug("main in Server Main", "Server has been successfully initialized.");
        logger->logDebug("main in Server Main", "Server is running.");

		io_service.run();

	}
	catch (std::exception &e)
	{

		std::cerr << "Exception: " << e.what() << "\n";

		logger->logFatal("main in Server Main", std::string("Server has caught an exception: " + std::string(e.what())));
	}
	
	logger->logDebug("main in Server Main", "Server is shutting down. \n");

	return 0;
}
