#include "event_loop.h"
#include "channel.h"
#include "poller.h"
#include "log4z.h"

#include <poll.h>
#include <cassert>
#include <sstream>

using namespace zsummer::log4z;
using namespace muduo;


__thread EventLoop* t_loopInThisThread = nullptr;
const int kPollTimeMs = 10000;


EventLoop::EventLoop()
        : looping_(false),
          quit_(false),
          tid_(std::this_thread::get_id()),
          poller_(new Poller(this))
{
    std::stringstream tid_stream;
    tid_stream << tid_;

    LOGT("EventLoop created" << this << "in thread" << tid_stream.str());
    if(t_loopInThisThread != nullptr)
    {
        LOGF("Another EventLoop" << t_loopInThisThread
                                 << "exits in this thread" << tid_stream.str());
    }
    else
    {
        t_loopInThisThread == this;
    }
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    t_loopInThisThread = nullptr;
}

void EventLoop::Loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while (!quit_)
    {
        active_channels_.clear();
        poller_->Poll(kPollTimeMs, &active_channels_);
        for (auto it = active_channels_.begin(); it!= active_channels_.end(); ++it)
        {
            (*it)->HandleEvent();
        }

    }

    LOGT("EventLoop" << this << " stop looping");
    looping_ = false;
}

void EventLoop::Quit()
{
    quit_ = true;
}

void EventLoop::UpdateChannel(Channel* channel)
{
    assert(channel->OwnerLoop() == this);
    assertInLoopThread();
    poller_->UpdateChannel(channel);
}


void EventLoop::AbortNotInLoopThread()
{
    std::stringstream tid_stream;
    tid_stream << tid_;

    std::stringstream current_thread_id_stream;
    current_thread_id_stream << std::this_thread::get_id();

    LOGF("EventLoop::abortNotInLoopThread - EventLoop" << this
                                                       << " was created in threadID = " << tid_stream.str()
                                                       << ", current thread id = " << current_thread_id_stream.str());
}




