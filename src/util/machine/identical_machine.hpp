#pragma once

#include <vector>
#include <memory>
#include <string>

#include "job.hpp"

namespace SJF
{

/**
 * @brief Machine type of Identical machine_model. 
 */
struct IdenticalMachine
{
    int64_t machineId_ = Invalid_Machine_Id;
    int64_t jobId_ = Invalid_Job_Id;                   // the jobId of the current running job on this machine
    int64_t remaining_time_ = Invalid_Remaining_Time;   // the remaining processing time of the current running job 

    IdenticalMachine() = default;

    IdenticalMachine(int64_t machineId) : machineId_(machineId) {} 

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
        std::string result = "Machine : " + std::to_string(machineId_) + "\n";
        result += "Job : " + std::to_string(jobId_) + "\n";
        result += "Remaining Time : " + std::to_string(remaining_time_) + "\n";
        return result;
    }

    static constexpr int64_t Invalid_Machine_Id = -1;
    static constexpr int64_t Invalid_Remaining_Time = -1; 
};

}