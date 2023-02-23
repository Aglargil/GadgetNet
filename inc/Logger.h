#pragma once

#include <cstdio>
#include <iostream>
#include <iterator>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>
#include "Common.h"
#include "Timestamp.h"

#define LOG_(type, format, ...)\
    do {\
        Logger &logger = Logger::instance();\
        char buffer[1024] = {0};\
        std::snprintf(buffer, 1024, format, ##__VA_ARGS__);\
        std::thread::id tid = std::this_thread::get_id();\
        std::stringstream ss;\
        ss << tid;\
        char info[256] = {0};\
        std::snprintf(info, 256, "[%s][%s:%d %s()] ", ss.str().c_str(), __FILE__, __LINE__, __FUNCTION__); \
        logger.log(type, std::string(info) + std::string(buffer));\
    } while (0)

#define LOG_INFO(format, ...) LOG_(INFO, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_(ERROR, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) LOG_(FATAL, format, ##__VA_ARGS__)
#ifdef DEBUGMODE
#define LOG_DEBUG(format, ...) LOG_(DEBUG, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)  
#endif

#define FUNCTION_DEBUG LOG_DEBUG(" ")

enum LogLevel {
    INFO,
    ERROR,
    FATAL,
    DEBUG
};

class Logger {

public:
    NONCOPYABLE(Logger)

    Logger() = default;

    ~Logger() = default;

    static Logger &instance() {
        static Logger logger;
        return logger;
    }

    void log(LogLevel level, const std::string& msg){
        std::string prefix = "";
        switch (level)
        {
            case INFO:  prefix = "[INFO] ["; break;
            case ERROR: prefix = "[ERROR]["; break;
            case FATAL: prefix = "[FATAL]["; break;
            case DEBUG: prefix = "[DEBUG]["; break;
            default:                     break;
        }
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        std::cout << prefix + Timestamp::now().toString() + "]:" + msg << std::endl;
    }

private:
    std::recursive_mutex mutex_;
};