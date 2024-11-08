#pragma once

struct NormalJob
{
    int64_t timestamp_;
    int64_t workload_;

    NormalJob(int64_t timestamp, int64_t workload)
     : timestamp_(timestamp), workload_(workload) {}

    bool operator<(const NormalJob & other) const
    {
        return (timestamp_ < other.timestamp_);
    }
};