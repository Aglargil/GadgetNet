#pragma once

#include <cstdio>
#include <ctime>
#include <string>
#include <chrono>
class Timestamp {
    using system_clock = std::chrono::system_clock;
public:
    Timestamp() = delete;
    Timestamp(system_clock::time_point time) : time_(time) {;}
    ~Timestamp() = default;
    
    std::string toString() const {
        struct std::tm info;
        char buffer[64];
        auto t = system_clock::to_time_t(time_);
        localtime_r(&t, &info);
        std::strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", &info);
        return buffer;
    }

    static Timestamp now() {
        return Timestamp(system_clock::now());
    }
    // 当前时刻加上x毫秒
    static Timestamp nowAddMS(int64_t x) {
        return Timestamp(system_clock::now() + std::chrono::milliseconds(x));
    }
    // 当前时刻加上x微秒
    static Timestamp nowAddUS(int64_t x) {
        return Timestamp(system_clock::now() + std::chrono::microseconds(x));
    }

    const system_clock::time_point& getTime() const {return time_;}
private:
    system_clock::time_point time_;
};
