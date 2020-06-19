#include "gtest/gtest.h"
#include "response.h"
#include "response_helper.h"
#include <boost/asio.hpp>
#include <iostream>

class ResponseTest : public ::testing::Test {
    protected:
        Response            r;
        response_helper     resp_helper;
};

TEST_F(ResponseTest, ResponseConstructionTest) {
    r = resp_helper.make_response(400, "Bad Request");
    resp_helper.push_header(r, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));

    std::string exp_response = 
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Bad Request";
    EXPECT_TRUE(resp_helper.to_string(r) == exp_response);
}

TEST_F(ResponseTest, HeadersTest) {
    resp_helper.push_header(r, "Content-Type", resp_helper.get_mime_type(MimeType::TEXT_PLAIN));
    resp_helper.push_header(r, "Connection", "Keep-Alive");

    EXPECT_EQ(resp_helper.get_header(r, "Connection"), "Keep-Alive");
    EXPECT_EQ(resp_helper.get_header(r, "Cookie"), "");

    std::string exp_headers =
        "Connection: Keep-Alive\r\n"
        "Content-Type: text/plain\r\n";
    EXPECT_TRUE(resp_helper.headers_to_string(r) == exp_headers);
}

TEST_F(ResponseTest, GetMimeTypeTest) {
    EXPECT_EQ(resp_helper.get_mime_type(MimeType::TEXT_PLAIN), "text/plain");
    EXPECT_EQ(resp_helper.get_mime_type(MimeType::TEXT_HTML), "text/html");
    EXPECT_EQ(resp_helper.get_mime_type(MimeType::IMAGE_JPEG), "image/jpeg");
    EXPECT_EQ(resp_helper.get_mime_type(MimeType::APPLICATION_ZIP), "application/zip");
    EXPECT_EQ(resp_helper.get_mime_type(MimeType::TEXT_CSS), "text/css");
    EXPECT_EQ(resp_helper.get_mime_type(MimeType::TEXT_JAVASCRIPT), "text/javascript");
}
