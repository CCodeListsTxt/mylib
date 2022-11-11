#include "../include/socket.hpp"

namespace mylib
{
    Socket::Socket()
    {
        m_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_fd < 0)
            __throw_runtime_error("Socket init error");
    }

    void Socket::setAddress(const string &ip_v4)
    {
        if (inet_aton(ip_v4.data(), &m_sockad.sin_addr) <= 0)
            __throw_invalid_argument("Ipv4 invalid");
    }

    string Socket::getAddress() const
    {
        char *buf = inet_ntoa(m_sockad.sin_addr);
        return string(buf);
    }

    // **********************************************************
    // **********************************************************

    Acceptor::Acceptor() : Socket()
    {
        int opt = 1;
        // 设置端口复用
        if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
            __throw_runtime_error("Acceptor init error");
    }

    void Acceptor::listen() const
    {
        if (::listen(m_fd, 10) < 0)
            __throw_runtime_error("Acceptor listen error");
    }

    void Acceptor::accept(Socket &sock) const
    {
        socklen_t sockad_len = sizeof(sock.m_sockad);
        sock.m_fd = ::accept(m_fd, (sockaddr *)&sock.m_sockad, &sockad_len);
        if (sock.m_fd < 0)
            __throw_runtime_error("Acceptor accept error");
    }

    string Connection::recv(size_t size) const
    {
        char buf[4096];
        if (size == 0)
        {
            ssize_t len = ::recv(m_fd, buf, sizeof(buf) - 1, 0);
            if (len <= 0)
                __throw_runtime_error("Connection recv error");
            buf[len] = 0;
            return string(buf);
        }
        string ret;
        while (size > 0)
        {
            size_t len = ::recv(m_fd, buf, sizeof(buf) - 1, 0);
            if (len <= 0)
                __throw_runtime_error("Connection recv error");
            buf[len] = 0;
            ret += buf;
            size -= len;
        }
        return ret;
    }

    void Connection::send(const string &buf) const
    {
        if (::send(m_fd, buf.data(), buf.size(), 0) < 0)
            __throw_runtime_error("Connection send error");
    }
}