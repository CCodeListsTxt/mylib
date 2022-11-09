#include "../include/log.hpp"

using namespace mylib;
int main()
{
    shared_ptr<Logger> log = Logger::instance();

    log->setLogFilename("2022/11/9/logfile");

    log->logDebug("debug content");

    return 0;
}