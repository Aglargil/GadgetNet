#pragma once
#include <future>

#define NONCOPYABLE(CLASS_NAME)                         \
    CLASS_NAME(const CLASS_NAME& ) = delete;            \
    CLASS_NAME &operator=(const CLASS_NAME &) = delete;

template<typename F, typename... Ts>
inline
std::future<typename std::result_of<F(Ts...)>::type>
reallyAsync(F&& f, Ts&&... params)     // 返回异步调用f(param...)的future
{
    return std::async(std::launch::async,
                      std::forward<F>(f),
                      std::forward<Ts>(params)...);
}
