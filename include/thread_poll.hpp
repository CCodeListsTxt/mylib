#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <any>
#include <condition_variable>
#include <queue>
#include "utility.hpp"

using namespace std;

namespace mylib
{
    class ThreadPool : public Noncopyable
    {
    public:
        using TaskType = function<void()>;

    public:
        ThreadPool(size_t thread_count = 8, size_t task_queue_size = 8);
        ~ThreadPool();

    public:
        void addTask(TaskType task);
        size_t getThreadCount() const { return m_threads.size(); }

    private:
        // 线程运行函数
        void threadCycle();

    private:
        constexpr static size_t MIN_THREAD_COUNT = 8;
        constexpr static size_t MIN_TASK_COUNT = 8;

    private:
        bool m_is_running = true;
        size_t m_task_max_count;
        mutex m_threads_mutex;
        vector<thread> m_threads;
        mutex m_task_mutex;
        queue<TaskType> m_tasks;
        mutex m_mutex_task_not_empty;
        condition_variable m_task_not_empty;
        mutex m_mutex_task_not_full;
        condition_variable m_task_not_full;
    };
}