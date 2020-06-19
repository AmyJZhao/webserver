#include "server_information.h"

ServerInformation::ServerInformation(std::map<std::string, request_handler*> path_map)
  : path_map_(path_map)
{
  for (std::pair<std::string, request_handler*> pair : path_map_) {
     request_count_by_url_[pair.first] = 0;
	}
 
  response_count_by_code_[200] = 0;
  response_count_by_code_[400] = 0;
  response_count_by_code_[404] = 0;
}

ServerInformation::~ServerInformation() 
{
  if (instance != NULL)
  {
    delete instance;
  }
}

void ServerInformation::incrementRequestCountByUrl(std::string url)
{
  const std::lock_guard<std::mutex> lock(server_mu);
  if (path_map_.find(url) != path_map_.end())
  {
    request_count_by_url_[url]++;
  } 
  else 
  {
    request_count_by_url_[url] = 1;
  }
}

void ServerInformation::incrementResponseCodeCountByCode(int code)
{
  const std::lock_guard<std::mutex> lock(server_mu);
  if (code == 200 || code == 400 || code == 404)
  {
    response_count_by_code_[code]++;
  }
}

int ServerInformation::getRequestCountForUrl(std::string url)
{
  const std::lock_guard<std::mutex> lock(server_mu);
  if (path_map_.find(url) != path_map_.end())
  {
    return request_count_by_url_[url];
  }
  else
  {
    return 0;
  }
}

int ServerInformation::getResponseCodeCountForCode(int code)
{
  const std::lock_guard<std::mutex> lock(server_mu);
  if (code == 200 || code == 400 || code == 404)
  {
    return response_count_by_code_[code];
  }
  else
  {
    return 0;
  }
}

ServerInformation *ServerInformation::instance = 0;