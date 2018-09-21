#include "event_loop.h"
#include "log4z.h"

muduo::EventLoop* g_loop;

void threadFunc()
{
    g_loop->Loop();

}

using namespace zsummer::log4z;

int main(int argc, char* argv[])
{
    ILog4zManager::getRef().start();

    muduo::EventLoop loop;
    g_loop = &loop;
    std::thread thread(threadFunc);
    thread.join();
}
