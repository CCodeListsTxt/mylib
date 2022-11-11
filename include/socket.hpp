#pragma once
#include <functional>
#include <exception>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "./utility.hpp"

using namespace std;

namespace mylib
{
    class Socket : public Noncopyable
    {
        friend class Acceptor;

    public:
        using CallbackType = function<void(Socket *)>;

    public:
        Socket();
        virtual ~Socket() {}

    public:
        void setAddress(const string &ip_v4);
        string getAddress() const;

        void setPort(uint16_t port) { m_sockad.sin_port = port; }
        uint16_t getPort() const { return m_sockad.sin_port; }

        void setReadCallback(CallbackType cb) { m_read_cb = cb; }
        CallbackType getReadCallback() const { return m_read_cb; }

        int getFd() const { return m_fd; }

    protected:
        int m_fd;
        CallbackType m_read_cb;
        struct sockaddr_in m_sockad;
    };

    // **********************************************************
    // **********************************************************

    class Acceptor : public Socket
    {
    public:
        Acceptor();
        ~Acceptor() {}

    public:
        void listen() const;
        void accept(Socket &sock) const;
    };

    // **********************************************************
    // **********************************************************

    class Connection : public Socket
    {
    public:
        Connection() : Socket() {}
        ~Connection() {}

    public:
        string recv(size_t size = 0) const;
        void send(const string &buf) const;
    };
}