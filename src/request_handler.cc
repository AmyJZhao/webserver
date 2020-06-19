#include "request_handler.h"

std::string request_handler::getLocationPath() {
  return path_;
}

void request_handler::setLocationPath(std::string location_path) {
  path_ = location_path;
}