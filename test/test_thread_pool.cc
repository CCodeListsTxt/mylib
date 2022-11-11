#include "../include/thread_poll.hpp"

using namespace mylib;

void task()
{
    int i = 0;
    while (true)
        i += 1;
}

int main()
{

    ThreadPool pool;

    for (int i = 0; i < 16; ++i)
    {
        cout << "添加第" + to_string(i + 1) + "个任务\n";
        pool.addTask(task);
        this_thread::sleep_for(chrono::seconds(1));
    }

    this_thread::sleep_for(chrono::seconds(100));

    return 0;
}