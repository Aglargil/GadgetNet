#pragma once

#include <ctime>
#include <string>
#include <chrono>

class Timestamp {

public:
    Timestamp() = delete;
    Timestamp(std::time_t time) : time_(time) {;}

    ~Timestamp() = default;
    
    std::string toString() const {
        struct std::tm info;
        char buffer[64];
        localtime_r(&time_, &info);
        std::strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", &info);
        return buffer;
    }

    static Timestamp now() {
        return Timestamp(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    }

private:
    std::time_t time_;
};
