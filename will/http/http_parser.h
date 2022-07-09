#ifndef __HTTP_PARSER__
#define __HTTP_PARSER__

#include <memory>
#include "http11_parser.h"
#include "httpclient_parser.h"
#include "http.h"
#include "../log.h"

namespace will {
namespace http {

class HttpRequestParser {
public:
    typedef std::shared_ptr<HttpRequestParser> ptr;
    HttpRequestParser();
    size_t execute(char* data, size_t len);
    int isFinished();
    int hasError();
    HttpRequest::ptr getData() const { return m_data; }
    void setError(int v) { m_error = v; }
    uint64_t getContentLength();
    const http_parser& getParser() const { return m_parser; }
public:
    static uint64_t GetHttpRequestBufferSize();
    static uint64_t GetHttpRequestMaxBodySize();
private:
    http_parser m_parser;
    HttpRequest::ptr m_data;
    int m_error;
};

class HttpResponseParser {
public:
    typedef std::shared_ptr<HttpResponseParser> ptr;
    HttpResponseParser();
    size_t execute(char* data, size_t len, bool chunck);
    int isFinished();
    int hasError();
    HttpResponse::ptr getData() const { return m_data; }
    void setError(int v) { m_error = v; }
    uint64_t getContentLength();
    const httpclient_parser& getParser() const { return m_parser; }

public:
    static uint64_t GetHttpResponseBufferSize();
    static uint64_t GetHttpResponseMaxBodySize();
private:
    httpclient_parser m_parser;
    HttpResponse::ptr m_data;
    int m_error;
};

}
}

#endif