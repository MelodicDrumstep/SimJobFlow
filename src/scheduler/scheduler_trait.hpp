#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>
#include <model_trait.hpp>
#include <vector>
#include <schedule_step.hpp>

namespace SJF
{

/**
 * @brief A concept constraining the scheduler type.
 */
template <typename SchedulerT, Machine_Model machine_model>
concept Scheduler = requires(SchedulerT scheduler, 
                             const std::vector<typename MachineModelTraits<machine_model>::JobT> & jobs_for_this_turn,
                             std::vector<typename MachineModelTraits<machine_model>::MachineT> & machines,
                             int64_t timestamp,
                             int64_t num_of_machines,
                             int64_t elapsing_time)
{
    { scheduler.schedule(jobs_for_this_turn, machines, timestamp) } -> std::same_as<std::vector<ScheduleStep>>;
    { scheduler.initialize(num_of_machines) } -> std::same_as<void>;
    { scheduler.updateMachineState(machines, elapsing_time) } -> std::same_as<void>;
    { scheduler.done() } -> std::same_as<bool>;
};

}