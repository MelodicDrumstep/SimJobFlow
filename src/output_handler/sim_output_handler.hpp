#pragma once

#include <vector>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

#include "model_trait.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief The output handler that directly output the string representation
 *  of the schedule steps.
 * 
 * @tparam machine_model Identical / Related / Unrelated
 */
template <Machine_Model machine_model>
class SimOutputHandler
{
using JobT = typename MachineModelTraits<machine_model>::JobT;
using MachineT = typename MachineModelTraits<machine_model>::MachineT;

public:
    SimOutputHandler(const json & config) {}

    void output(const std::vector<MachineT> & machines,
                const std::vector<JobT> & jobs,
                int64_t timestamp,
                const std::vector<ScheduleStep> & schedule_steps) {}

    void output(const std::vector<MachineT> & machines,
                const std::vector<JobT> & jobs,
                const std::vector<ScheduleStep> & schedule_steps) {}

    void output(const int64_t timestamp) {}
};

}