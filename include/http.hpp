#pragma once
#include <map>
#include "utility.hpp"

#define ERROR_HTTP_REQUEST_LINE_STRING_INVALID ("HTTP请求行错误。" + to_string(__LINE__) + "行\n" + m_str)
#define ERROR_HTTP_RESPONSE_LINE_STARING_INVALID ("HTTP响应行错误。" + to_string(__LINE__) + "行\n" + m_str)
#define ERROR_HTTP_HEADERS_STRING_INVALID ("HTTP报文头错误。" + to_string(__LINE__) + "行\n" + m_str)

#define CONTENT_TYPE "Content-Type"
#define CONTENT_LANGUAGE "Content-Language"
#define CONTENT_LENGTH "Content-Length"
#define CONTENT_LOCATION "Content-Location"
#define CONTENT_TYPE "Content-Type"
#define COOKIE "Cookie"

namespace mylib
{
    using namespace std;

    class HttpContext
    {
    public:
        enum class HttpVersion
        {
            UNKNOWN,
            HTTP10,
            HTTP11
        };

    public:
        HttpContext() = default;
        virtual ~HttpContext() = default;

    public:
        void setVersion(HttpVersion version) { m_version = version; }
        HttpVersion getVersion() const { return m_version; }

        void setHeader(const string &key, const string &value) { m_headers.emplace(key, value); }
        string getHeader(const string &key) const;
        string getHeaders() const;

        void setBody(const string &body) { m_body = body; }
        string getBody() const { return m_body; }

        virtual string getLine() const = 0;
        virtual string toString() const;

        static string versionToString(HttpVersion version);
        static HttpVersion stringToVersion(const string &str);

        // 常用报头
    public:
        string getContentType() const { return getHeader(CONTENT_TYPE); }
        void setContentType(const string &value) { setHeader(CONTENT_TYPE, value); }

        string getContentLanguage() const { return getHeader(CONTENT_LANGUAGE); }
        void setContentLanguage(const string &value) { setHeader(CONTENT_LANGUAGE, value); }

        string getContentLength() const { return getHeader(CONTENT_LENGTH); }
        void setContentLength(const string &value) { setHeader(CONTENT_LENGTH, value); }

        string getContentType() const { return getHeader(CONTENT_TYPE); }
        void setContentType(const string &value) { setHeader(CONTENT_TYPE, value); }

        string getCookie() const { return getHeader(COOKIE); }
        void setCookie(const string &value) { setHeader(COOKIE, value); }

    protected:
        HttpVersion m_version = HttpVersion::UNKNOWN; // 版本
        map<string, string> m_headers;                // 报头
        string m_body;                                // 正文
    };

    // ---------------------------------------------------------------
    // ---------------------------------------------------------------

    class HttpRequest : public HttpContext
    {
    public:
        enum class RequestMethod
        {
            UNKNOWN,
            GET,
            POST,
            HEAD,
            PUT,
            DELETE
        };

    public:
        HttpRequest() = default;
        ~HttpRequest() = default;

    public:
        void setMethod(RequestMethod method) { m_method = method; }
        RequestMethod getMethod() const { return m_method; }
        void setUrl(const string &url) { m_url = url; }
        string getUrl() const { return m_url; }

        string getLine() const override;

        static string methodToString(RequestMethod method);
        static RequestMethod stringToMethod(const string &str);

    private:
        RequestMethod m_method = RequestMethod::UNKNOWN; // 方法
        string m_url;                                    // url
    };

    // ---------------------------------------------------------------
    // ---------------------------------------------------------------

    class HttpResponse : public HttpContext
    {
    public:
        enum class ResponseStatus
        {
            UNKNOWN,
            _200,
            _301,
            _404
        };

    public:
        HttpResponse() = default;
        ~HttpResponse() = default;

    public:
        void setStatus(ResponseStatus status);
        ResponseStatus getStatus() const { return m_status; }

        void setStatusMsg(const string &msg) { m_status_msg = msg; }
        string getStatusMsg() const { return m_status_msg; }

        string getLine() const override;

        static string statusToString(ResponseStatus status);
        static ResponseStatus stringToStatus(const string &str);

    private:
        ResponseStatus m_status = ResponseStatus::UNKNOWN; // 状态码
        string m_status_msg;                               // 状态信息
    };

    // ---------------------------------------------------------------
    // ---------------------------------------------------------------

    class HttpParser
    {
    public:
        HttpParser() = default;
        HttpParser(const string &str) : m_str(str), m_index(0) {}
        ~HttpParser() = default;

    public:
        void parseLine(HttpRequest &request);
        void parseLine(HttpResponse &response);
        void parseHeaders(HttpContext &context);
        // 重置状态
        void reset() { m_index = 0; }
        void reset(const string &str);
        // 获取当前下标
        int getCurIndex() const { return m_index; }

    private:
        void parseMethod(HttpRequest &request);
        void parseUrl(HttpRequest &request);
        void parseVersion(HttpRequest &request);

        void parseVersion(HttpResponse &response);
        void parseStatus(HttpResponse &response);
        void parseStatusMsg(HttpResponse &response);

        bool isCRLF();

    private:
        string m_str;
        size_t m_index;
    };
}