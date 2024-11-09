#pragma once

#include <string>

namespace SJF
{

struct NormalJob
{
    int64_t timestamp_;
    int64_t workload_;
    int64_t id_;

    NormalJob(int64_t timestamp, int64_t workload)
     : timestamp_(timestamp), workload_(workload) {}

    NormalJob(int64_t timestamp, int64_t workload, int64_t id)
     : timestamp_(timestamp), workload_(workload), id_(id) {}

    NormalJob(const NormalJob & other) = default;
    NormalJob(NormalJob && other) = default;
    NormalJob & operator=(const NormalJob & other) = default;
    NormalJob & operator=(NormalJob && other) = default;

    bool operator<(const NormalJob & other) const
    {
        return (timestamp_ < other.timestamp_) || (timestamp_ == other.timestamp_ && workload_ < other.workload_);
    }

    std::string toString() const
    {
        return "Id = " + std::to_string(id_) + ", timestamp = " + std::to_string(timestamp_) + " workload = " + std::to_string(workload_);
    }
};

struct NormalJobWorkloadCmp
{
    bool operator()(const NormalJob & job1, const NormalJob & job2) const
    {
        return (job1.workload_ < job2.workload_);
    }
};

}