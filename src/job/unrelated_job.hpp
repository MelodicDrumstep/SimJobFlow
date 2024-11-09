#pragma once

#include <vector>
#include <array>

#include <string>

namespace SJF
{

struct UnrelatedJob
{
    int64_t timestamp_;
    std::vector<int64_t> processing_time_;
    int64_t id_;

    UnrelatedJob(int64_t timestamp, std::vector<int64_t> && processing_speeds)
     : timestamp_(timestamp), processing_time_(std::move(processing_speeds)) {}

    UnrelatedJob(int64_t timestamp, std::vector<int64_t> && processing_speeds, int64_t id)
     : timestamp_(timestamp), processing_time_(std::move(processing_speeds)), id_(id) {}

    UnrelatedJob(const UnrelatedJob & other) = default;
    UnrelatedJob(UnrelatedJob && other) = default;
    UnrelatedJob & operator=(const UnrelatedJob & other) = default;
    UnrelatedJob & operator=(UnrelatedJob && other) = default;

    bool operator<(const UnrelatedJob & other) const
    {
        return (timestamp_ < other.timestamp_);
    }

    std::string toString() const
    {
        std::string result = "Id = " + std::to_string(id_) + ", timestamp = " + std::to_string(timestamp_) + "\nprocessing_time = \n";
        for (const auto & speed : processing_time_)
        {
            result += " " + std::to_string(speed);
        }
        return result;
    }
};

}