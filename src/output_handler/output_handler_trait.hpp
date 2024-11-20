#pragma once

#include <vector>

#include "basic_utils_in_one_header.hpp"

namespace SJF
{

/**
 * @brief A concept constraining the output handler type.
 */
template <typename OutputHandlerT, Machine_Model machine_model>
concept OutputHandler = requires(OutputHandlerT output_handler,
                                 std::vector<typename MachineModelTraits<machine_model>::MachineT> machines,
                                 std::vector<typename MachineModelTraits<machine_model>::JobT> jobs,
                                 int64_t timestamp,
                                 std::vector<ScheduleStep> schedule_steps)
{
    { output_handler.output(machines, jobs, timestamp, schedule_steps) } -> std::same_as<void>;
    { output_handler.output(timestamp) } -> std::same_as<void>;
};
}