#pragma once

#include <vector>
#include <array>

#include <string>

struct UnrelatedJob
{
    int64_t timestamp_;
    std::vector<int64_t> processing_time_;

    UnrelatedJob(int64_t timestamp, std::vector<int64_t> && processing_speeds)
     : timestamp_(timestamp), processing_time_(std::move(processing_speeds)) {}

    UnrelatedJob(const UnrelatedJob & other) = default;

    bool operator<(const UnrelatedJob & other) const
    {
        return (timestamp_ < other.timestamp_);
    }

    std::string toString() const
    {
        std::string result = "timestamp = " + std::to_string(timestamp_) + "\nprocessing_time = \n";
        for (const auto & speed : processing_time_)
        {
            result += " " + std::to_string(speed);
        }
        return result;
    }
};