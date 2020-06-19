# AAA Batteries Webserver

# Building + Running
## Building code
```
$ cd <repo>
$ mkdir build && cd build
$ cmake ..
$ make
```
## Running code
```
$ ./bin/server ../config/local_dev_config
```

## Testing
### Running unit tests
```
$ cd build
$ make test
```

### Running manual tests
```
$ ./bin/server ../config/local_dev_config
# in another terminal window
$ curl localhost:<port>
```

### Running a build through Docker
To test from a browser locally, assuming the server is running on port `8000` and you want to map it to your machine's `8080`:
```
$ docker build -f docker/base.Dockerfile -t aaa-p2:base .
$ docker build -f docker/Dockerfile -t im .
$ docker run --rm -p 127.0.0.1:8080:8000 --name my_run im:latest
```

# Playing Snake

Our web server features a friendly game of snake for users to play.

To play snake locally, deploy a local docker image and go to http://localhost:8080/snake in your web browser. 
View the leaderboard at http://localhost:8080/leaderboard/dashboard

To play snake on the production server, go to http://aaa.cs130.org/snake in your web browser. 
View the leaderboard at http://aaa.cs130.org/leaderboard/dashboard

# Request handlers
## Interface
Request handlers should all inherit from the abstract `request_handler` class:

```cpp
// request_handler.h
class request_handler
{
public:
	virtual request_handler* Init(const std::string& location_path, const NginxConfig& config) = 0;
	virtual Response  handle_request(const Request& request) = 0;
	void setLocationPath(std::string location_path);
	std::string getLocationPath();

protected:
	// The path that the request handler responds to
	std::string path_;
};
```

### Current request handlers
- `common_request_handler` - echoes back the request the client sent it
- `static_request_handler` - serves static files from a config-specified directory, if they exist on the server
- `NotFoundRequestHandler` - serves a `404` Not Found error when a route was hit that does not exist on the server
- `StatusRequestHandler` - reports server stats (how many of each type of response code was sent out, how many requests were sent to each route)
- `ProxyRequestHandler` - acts as a reverse proxy, issues requests to another server and returns (a modified version of) the response to the client
- `HealthRequestHandler` - returns a `200` OK response, used to check server uptime
- `SnakeRequestHandler` - serves a static file containing a game of snake for users to play
- `LeaderboardRequestHandler` - serves a static file containing the top scores for the snake game served by `SnakeRequestHandler`

## Adding new handlers
### Header file
New handlers should have the following minimal structure in their header file, with `${NewRequestHandlerName}` corresponding to the name of the request handler:
```cpp
// <NewRequestHandlerName>_request_handler.h

#ifndef <NewRequestHandlerName>_REQUEST_HANDLER_H
#define <NewRequestHandlerName>_REQUEST_HANDLER_H

#include <string>
#include "request_handler.h"

class Request;

class <NewRequestHandlerName>request_handler : public request_handler 
{
public:
	request_handler* Init(const std::string& location_prefix, const NginxConfig& config);
	Response handle_request(const Request& request);
};

#endif
```
This header file will go under the directory `/include`.

### Implementation
In `<NewRequestHandlerName>_request_handler.cc`:
#### `Init` method
The initialization of each handler is a static method. It should perform any necessary setting of private member variables, and return `this`.
```cpp
request_handler* <NewRequestHandlerName>_request_handler::Init(const std::string& location_prefix, const NginxConfig& config){
	// Perform any initialization necessary here...
	return this;
}
```
#### `handle_request` method
This method performs the request handling by parsing through a `Request` object and populating a `Response` object. 

```cpp
Response <NewRequestHandlerName>_request_handler::handle_request(const Request& request){
	Response response;
	response.set_status_code(code);
	response.set_MIME(Response::TEXT_PLAIN);
	response.set_content(content);
	return response;
}
```
> Note: The `handle_request` method should assume that the `Request` object's `uri` field is correctly correlated to a route that will be handled by this type of handler (specified in config file). This is taken care of by the session's map of paths to handler requests, constructed at server intialization.

### Updating configuration
#### Config file
The configuration file should be appropriately updated to map routes to your new request handler.
```
location /route <NewRequestHandlerName>Handler {
	# Other directives here...
}
```
#### Config parsing
The configuration parser should also be able to recognize your new handler when it is being parsed. In `config_parser`, the following should be updated:

- `config_parser.h` should have its `Statement_Type` enum updated. Add a new value to `Statement_Type`, named `NewRequestHandlerName_HANDLER`.
- `config_parser.cc` -- the `parse_handlers` method should be updated to be able to parse and store relevant information about your handler in a new `NginxConfig` object. It should ultimately indicate that this `NginxConfig` will be handled by your new request handler by the statement `setStatementType(statement_type)`, where `statement_type` is `NewRequestHandlerName_HANDLER`.

### Adding registration in server
Lastly, your handler type needs to be registered by the server. 

The server's `create_all_handlers` method will handle parsing through all of the configuration for you, so in adding a new request handler, you will need to update the `CreateHandlerByHandlerType` function in `server.cc`

```cpp
request_handler* server::CreateHandlerByHandlerType(NginxConfig config)
{
	if  (config.getStatementType() == NginxConfigStatement::NewRequestHandlerName_HANDLER)
	{
		<NewRequestHandlerName>request_handler* h = new <NewRequestHandlerName>request_handler;
		return (h->Init(config.getUrl(), config));
	}
	...
}
```
