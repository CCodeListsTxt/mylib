#include "../include/thread_poll.hpp"

namespace mylib
{
    ThreadPool::ThreadPool(size_t thread_count, size_t task_queue_size)
    {
        m_task_max_count = std::max(task_queue_size, MIN_TASK_COUNT);
        thread_count = std::max(thread_count, MIN_THREAD_COUNT);
        for (int i = 0; i < thread_count; ++i)
            m_threads.emplace_back(&ThreadPool::threadCycle, this);
    }

    ThreadPool::~ThreadPool()
    {
        m_is_running = false;
        for (int i = 0; i < m_threads.size(); ++i)
            if (m_threads[i].joinable())
                m_threads[i].join();
    }

    void ThreadPool::threadCycle()
    {

        while (m_is_running)
        {
            while (m_tasks.empty())
            {
                unique_lock<mutex> lock(m_mutex_task_not_empty);
                m_task_not_empty.wait(lock);
            }
            TaskType task;
            {
                lock_guard<mutex> lock(m_task_mutex);
                task = m_tasks.front();
                m_tasks.pop();
                m_task_not_full.notify_one();
            }
            task();
        }
    }

    void ThreadPool::addTask(TaskType task)
    {
        while (m_tasks.size() >= m_task_max_count)
        {
            unique_lock<mutex> lock(m_mutex_task_not_full);
            m_task_not_full.wait(lock);
        }
        lock_guard<mutex> lock(m_task_mutex);
        m_tasks.push(task);
        m_task_not_empty.notify_one();
    }
}