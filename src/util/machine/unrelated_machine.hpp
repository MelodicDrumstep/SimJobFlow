#pragma once

#include <vector>
#include <string>

#include "job.hpp"

namespace SJF
{

/**
 * @brief Machine type of Unrelated machine_model. 
 */
struct UnrelatedMachine
{
    int32_t machineId_ = Invalid_Machine_Id;
    int32_t jobId_ = Invalid_Job_Id;                   // the jobId of the current running job on this machine
    int64_t remaining_time_ = Invalid_Remaining_Time;   // the remaining processing time of the current running job 

    UnrelatedMachine() = default;

    UnrelatedMachine(int32_t machineId) : machineId_(machineId) {}

    bool operator==(const UnrelatedMachine& other) const = default;
    bool operator!=(const UnrelatedMachine& other) const = default;

    bool isFree() const
    {
        assert(((jobId_ == Invalid_Job_Id) && (remaining_time_ == Invalid_Remaining_Time)) || 
               ((jobId_ != Invalid_Job_Id) && (remaining_time_ != Invalid_Remaining_Time)));
        return (jobId_ == Invalid_Job_Id);
    }

    void setFree()
    {
        jobId_ = Invalid_Job_Id;
        remaining_time_ = Invalid_Remaining_Time;
    }

    void execute(int64_t jobId, int64_t remaining_time)
    {
        jobId_ = jobId;
        remaining_time_ = remaining_time;
    }

    void execute(const UnrelatedJob & job)
    {
        jobId_ = job.id_;
        remaining_time_ = job.processing_time_[machineId_];
    }

    std::string toString() const
    {
        std::string result = "machineId : " + std::to_string(machineId_) +
        ", jobId : " + std::to_string(jobId_) + 
        ", remaining Time : " + std::to_string(remaining_time_);
        return result;
    }   

    static constexpr int64_t Invalid_Machine_Id = -1;
    static constexpr int64_t Invalid_Remaining_Time = -1;
};

}