#include "event_loop.h"
#include "log4z.h"

#include <sstream>
#include <iostream>

void threadFunc()
{
    std::stringstream current_thread_id_stream;
    current_thread_id_stream << std::this_thread::get_id();
    std::cout << "thread Function id is : " << current_thread_id_stream.str() << std::endl;

    muduo::EventLoop loop;
    loop.Loop();
}

using namespace zsummer::log4z;

int main(int argc, char* argv[])
{
    ILog4zManager::getRef().start();

    muduo::EventLoop loop;
    std::thread thd(threadFunc);

    loop.Loop();

    std::stringstream current_thread_id_stream;
    current_thread_id_stream << std::this_thread::get_id();
    std::cout << "main Function id is : " << current_thread_id_stream.str() << std::endl;

    thd.join();
}
