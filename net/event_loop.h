#pragma once

#include "types.h"
#include <thread>
#include <vector>

namespace muduo
{
    class Channel;
    class Poller;

    class EventLoop: noncopyable
    {
    public:
        using ChannelList = std::vector<Channel*>;

        EventLoop();
        ~EventLoop();

        void Loop();
        void Quit();

        //internal use only
        void UpdateChannel(Channel* channel);
        void RemoveChannel(Channel* channel);

        bool IsInLoopThread() const { return tid_ == std::this_thread::get_id(); }

        void assertInLoopThread()
        {
            if (!IsInLoopThread())
                AbortNotInLoopThread();
        }

    private:
        void AbortNotInLoopThread();

        std::thread::id tid_;
        bool looping_;
        bool quit_;
        std::unique_ptr<Poller> poller_;
        ChannelList active_channels_;
    };
}

