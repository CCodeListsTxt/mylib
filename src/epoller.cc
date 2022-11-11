#include "../include/epoller.hpp"

using namespace std;

namespace mylib
{
    Epoller::Epoller(size_t thread_count, size_t task_queue_size)
        : m_thread_pool(thread_count, task_queue_size)
    {
        m_epoll_fd = epoll_create(10);
        if (m_epoll_fd < 0)
            __throw_runtime_error("Epoller Init Error");
    }

    void Epoller::add(Socket *sock, EpollEvent epollEvent)
    {
        epoll_event event;
        switch (epollEvent)
        {
        case Epoller::EpollEvent::EPOLL_READABLE:
            event.events = EPOLLIN | EPOLLET;
            break;
        case Epoller::EpollEvent::EPOLL_WRITEABLE:
            event.events = EPOLLOUT | EPOLLET;
            break;
        }
        event.data.ptr = (void *)sock;
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, sock->getFd(), &event) != 0)
            __throw_runtime_error("Epoller Add Error");
    }

    void Epoller::del(Socket *sock)
    {
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, sock->getFd(), nullptr) != 0)
            __throw_runtime_error("Epoller Del Error");
    }

    void Epoller::run()
    {
        size_t buf_size = m_thread_pool.getThreadCount();
        vector<epoll_event> events(buf_size);
        while (m_is_running)
        {
            int count = epoll_wait(m_is_running, events.data(), buf_size, -1);
            if (count < 0)
                __throw_runtime_error("Epoller Run Error");
            for (int i = 0; i < count; ++i)
            {
                Socket *sock = (Socket *)events[i].data.ptr;
                m_thread_pool.addTask([sock]
                                      { sock->getReadCallback()(sock); });
            }
        }
    }
}