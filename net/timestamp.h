#pragma once

#include "types.h"
#include <cstdint>
#include <string>

namespace muduo
{
    class Timestamp: public copyable
    {
    public:
        static const int kMicroSecondsPerSecond = 1000 * 1000;

        Timestamp() : micro_seconds_since_epoch_(0)
        {}

        explicit Timestamp(int64_t microSecondsSinceEpoch);

        std::string toString() const;
        std::string toFormattedString() const;

        bool Valid() const { return micro_seconds_since_epoch_ > 0; }

        //for internal usage.
        int64_t micro_seconds_since_epoch() const { return micro_seconds_since_epoch_; }
        time_t seconds_since_epoch() const { return micro_seconds_since_epoch_ / kMicroSecondsPerSecond; }

        ///
        /// Get time of now.
        ///
        static Timestamp now();
        static Timestamp invalid();

    private:
        int64_t micro_seconds_since_epoch_;
    };

    ///
    /// Gets time difference of two timestamps, result in seconds.
    ///
    /// @param high, low
    /// @return (high-low) in seconds
    /// @c double has 52-bit precision, enough for one-microseciond
    /// resolution for next 100 years.
    inline double timeDifference(Timestamp high, Timestamp low)
    {
        int64_t diff = high.micro_seconds_since_epoch() - low.micro_seconds_since_epoch();
        return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
    }

    inline Timestamp addTime(Timestamp timestamp, double seconds)
    {
        auto const delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
        return Timestamp(timestamp.micro_seconds_since_epoch() + delta);
    }

}
