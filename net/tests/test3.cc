#include "channel.h"
#include "event_loop.h"
#include "log4z.h"

#include <iostream>
#include <sys/timerfd.h>
#include <cstring>
#include <unistd.h>
#include <string.h>

muduo::EventLoop* g_loop;

void timeout()
{
    std::cout << "Time Out." << std::endl;
    g_loop->Quit();
}

using namespace zsummer::log4z;
int main(int argc, char* argv[])
{
    ILog4zManager::getRef().start();

    muduo::EventLoop loop;
    g_loop = &loop;

    int timefd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    muduo::Channel channel(&loop, timefd);
    channel.set_read_callback(timeout);
    channel.EnableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 3;
    ::timerfd_settime(timefd, 0, &howlong, NULL);

    loop.Loop();

    ::close(timefd);
}
