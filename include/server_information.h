#ifndef SERVER_INFORMATION_H
#define SERVER_INFORMATION_H

#include <map>
#include <mutex>
#include <string>

class request_handler;

class ServerInformation {
  public:
    ServerInformation(std::map<std::string, request_handler*> path_map);
    ~ServerInformation();
    static ServerInformation *instance; // the one, single instance
    void incrementRequestCountByUrl(std::string url);
    void incrementResponseCodeCountByCode(int code);
    int getRequestCountForUrl(std::string url);
    int getResponseCodeCountForCode(int code);
    static ServerInformation *setInstance(std::map<std::string, request_handler*> path_map)
    {
      if (ServerInformation::instance == 0)
      {
        ServerInformation::instance = new ServerInformation(path_map);
      }
      return ServerInformation::instance;
    }
    static ServerInformation *getInstance()
    {
      return ServerInformation::instance;
    }
    std::map<std::string, request_handler*> path_map_;

  private:
    std::map<std::string, int> request_count_by_url_;
    std::map<int, int> response_count_by_code_;
    std::mutex server_mu; 
};

#endif
