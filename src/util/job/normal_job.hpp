#pragma once

#include <string>

namespace SJF
{

/**
 * @brief Normal job, used in Identical machine_model and Related machine_model. 
 */
struct NormalJob
{
    int64_t timestamp_; // the timestamp when this job enters
    int64_t workload_;  // the workload of this job. In Identical machine_model, all machines have processing speed as 1
                        // therefore the workload is the same as the processing time of this job
                        // In Related machine_model, different machines have different processing speed
                        // therefore the processing time is "workload / processing speed of the machine"
    int64_t id_;        // jobId, sorted by timestamp

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
        return (timestamp_ < other.timestamp_) || (timestamp_ == other.timestamp_ && workload_ > other.workload_);
    }

    std::string toString() const
    {
        return "jobId : " + std::to_string(id_) + ", timestamp : " + std::to_string(timestamp_) + " workload : " + std::to_string(workload_);
    }
};

/**
 * @brief A compare function class used for creating the job heap.
 *      It compares two job just based on the workload.
 */
struct NormalJobWorkloadCmp
{
    bool operator()(const NormalJob & job1, const NormalJob & job2) const
    {
        return (job1.workload_ < job2.workload_);
    }
};

}