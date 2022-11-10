#include "../include/http.hpp"

namespace mylib
{
    static const char CR = '\r';
    static const char LF = '\n';
    static const string CRLF = "\r\n";

    string HttpContext::versionToString(HttpContext::HttpVersion version)
    {
        // using enum HttpContext::HttpVersion;
        static map<HttpVersion, string> version_str_map = {
            {HttpContext::HttpVersion::UNKNOWN, "UNKNOWN"},
            {HttpContext::HttpVersion::HTTP10, "HTTP/1.0"},
            {HttpContext::HttpVersion::HTTP11, "HTTP/1.1"}};
        return version_str_map[version];
    }

    HttpContext::HttpVersion HttpContext::stringToVersion(const string &str)
    {
        // using enum HttpContext::HttpVersion;
        static map<string, HttpVersion> str_version_map = {
            {"HTTP/1.0", HttpContext::HttpVersion::HTTP10},
            {"HTTP/1.1", HttpContext::HttpVersion::HTTP11}};
        // map较小，个人认为这种方式比注释的代码更方便阅读，且效率影响不会很大
        // try{ return str_version_map.at(toUpper(str)); }
        // catch(...) { return UNKNOWN; }
        string upper_str = toUpper(str);
        if (str_version_map.contains(upper_str))
            return str_version_map[upper_str];
        return HttpContext::HttpVersion::UNKNOWN;
    }

    string HttpContext::getHeader(const string &key) const
    {
        if (m_headers.contains(key))
            return m_headers.at(key);
        return "";
    }

    string HttpContext::getHeaders() const
    {
        string ret;
        for (auto &[key, value] : m_headers)
            ret += key + " : " + value + CRLF;
        return ret;
    }

    string HttpContext::toString() const
    {
        string ret;
        ret += getLine();
        ret += getHeaders();
        ret += CRLF;
        ret += getBody();
        return ret;
    }

    // ---------------------------------------------------------------
    // ---------------------------------------------------------------

    string HttpRequest::getLine() const
    {
        string ret;
        ret += methodToString(m_method) + " ";
        ret += m_url + " ";
        ret += versionToString(m_version) + CRLF;
        return ret;
    }

    string HttpRequest::methodToString(HttpRequest::RequestMethod method)
    {
        // using enum HttpRequest::RequestMethod;
        static map<RequestMethod, string> method_str_map = {
            {HttpRequest::RequestMethod::UNKNOWN, "UNKNOWN"},
            {HttpRequest::RequestMethod::GET, "GET"},
            {HttpRequest::RequestMethod::POST, "POST"},
            {HttpRequest::RequestMethod::HEAD, "HEAD"},
            {HttpRequest::RequestMethod::PUT, "PUT"},
            {HttpRequest::RequestMethod::DELETE, "DELETE"}};
        return method_str_map[method];
    }

    HttpRequest::RequestMethod HttpRequest::stringToMethod(const string &str)
    {
        // using enum HttpRequest::RequestMethod;
        static map<string, RequestMethod> str_method_map = {
            {"GET", HttpRequest::RequestMethod::GET},
            {"POST", HttpRequest::RequestMethod::POST},
            {"HEAD", HttpRequest::RequestMethod::HEAD},
            {"PUT", HttpRequest::RequestMethod::PUT},
            {"DELETE", HttpRequest::RequestMethod::DELETE}};
        string upper_str = toUpper(str);
        if (str_method_map.contains(upper_str))
            return str_method_map[str];
        return HttpRequest::RequestMethod::UNKNOWN;
    }

    // ---------------------------------------------------------------
    // ---------------------------------------------------------------

    void HttpResponse::setStatus(ResponseStatus status)
    {
        m_status = status;
        m_status_msg = statusToString(status);
    }

    string HttpResponse::getLine() const
    {
        string ret;
        ret += versionToString(m_version) + " ";
        ret += statusToString(m_status) + " ";
        ret += m_status_msg + CRLF;
        return ret;
    }

    string HttpResponse::statusToString(HttpResponse::ResponseStatus status)
    {
        // using enum HttpResponse::ResponseStatus;
        static map<ResponseStatus, string> status_str_map = {
            {HttpResponse::ResponseStatus::UNKNOWN, "UNKNOWN"},
            {HttpResponse::ResponseStatus::_200, "200"},
            {HttpResponse::ResponseStatus::_301, "301"},
            {HttpResponse::ResponseStatus::_404, "404"}};
        return status_str_map[status];
    }

    HttpResponse::ResponseStatus HttpResponse::stringToStatus(const string &str)
    {
        // using enum HttpResponse::ResponseStatus;
        static map<string, ResponseStatus> str_status_map = {
            {"200", HttpResponse::ResponseStatus::_200},
            {"301", HttpResponse::ResponseStatus::_301},
            {"404", HttpResponse::ResponseStatus::_404}};
        if (str_status_map.contains(str))
            return str_status_map[str];
        return HttpResponse::ResponseStatus::UNKNOWN;
    }

    // ---------------------------------------------------------------
    // ---------------------------------------------------------------

    void HttpParser::parseMethod(HttpRequest &request)
    {
        size_t pos = findFirstOf(m_str, m_index, ' ');
        if (pos == string::npos)
            throw new invalid_argument(ERROR_HTTP_REQUEST_LINE_STRING_INVALID);
        string method_str = trim(toUpper(m_str.substr(m_index, pos - m_index)));
        request.setMethod(HttpRequest::stringToMethod(method_str));
        m_index = pos + 1;
    }

    void HttpParser::parseUrl(HttpRequest &request)
    {
        size_t pos = findFirstOf(m_str, m_index, ' ');
        if (pos == string::npos)
            throw new invalid_argument(ERROR_HTTP_REQUEST_LINE_STRING_INVALID);
        request.setUrl(trim(m_str.substr(m_index, pos - m_index)));
        m_index = pos + 1;
    }

    void HttpParser::parseVersion(HttpRequest &request)
    {
        size_t pos = findFirstOf(m_str, m_index, CR);
        if (pos == string::npos || m_str[pos + 1] != LF)
            throw new invalid_argument(ERROR_HTTP_REQUEST_LINE_STRING_INVALID);
        string version_str = trim(toUpper(m_str.substr(m_index, pos - m_index)));
        request.setVersion(HttpRequest::stringToVersion(version_str));
        m_index = pos + 2;
    }

    void HttpParser::parseLine(HttpRequest &request)
    {
        parseMethod(request);
        parseUrl(request);
        parseVersion(request);
    }

    void HttpParser::parseVersion(HttpResponse &response)
    {
        size_t pos = findFirstOf(m_str, m_index, ' ');
        if (pos == string::npos)
            throw new invalid_argument(ERROR_HTTP_RESPONSE_LINE_STARING_INVALID);
        string version_str = trim(toUpper(m_str.substr(m_index, pos - m_index)));
        response.setVersion(HttpResponse::stringToVersion(version_str));
        m_index = pos + 1;
    }

    void HttpParser::parseStatus(HttpResponse &response)
    {
        size_t pos = findFirstOf(m_str, m_index, ' ');
        if (pos == string::npos)
            throw new invalid_argument(ERROR_HTTP_RESPONSE_LINE_STARING_INVALID);
        string status_str = trim(toUpper(m_str.substr(m_index, pos - m_index)));
        response.setStatus(HttpResponse::stringToStatus(status_str));
        m_index = pos + 1;
    }

    void HttpParser::parseStatusMsg(HttpResponse &response)
    {
        size_t pos = findFirstOf(m_str, m_index, CR);
        if (pos == string::npos || m_str[pos + 1] != LF)
            throw new invalid_argument(ERROR_HTTP_RESPONSE_LINE_STARING_INVALID);
        string status_msg = trim(m_str.substr(m_index, pos - m_index));
        response.setStatusMsg(status_msg);
        m_index = pos + 2;
    }

    void HttpParser::parseLine(HttpResponse &response)
    {
        parseVersion(response);
        parseStatus(response);
        parseStatusMsg(response);
    }

    void HttpParser::parseHeaders(HttpContext &context)
    {
        // 没有遇到空行
        while (!isCRLF())
        {
            // 键
            size_t pos = findFirstOf(m_str, m_index, ':');
            if (pos == string::npos)
                throw new invalid_argument(ERROR_HTTP_HEADERS_STRING_INVALID);
            string key = trim(m_str.substr(m_index, pos - m_index));
            m_index = pos + 1;

            // 值
            pos = findFirstOf(m_str, m_index, CR);
            if (m_str[pos + 1] != LF)
                throw new invalid_argument(ERROR_HTTP_HEADERS_STRING_INVALID);
            string value = trim(m_str.substr(m_index, pos - m_index));
            m_index = pos + 2;

            // 添加
            context.setHeader(key, value);
        }
        m_index += 2;
    }

    void HttpParser::reset(const string &str)
    {
        m_str = str;
        m_index = 0;
    }

    bool HttpParser::isCRLF()
    {
        return m_str[m_index] == CR && m_str[m_index + 1] == LF;
    }

}