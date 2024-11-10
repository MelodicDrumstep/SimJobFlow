#pragma once

#include <vector>

#include "model_trait.hpp"
#include "schedule_step.hpp"

namespace SJF
{

/**
 * @brief A concept constraining the output handler type.
 */
template <typename OutputHandlerT, Model model>
concept OutputHandler = requires(OutputHandlerT output_handler,
                                 std::vector<typename ModelTraits<model>::MachineT> machines,
                                 std::vector<typename ModelTraits<model>::JobT> jobs,
                                 int64_t timestamp,
                                 std::vector<ScheduleStep> schedule_steps)
{
    { output_handler.output(machines, jobs, timestamp, schedule_steps) } -> std::same_as<void>;
};
}