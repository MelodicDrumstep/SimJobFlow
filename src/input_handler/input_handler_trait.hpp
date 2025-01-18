#pragma once

#include <concepts>
#include <vector>

#include "model_trait.hpp"

namespace SJF
{

/**
 * @brief A concept constraining the input handler type.
 */
template <typename InputHandlerT, Machine_Model machine_model>
concept InputHandler = requires(InputHandlerT input_handler, int64_t timestamp, int64_t num_of_machines) 
{
    { input_handler.checkValidity(num_of_machines) } -> std::same_as<bool>;
    { input_handler.getJobs(timestamp) } -> std::same_as<std::vector<typename MachineModelTraits<machine_model>::JobT>>;
    { input_handler.done() } -> std::same_as<bool>;
};

template <typename InputHandlerT, Machine_Model machine_model>
concept OPTInputHandler = InputHandler<InputHandlerT, machine_model> &&
requires(InputHandlerT input_handler) 
{
    { input_handler.refresh() } -> std::same_as<void>;
    { input_handler.getJobArray() } -> std::same_as<const std::vector<typename MachineModelTraits<machine_model>::JobT>&>;
};

}