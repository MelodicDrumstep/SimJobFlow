#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>
#include <model_trait.hpp>

namespace SJF
{

/**
 * @brief A concept constraining the timer type.
 */
template <typename TimerT, Model model>
concept Timer = requires(TimerT timer, const std::vector<typename ModelTraits<model>::MachineT> & machines) 
{
    { timer.tick(machines) } -> std::same_as<int64_t>;
    { std::as_const(timer).timestamp() } -> std::same_as<int64_t>;
};

}