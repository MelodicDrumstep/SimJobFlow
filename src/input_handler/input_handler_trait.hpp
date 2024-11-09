#pragma once

#include <concepts>
#include <optional>
#include <vector>

#include "model_trait.hpp"

namespace SJF
{

template <typename InputHandlerT, Model model>
concept InputHandler = requires(InputHandlerT input_handler, int64_t timestamp, int64_t num_of_machines) 
{
    { input_handler.checkValidity(num_of_machines) } -> std::same_as<bool>;
    { input_handler.getJobs(timestamp) } -> std::same_as<std::optional<std::vector<typename ModelTraits<model>::JobT>>>;
    { input_handler.done() } -> std::same_as<bool>;
};

}