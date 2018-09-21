#include "poller.h"

#include "channel.h"

#include <cassert>
#include <poll.h>
#include "log4z.h"

using namespace muduo;

Poller::Poller(EventLoop* loop)
        : owner_loop_(loop)
{

}

Poller::~Poller()
{
}

Timestamp Poller::Poll(int timeOutMs, ChannelList* activeChannels)
{
    int numEvents = ::poll(pollfds_.data(), pollfds_.size(), timeOutMs);
    Timestamp now(Timestamp::now());
    if(numEvents > 0)
    {
        LOGT(numEvents << " events happened");
        FillActiveChannels(numEvents, activeChannels);
    }
    else if(numEvents == 0)
    {
        LOGT("nothing happened");
    }
    else { LOGE("Poller::Poll()"); }
    return now;
}

void Poller::FillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
    for(auto pfd = pollfds_.begin(); pfd!=pollfds_.end() && numEvents > 0; ++pfd)
    {
        if(pfd->revents >0)
        {
            --numEvents;
            const auto ch = channels_.find(pfd->fd);
            assert(ch != channels_.end());
            auto channel = ch->second;
            assert(channel->fd() == pfd->fd);
            channel->set_revents(pfd->revents);
            activeChannels->push_back(channel);
        }
    }
}

void Poller::UpdateChannel(Channel* channel)
{
    AssertInLoopThread();
    LOGT("fd = " << channel->fd() << "events = " << channel->events());

    if(channel->index() < 0)
    {
        assert(channels_.find(channel->fd()) == channels_.end());
        struct pollfd pfd;
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        pollfds_.push_back(pfd);

        int index = static_cast<int>(pollfds_.size()) - 1;
        channel->set_index(index);
        channels_[pfd.fd] = channel;
    }else
    {
        auto index = channel->index();
        assert(0 <= index && index < static_cast<int>(pollfds_.size()));
        auto& pfd = pollfds_[index];
        assert(pfd.fd == channel->fd() || pfd.fd == -1);
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;

        if(channel->IsNoneEvent())
        {
            pfd.fd = -1;
        }
    }
}
