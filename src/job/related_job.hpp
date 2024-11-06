#pragma once

#include <vector>
#include <array>

template <size_t Num_of_Machines>
struct RelatedJob
{
    size_t index_;
    std::array<int64_t, Num_of_Machines> processing_speeds_;
};