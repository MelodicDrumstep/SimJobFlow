#pragma once

#include <vector>
#include <array>

template <size_t Num_of_Machines>
struct UnrelatedJob
{
    int64_t timestamp_;
    size_t index_;
    std::array<int64_t, Num_of_Machines> processing_speeds_;

    UnrelatedJob(int64_t timestamp, size_t index, std::array<int64_t, Num_of_Machines> processing_speeds)
     : timestamp_(timestamp), index_(index), processing_speeds_(processing_speeds) {}

    bool operator<(const UnrelatedJob & other) const
    {
        return (timestamp_ < other.timestamp_);
    }
};