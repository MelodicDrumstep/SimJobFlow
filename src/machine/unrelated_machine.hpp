#pragma once

#include <vector>
#include <string>

#include "job.hpp"

namespace SJF
{

/**
 * @brief Machine type of Unrelated model. 
 */
struct UnrelatedMachine
{
    int32_t machineId_ = Invalid_Machine_Id;
    int32_t job_id_ = Invalid_Job_Id;                   // the jobId of the current running job on this machine
    int64_t remaining_time_ = Invalid_Remaining_Time;   // the remaining processing time of the current running job 

    UnrelatedMachine() = default;

    UnrelatedMachine(int32_t machineId) : machineId_(machineId) {}

    void setFree()
    {
        job_id_ = Invalid_Job_Id;
        remaining_time_ = Invalid_Remaining_Time;
    }

    std::string toString() const
    {
        std::string result = "Machine : " + std::to_string(machineId_) + "\n";
        result += "Job : " + std::to_string(job_id_) + "\n";
        result += "Remaining Time : " + std::to_string(remaining_time_) + "\n";
        return result;
    }   

    static constexpr int64_t Invalid_Machine_Id = -1;
    static constexpr int64_t Invalid_Remaining_Time = -1;
};

}