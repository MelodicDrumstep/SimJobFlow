#pragma once

#include <vector>
#include <array>

#include <string>

namespace SJF
{

/**
 * @brief Unrelated job, used in Unrelated machine_model.
 */
struct UnrelatedJob
{
    int64_t timestamp_;         // the timestamp when this job enters
    std::vector<int64_t> processing_time_;
        // In unrelated machine_model, each "machine, job" pair has different processing time
        // And it's given in the job struct
    int64_t id_;                // jobId, sorted by timestamp

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
        std::string result = "jobId : " + std::to_string(id_) + ", timestamp : " + std::to_string(timestamp_) + ", processing_time : [";
        for (const auto & speed : processing_time_)
        {
            result += std::to_string(speed) + ", ";
        }
        result.pop_back();
        result.pop_back();
        result += ']';
        return result;
    }
};

}