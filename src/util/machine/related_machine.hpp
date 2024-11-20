#pragma once

#include <vector>
#include <string>

#include "job.hpp"

namespace SJF
{

/**
 * @brief Machine type of Related machine_model. 
 */
struct RelatedMachine
{
    int64_t machineId_ = Invalid_Machine_Id;
    int64_t processing_speed_ = Invalid_Processing_Speed;   // the processing speed of this machine
    int64_t jobId_ = Invalid_Job_Id;                       // the jobId of the current running job on this machine
    int64_t remaining_time_ = Invalid_Remaining_Time;       // the remaining processing time of the current running job 

    RelatedMachine() = default;

    RelatedMachine(int64_t machineId, int64_t processing_speed)
     : machineId_(machineId), processing_speed_(processing_speed) {}

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

    void execute(int64_t job_id, int64_t remaining_time)
    {
        jobId_ = job_id;
        remaining_time_ = remaining_time;
    }

    std::string toString() const
    { 
        std::string result = "machineId : " + std::to_string(machineId_) + 
        ", processing_speed : " + std::to_string(processing_speed_) +
        ", jobId : " + std::to_string(jobId_) + 
        ", remaining Time : " + std::to_string(remaining_time_);
        return result;
    }

    static constexpr int64_t Invalid_Machine_Id = -1;
    static constexpr int64_t Invalid_Processing_Speed = -1;
    static constexpr int64_t Invalid_Remaining_Time = -1;
};


}