#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>

template <typename T>
concept Timer = requires(T timer) 
{
    { timer.tick() } -> std::same_as<void>;
    { timer.timestamp() } -> std::same_as<size_t>;
};