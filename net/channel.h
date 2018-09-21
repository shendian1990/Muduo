#pragma once

#include <functional>
#include "types.h"

namespace muduo
{
    class EventLoop;

    class Channel: noncopyable
    {
    public:

        using EventCallback = std::function<void()>;

        Channel(EventLoop* loop, int fd);

        void HandleEvent();
        void set_read_callback(const EventCallback& callback)
        {
            read_callback_ = callback;
        }
        void set_write_callback(const EventCallback& callback)
        {
            write_callback_ = callback;
        }
        void set_error_callback(const EventCallback& callback)
        {
            error_callback_ = callback;
        }

        int fd() const { return fd_; }
        int events() const { return events_; }
        void set_revents(int revent) { revents_ = revent; }
        bool IsNoneEvent() const { return events_ == kNonoEvent; }

        void EnableReading() { events_ |= kReadEvent; update(); }
        void EnableWriting() { events_ |= kWriteEvent; update(); }
        void DisableReading() { events_ &= ~kReadEvent; update(); }
        void DisableWriting() { events_ |= ~kWriteEvent; update(); }

        // For Poller
        int index() const { return index_; }
        void set_index(int index) { index_ = index; }

        EventLoop* OwnerLoop() const { return loop_; }

    private:
        void update();

        static const int kNonoEvent;
        static const int kReadEvent;
        static const int kWriteEvent;

        EventLoop* loop_;
        const int fd_;
        int events_;
        int revents_;
        int index_;

        EventCallback read_callback_;
        EventCallback write_callback_;
        EventCallback error_callback_;
    };
}