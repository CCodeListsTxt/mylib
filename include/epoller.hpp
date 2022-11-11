#pragma once
#include <vector>
#include <sys/epoll.h>
#include "socket.hpp"
#include "thread_poll.hpp"

using namespace std;

namespace mylib
{
    class Epoller : public Noncopyable
    {
    public:
        enum class EpollEvent
        {
            EPOLL_READABLE,
            EPOLL_WRITEABLE
        };

    public:
        Epoller(size_t thread_count = 8, size_t task_queue_size = 8);
        ~Epoller() = default;

    public:
        void add(Socket *sock, EpollEvent epollEvent);
        void del(Socket *sock);
        void run();
        void stop() { m_is_running = false; }

    private:
        bool m_is_running = true;
        int m_epoll_fd;
        ThreadPool m_thread_pool;
    };
}