#### 简介

* 基于单例模式设计的，线程安全的日志库。

* 一共具有5个日志等级，从上到下等级递增。

  ```cpp
  enum LogLevel
  {
      DEBUG,
      INFO,
      WARNING,
      ERROR,
      CRITICAL
  };
  ```

  不同等级的日志会被写入不同后缀的文件中，由用户设置文件路径。

  如设置路径`logfile`后，会生成`logfile_DEBUG`、`logfile_INFO`等日志文件。

---

#### 基本使用

```cpp
#include "log.h"
using namespace mylib;

int main()
{
    // 获取log实例对象，智能指针类型
    shared_ptr<Logger> log = Logger::instance();
    
    // 设置日志文件相对路径，根路径是./log
    log->setLogFilename("logfile");
    
    // 写日志，默认还会输出到控制台
    log->logDebug("log content");
    
    return 0;
}
```

---

#### API

```cpp
// 写日志
// 输出到文件的格式如 : 		2022-11-09 23:51:53 DEBUG : debug content
// 输出到控制台的格式如 : 	(2022-11-09 23:51:53)DEBUG      : debug content
void logDebug(const string &log_content);
void logInfo(const string &log_content);
void logWarning(const string &log_content);
void logError(const string &log_content);
void logCritical(const string &log_content);

// 设置是否输出到控制台，以及输出到控制台的最低日志等级
void setLogToConsole(bool b);
void setLogToConsoleMinLevel(LogLevel min_level);

// 设置日志文件相对路径
// 会递归生成目录，如传入 2022/11/9/logfile，会生成下面五个文件
// 2022/11/9/logfile_DEBUG、2022/11/9/logfile_INFO、......
void setLogFilename(const string &file_name);
```

