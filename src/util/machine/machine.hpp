#pragma once

#include <string>
#include <sstream>

#include "identical_machine.hpp"
#include "related_machine.hpp"
#include "unrelated_machine.hpp"

namespace SJF
{
    constexpr int64_t Invalid_Machine_Id = -1;
    constexpr int64_t Invalid_Remaining_Time = -1;

    template <typename MachineT>
    concept Machine = requires(MachineT machine)
    {
        { machine.machineId_ };
        { machine.jobId_ };
        { machine.remaining_time_ };
    };

    template <typename MachineT>
    static inline std::string toSnapShot(const std::vector<MachineT> & machines)
    {
        std::stringstream ss;
        ss << std::left;  // Set left alignment
        ss << std::setw(10) << "MachineId" << std::setw(10) << "JobId" << std::setw(15) << "RemainingTime" << "\n";
        for(const auto & machine : machines)
        {
            ss << std::setw(10) << machine.machineId_
            << std::setw(10) << machine.jobId_
            << std::setw(15) << machine.remaining_time_ << "\n";
        }
        return ss.str();
    }
}