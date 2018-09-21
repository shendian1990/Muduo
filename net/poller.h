#pragma once

#include <map>
#include <vector>

#include "timestamp.h"
#include "event_loop.h"

struct pollfd;

namespace muduo
{
    class Channel;

    class Poller: noncopyable
    {
    public:
        using ChannelList = std::vector<Channel*>;
        using PollFdList = std::vector<struct pollfd> ;
        using ChannelMap = std::map<int, Channel*> ;

        Poller(EventLoop* loop);
        ~Poller();

        ///Polls the I/O events.
        ///Must be called in the thread.
        Timestamp Poll(int timeOutMs, ChannelList* activeChannels);

        void UpdateChannel(Channel* channel);

        void AssertInLoopThread() {}

    private:
        void FillActiveChannels(int numEvents,
                                ChannelList* activeChannels) const;

        EventLoop* owner_loop_;
        PollFdList pollfds_;
        ChannelMap channels_;
    };
}