#include "timestamp.h"

#include <chrono>
#include <ctime>

using namespace muduo;

Timestamp::Timestamp(int64_t microseconds)
        : micro_seconds_since_epoch_(microseconds)
{}

std::string Timestamp::toString() const
{
    char buf[32] = { 0 };
    const auto seconds = micro_seconds_since_epoch_ / kMicroSecondsPerSecond;
    const auto microseconds = micro_seconds_since_epoch_ % kMicroSecondsPerSecond;

    snprintf(buf, sizeof(buf), "%lld.%06lld", seconds, microseconds);
    return buf;
}

std::string Timestamp::toFormattedString() const
{
    char buf[32] = { 0 };
    auto seconds = static_cast<time_t>(micro_seconds_since_epoch_ / kMicroSecondsPerSecond);
    int microseconds = static_cast<time_t>(micro_seconds_since_epoch_ % kMicroSecondsPerSecond);

    ctime_r(&seconds, buf);
    return buf;
}

Timestamp Timestamp::now()
{
    return Timestamp(std::chrono::duration_cast<std::chrono::microseconds>
                             (std::chrono::system_clock::now().time_since_epoch()).count());
}

Timestamp Timestamp::invalid()
{
    return Timestamp{0};
}


