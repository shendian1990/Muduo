#include "channel.h"
#include "event_loop.h"
#include "log4z.h"

#include <sstream>

#include <poll.h>

using namespace muduo;

const int Channel::kNonoEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
        : loop_(loop),
          fd_(fd),
          events_(0),
          revents_(0),
          index_(-1)
{
}

void Channel::update()
{
    loop_->UpdateChannel(this);
}

void Channel::HandleEvent()
{
    if(revents_ & POLLNVAL)
    {
        LOGW("Channel::HandleEvent() POLLNVAL");
    }
    if(revents_ & (POLLERR | POLLNVAL))
    {
        if (error_callback_)
            error_callback_();
    }

    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (read_callback_)
            read_callback_();
    }
    if(revents_ & POLLOUT)
    {
        if (write_callback_)
            write_callback_();
    }
}


