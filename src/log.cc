#include "../include/log.hpp"

namespace mylib
{
    // 各等级日志对应字符串
    static const string LOG_LEVEL_STR[] = {
        "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    // 各等级日志对应输出到控制台的格式化字符串
    static const string LOG_LEVEL_FORMAT_STR[] = {
        "(%s)\033[;32m%-10s : \033[0m%s\n", // 绿色
        "(%s)\033[;34m%-10s : \033[0m%s\n", // 蓝色
        "(%s)\033[;33m%-10s : \033[0m%s\n", // 黄色
        "(%s)\033[;31m%-10s : \033[0m%s\n", // 红色
        "(%s)\033[;30m%-10s : \033[0m%s\n"  // 黑色
    };

    Logger::~Logger()
    {
        for (int i = 0; i < 5; ++i)
            m_file_out[i].flush();
    }

    void Logger::setLogFilename(const string &file_name)
    {
        for (int i = 0; i < 5; ++i)
        {
            lock_guard<mutex> lock(m_file_mutex[i]);
            m_file_out[i].flush();
            string log_file_path = LOG_ROOT_PATH + file_name + "_" + LOG_LEVEL_STR[i]; // 合成后的日志文件路径
            if (!filesystem::exists(log_file_path.data()))
            {
                // 创建多层目录
                int index = log_file_path.find_last_of('/');
                if (index != string::npos)
                    filesystem::create_directories(log_file_path.substr(0, index));
            }
            m_file_out[i].open(log_file_path, ios::app);
        }
    }

    void Logger::log(LogLevel level, const string &log_content)
    {
        if (!m_file_out[level].is_open())
            throw new std::runtime_error("日志文件尚未打开");
        string curr_time = localDateTimeString(); // 当前日期字符串

        // 输出到控制台时无需考虑加锁
        if (m_log_to_console && level >= m_log_to_console_min_level)
            printf(LOG_LEVEL_FORMAT_STR[level].data(), curr_time.data(), LOG_LEVEL_STR[level].data(), log_content.data());

        // 输出到文件
        lock_guard<mutex> lock(m_file_mutex[level]);
        m_file_out[level] << curr_time << " " << LOG_LEVEL_STR[level] << " : " << log_content << "\n";
    }

    void Logger::setLogToConsoleMinLevel(LogLevel min_level)
    {
        assert(min_level >= DEBUG && min_level <= CRITICAL);
        lock_guard<mutex> lock0(m_file_mutex[0]);
        lock_guard<mutex> lock1(m_file_mutex[1]);
        lock_guard<mutex> lock2(m_file_mutex[2]);
        lock_guard<mutex> lock3(m_file_mutex[3]);
        lock_guard<mutex> lock4(m_file_mutex[4]);
        m_log_to_console_min_level = min_level;
    }

    shared_ptr<Logger> Logger::instance()
    {
        if (single_logger == nullptr)
            single_logger = make_shared<Logger>();
        return single_logger;
    }
}