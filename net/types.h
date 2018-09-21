#pragma once

namespace muduo
{
    class copyable
    {};

    class noncopyable
    {
    public:
        noncopyable()= default;
        ~noncopyable()= default;

        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
}
