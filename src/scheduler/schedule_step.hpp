#pragma once

#include <vector>
#include <string>

namespace SJF
{

/**
 * @brief A schedule step, containing the timestamp, and the id of the "machine, job" pair.
 */
struct ScheduleStep
{
    int64_t timestamp_;
    int64_t jobId_;
    int64_t machineId_;

    ScheduleStep(int64_t timestamp, int64_t jobId, int64_t machineId)
        : timestamp_(timestamp), jobId_(jobId), machineId_(machineId) {}

    std::string toString() const
    {
        std::string result = ("Timestamp = " + std::to_string(timestamp_)) + 
            (", jobId = " + std::to_string(jobId_)) +
            (", machineId = " + std::to_string(machineId_));
        return result;
    }
};

}