// proxy_request_handler.cc
// ~~~~~~~~~~~~~~~~~

#include "proxy_request_handler.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "response_helper.h"
#include "html_generator.h"
#include "logger.h"

// Constants for GZIP compression
#define MOD_GZIP_ZLIB_WINDOWSIZE 15
#define MOD_GZIP_ZLIB_CFACTOR 9
#define MOD_GZIP_ZLIB_BSIZE 8096

request_handler *ProxyRequestHandler::Init(const std::string &location_path, const NginxConfig &config)
{
  setLocationPath(location_path);
  config.getProxyVariables(host_, port_);
  response_helper resp_helper;
  default_response_ = resp_helper.make_response(500, "An error occured inside of proxy handler.");
  resp_helper.push_header(default_response_, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));
  resp_helper.push_header(default_response_, "Content-Length", std::to_string(default_response_.body_.length()));
  return this;
}

Response ProxyRequestHandler::handle_request(const Request &request)
{
  Logger *logger = Logger::getLogger();
  response_helper resp_helper;

  // initialize services
  boost::asio::io_service io_service_;
  tcp::socket socket_(io_service_);
  tcp::acceptor acceptor_(io_service_);
  tcp::resolver resolver_(io_service_);

  std::string request_url = request.getUri();

  std::string path = getLocationPath();
  int start_pos_to_erase = request_url.find(path);
  if (start_pos_to_erase == std::string::npos)
  {
    logger->logDebug("getLocationPath in ProxyHandler",  "The path for the handler is not found in the request URI");
    Logger *logger = Logger::getLogger();
    logger->logDebug("getLocationPath in ProxyHandler", "Response Code: 500");
    return default_response_;
  }

  request_url.erase(start_pos_to_erase, path.size());

  // In case path is like /proxy/, and we deleted the first slash
  if (request_url[0] != '/')
  {
    request_url.insert(0, "/");
  }

  std::string new_request = construct_request(request, request_url);

  try
  {
    send_proxy_request(new_request, resolver_, socket_);
  }
  catch (...)
  {
    return default_response_;
  }

  Response response = parse_response(acceptor_, socket_);

  // handling 302 responses
  int num_redirect = 0;
  bool redirect_status = true;
  while ((response.code_ == StatusCode::MovedTemporarily || response.code_ == StatusCode::MovedPermanently) && num_redirect < MAX_REDIRECT && redirect_status)
  {
    redirect_status = handle_redirect(request, response, resolver_, acceptor_, socket_);
    num_redirect++;
  }

  return response;
}

bool ProxyRequestHandler::handle_redirect(const Request &request, Response &response, tcp::resolver &resolver_, tcp::acceptor &acceptor_, tcp::socket &socket_)
{
  Logger *logger = Logger::getLogger();
  logger->logDebug("handle_redirect in ProxyHandler", "Response received 301 or 302 response");

  std::map<std::string, std::string>::iterator it;
  std::string redirected_url = response.headers_["location"];
  logger->logDebug("handle_redirect in ProxyHandler", "Redirect url is: " + redirected_url);
  std::string redirect_request;
  try
  {
    redirect_request = construct_request(request, redirected_url);
  }
  // Error occured trying to convert uri to url, https or different host -> return 302 directly
  catch (std::runtime_error &e)
  {
    logger->logError("handle_redirect in ProxyHandler", e.what());
    return false;
  }

  try
  {
    send_proxy_request(redirect_request, resolver_, socket_);
  }
  catch (...)
  {
    response = default_response_;
    return false;
  }
  Response redirect_response = parse_response(acceptor_, socket_);
  response = redirect_response;
  return true;
}

std::string ProxyRequestHandler::construct_request(const Request &request, std::string new_uri)
{
  if (!boost::starts_with(new_uri, "/"))
  {
    if (boost::starts_with(new_uri, "https://"))
      throw(std::runtime_error("HTTPS not supported"));
    else
    {
      int start_pos_to_delete = new_uri.find(host_);
      if (start_pos_to_delete == std::string::npos)
        throw(std::runtime_error("Different host, no redirect should occur"));
      new_uri.erase(0, start_pos_to_delete + host_.size());

      // Making sure that new uri is valid
      if (new_uri[0] != '/')
      {
        new_uri.insert(0, "/");
      }
    }
  }

  std::string result = "";
  // First line of request
  result.append(request.getMethod() + " " + new_uri + " ");

  // Sending 1.0 request so that the response won't be chunked
  result.append("HTTP/1.0");
  result.append("\r\n");

  // Headers
  std::string headers = "";

  bool have_host = false;
  for (auto const &pair : request.headers_)
  {
    std::string header_name = pair.first;
    std::string header_value = pair.second;
    boost::to_lower(header_name);
    if (header_name == "host")
    {
      have_host = true;
      header_value = host_;
    }
    std::string header = header_name + ": " + header_value + "\r\n";
    headers += header;
  }

  if (!have_host)
  {
    std::string host_string = "host: " + host_ + "\r\n";
    headers += host_string;
  }

  result.append(headers);

  // Trailing CRLF
  result.append("\r\n");
  result.append(request.getBody());

  return result;
}

void ProxyRequestHandler::send_proxy_request(std::string &new_request, tcp::resolver &resolver_, tcp::socket &socket_)
{
  tcp::resolver::iterator endpoint_iterator;
  if (port_ > 0)
    endpoint_iterator = resolver_.resolve(tcp::resolver::query(host_, std::to_string(port_)));
  else
    endpoint_iterator = resolver_.resolve(tcp::resolver::query(host_, "http"));

  tcp::resolver::iterator end;

  boost::system::error_code error = boost::asio::error::host_not_found;

  while (error && endpoint_iterator != end)
  {
    socket_.close();
    socket_.connect(*endpoint_iterator++, error);
  }
  if (error)
    throw boost::system::system_error(error);

  socket_.write_some(boost::asio::buffer(new_request, new_request.size() + 1), error);

  if (error)
    throw boost::system::system_error(error);
}

Response ProxyRequestHandler::parse_response(tcp::acceptor &acceptor_, tcp::socket &socket_)
{
  Logger *logger = Logger::getLogger();
  std::string body = "";
  int status_code = 500;
  boost::asio::streambuf buffer;
  size_t header_size = 0;

  // Read until the end of the headers
  header_size = boost::asio::read_until(socket_, buffer, "\r\n\r\n");

  boost::asio::streambuf::const_buffers_type bufs = buffer.data();
  std::string header_data(boost::asio::buffers_begin(bufs),
                          boost::asio::buffers_begin(bufs) + header_size);

  auto headers = parse_headers(header_data, status_code);

  // Check for content-length header
  size_t content_length = 0;
  auto content_length_it = headers.find("content-length");

  if (content_length_it != headers.end())
  {
    logger->logDebug("parse_response in ProxyHandler", "Response received has content length: " + content_length_it->second);
    content_length = std::stoi(content_length_it->second);
  }

  // Have additional data to read
  if (content_length > 0)
  {
    size_t additional_read = content_length - (buffer.size() - header_size);

    if (additional_read > 0)
    {
      size_t read_bytes;
      read_bytes = boost::asio::read(socket_, buffer, boost::asio::transfer_exactly(additional_read));
    }

    boost::asio::streambuf::const_buffers_type body_buffers = buffer.data();

    std::string body_buffer(boost::asio::buffers_begin(body_buffers) + header_size,
                            boost::asio::buffers_begin(body_buffers) + buffer.size());

    body = body_buffer;
  }
  else
  {
    auto connection_it = headers.find("connection");

    if (connection_it != headers.end() && connection_it->second == "close")
    {
      logger->logDebug("parse_response in ProxyHandler", "Response received has no content length but has connection: close");
      boost::system::error_code error;
      while (boost::asio::read(socket_, buffer,
                               boost::asio::transfer_at_least(1), error))
      {
      }
      if (error != boost::asio::error::eof)
        throw boost::system::system_error(error);
      boost::asio::streambuf::const_buffers_type body_buffers = buffer.data();

      std::string body_buffer(boost::asio::buffers_begin(body_buffers) + header_size,
                              boost::asio::buffers_begin(body_buffers) + buffer.size());

      body = body_buffer;
    }
  }

  // Replaces hrefs in body if body is html
  auto href_it = headers.find("content-type");

  // 301 responses send back the correct headers but no body -> check content_length
  if (href_it != headers.end() && boost::starts_with(href_it->second, "text/html") && content_length > 0)
  {
    logger->logDebug("parse_response in ProxyHandler", "File is html, replacing links...");

    // Check if html file  is compressed
    bool compressed = false;
    auto compressed_it = headers.find("content-encoding");

    if (compressed_it != headers.end() && compressed_it->second == "gzip")
    {
      logger->logDebug("parse_response in ProxyHandler", "Response received is gzipped compressed");
      std::string decompressed_body = gzip_decompress(body);
      decompressed_body = html_generator::replace_all(decompressed_body, path_);
      body = gzip_compress(decompressed_body);
    }
    else
    {
      body = html_generator::replace_all(body, path_);
    }
  }

  response_helper resp_helper;

  logger->logDebug("parse_response in ProxyHandler", "Client response body has size: " + std::to_string(body.size()));
  Response response = resp_helper.make_response(status_code, body);

  // Push headers into response

  for (auto header = headers.begin(); header != headers.end(); ++header)
  {
    if (header->first == "content-length")
    {
      resp_helper.push_header(response, header->first, std::to_string(body.size()));
    }
    else
      resp_helper.push_header(response, header->first, header->second);
  }

  logger->logDebug("parse_response in ProxyHandler", "Response Code: " + status_code);
  return response;
}

std::map<std::string, std::string> ProxyRequestHandler::parse_headers(std::string header_data, int &status_code)
{
  std::vector<std::string> header_lines = split_string(header_data, "\r\n");
  std::map<std::string, std::string> headers;
  for (auto it = header_lines.begin(); it != header_lines.end(); ++it)
  {
    // First line is the response code, split by space then grab 2nd element for status code
    if (it == header_lines.begin())
    {
      std::stringstream http_line(*it);
      std::string segment;
      std::vector<std::string> seglist;

      while (std::getline(http_line, segment, ' '))
      {
        seglist.push_back(segment);
      }

      try
      {
        status_code = std::stoi(seglist[1]);
      }
      catch (...)
      {
        continue;
      }
    }
    else
    {
      if (*it == "\r\n" || *it == "\r" || *it == "\n" || *it == "")
        continue;
      std::vector<std::string> header_values = split_string(*it, ": ");
      std::string header_name = header_values[0];

      // converting header name to lowercase so that the other parser can find content-length
      // HTTP headers are case insensitive
      boost::to_lower(header_name);
      headers[header_name] = header_values[1];
    }
  }

  return headers;
}

std::vector<std::string> ProxyRequestHandler::split_string(std::string str, std::string delimiter)
{
  size_t pos = 0;

  std::string token;
  std::vector<std::string> vec;

  while ((pos = str.find(delimiter)) != std::string::npos)
  {
    token = str.substr(0, pos);
    str.erase(0, pos + delimiter.length());
    vec.push_back(token);
  }

  vec.push_back(str);

  return vec;
}

/** Compress a STL string using zlib with given compression level and return
  * the binary data. */
std::string ProxyRequestHandler::gzip_compress(const std::string &data)
{
  z_stream zs; // z_stream is zlib's control structure
  memset(&zs, 0, sizeof(zs));

  if (deflateInit2(&zs,
                   Z_BEST_COMPRESSION,
                   Z_DEFLATED,
                   MOD_GZIP_ZLIB_WINDOWSIZE + 16,
                   MOD_GZIP_ZLIB_CFACTOR,
                   Z_DEFAULT_STRATEGY) != Z_OK)
    throw(std::runtime_error("deflateInit failed while compressing."));

  zs.next_in = (Bytef *)data.data();
  zs.avail_in = data.size(); // set the z_stream's input

  int ret;
  char outbuffer[32768];
  std::string outstring;

  // retrieve the compressed bytes blockwise
  do
  {
    zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = deflate(&zs, Z_FINISH);

    if (outstring.size() < zs.total_out)
    {
      // append the block to the output string
      outstring.append(outbuffer,
                       zs.total_out - outstring.size());
    }
  } while (ret == Z_OK);

  deflateEnd(&zs);

  if (ret != Z_STREAM_END)
  { // an error occurred that was not EOF
    std::ostringstream oss;
    oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
    throw(std::runtime_error(oss.str()));
  }

  return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
std::string ProxyRequestHandler::gzip_decompress(const std::string &data)
{
  z_stream zs; // z_stream is zlib's control structure
  memset(&zs, 0, sizeof(zs));

  if (inflateInit2(&zs, MOD_GZIP_ZLIB_WINDOWSIZE + 16) != Z_OK)
    throw(std::runtime_error("inflateInit failed while decompressing."));

  zs.next_in = (Bytef *)data.data();
  zs.avail_in = data.size();

  int ret;
  char outbuffer[32768];
  std::string outstring;

  // get the decompressed bytes blockwise using repeated calls to inflate
  do
  {
    zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = inflate(&zs, 0);

    if (outstring.size() < zs.total_out)
    {
      outstring.append(outbuffer,
                       zs.total_out - outstring.size());
    }

  } while (ret == Z_OK);

  inflateEnd(&zs);

  if (ret != Z_STREAM_END)
  { // an error occurred that was not EOF
    std::ostringstream oss;
    oss << "Exception during zlib decompression: (" << ret << ") "
        << zs.msg;
    throw(std::runtime_error(oss.str()));
  }

  return outstring;
}
