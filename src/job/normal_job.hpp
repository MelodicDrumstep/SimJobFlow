#pragma once

#include <string>

struct NormalJob
{
    int64_t timestamp_;
    int64_t workload_;

    NormalJob(int64_t timestamp, int64_t workload)
     : timestamp_(timestamp), workload_(workload) {}

    NormalJob(const NormalJob & other) = default;

    bool operator<(const NormalJob & other) const
    {
        return (timestamp_ < other.timestamp_) || (timestamp_ == other.timestamp_ && workload_ < other.workload_);
    }

    std::string toString() const
    {
        return "timestamp = " + std::to_string(timestamp_) + " workload = " + std::to_string(workload_);
    }
};