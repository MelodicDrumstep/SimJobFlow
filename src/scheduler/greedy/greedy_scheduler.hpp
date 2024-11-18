#pragma once

#include <vector>
#include <type_traits>

#include "model_trait.hpp"
#include "greedy_scheduler_identical_list_arrival.hpp"
#include "greedy_scheduler_identical_real_time_arrival.hpp"
#include "greedy_scheduler_related_list_arrival.hpp"
// #include "greedy_scheduler_unrelated_list_arrival.hpp"
#include "not_supported_scheduler_type.hpp"

namespace SJF
{

/**
 * @brief The scheduler using greedy strategy.
 * It will choose different back type corresponding to different machine_model type.
 * 
 * @tparam machine_model Identical / Related / Unrelated
 * @tparam release_model List_Arrival / Real_Time_Arrival
 */
template <Machine_Model machine_model, Release_Model release_model>
using GreedyScheduler = std::conditional_t<
    machine_model == Machine_Model::Identical,
    std::conditional_t<
        release_model == Release_Model::List_Arrival,
        GreedySchedulerIdenticalListArrival,
        GreedySchedulerIdenticalRealTimeArrival
    >,
    std::conditional_t<
        machine_model == Machine_Model::Related,
        std::conditional_t<
            release_model == Release_Model::List_Arrival,
            GreedySchedulerRelatedListArrival,
            NotSupportedSchedulerType
        >,
        std::conditional_t<
            release_model == Release_Model::List_Arrival,
            NotSupportedSchedulerType,
            NotSupportedSchedulerType
        >
    >
>;

}