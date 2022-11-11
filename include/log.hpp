#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <exception>
#include <filesystem>
#include <fcntl.h>
#include <sys/stat.h>
#include <cassert>
#include "./utility.hpp"

using namespace std;

namespace mylib
{
    class Logger : public Noncopyable
    {
    public:
        // 日志等级
        enum LogLevel
        {
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            CRITICAL
        };

    public:
        Logger() = default;
        ~Logger();

    public:
        // 写日志
        void logDebug(const string &log_content) { log(DEBUG, log_content); }
        void logInfo(const string &log_content) { log(INFO, log_content); }
        void logWarning(const string &log_content) { log(WARNING, log_content); }
        void logError(const string &log_content) { log(ERROR, log_content); }
        void logCritical(const string &log_content) { log(CRITICAL, log_content); }
        // 设置日志文件相对路径
        void setLogFilename(const string &file_name);
        // 设置是否输出到控制台
        void setLogToConsole(bool b) { m_log_to_console = b; }
        // 设置输出到控制台的最小等级
        void setLogToConsoleMinLevel(LogLevel min_level);

    public:
        // 获取单例对象
        static shared_ptr<Logger> instance();

    private:
        // 写日志
        void log(LogLevel level, const string &log_content);

    private:
        bool m_log_to_console = true;                        // 是否输出到控制台
        int m_log_to_console_min_level = DEBUG;              // 输出到控制台的最小等级
        mutex m_file_mutex[5];                               // 互斥锁
        ofstream m_file_out[5];                              // 输出文件流
        inline static shared_ptr<Logger> single_logger;      // 单例对象
        const inline static string LOG_ROOT_PATH = "./log/"; // 日志文件的根路径
    };
}