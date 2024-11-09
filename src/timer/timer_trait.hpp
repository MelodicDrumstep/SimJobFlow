#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>
#include <model_trait.hpp>

namespace SJF
{

template <typename TimerT, Model model>
concept Timer = requires(TimerT timer, const std::vector<typename ModelTraits<model>::MachineT> & machines) 
{
    { timer.tick(machines, machine_free_list) } -> std::same_as<int64_t>;
    { timer.timestamp() } -> std::same_as<size_t>;
};

}