#pragma once

#include <vector>
#include <type_traits>

#include "model_trait.hpp"
#include "MR_scheduler_identical_list_arrival.hpp"
#include "not_supported_scheduler_type.hpp"

namespace SJF
{

/**
 * @brief The scheduler using MR strategy.
 * It will choose different back type corresponding to different machine_model type.
 * 
 * @tparam machine_model Identical / Related / Unrelated
 * @tparam release_model List_Arrival / Real_Time_Arrival
 */
template <Machine_Model machine_model, Release_Model release_model>
using MRScheduler = std::conditional_t<
    machine_model == Machine_Model::Identical,
    std::conditional_t<
        release_model == Release_Model::List_Arrival,
        MRSchedulerIdenticalListArrival,
        NotSupportedSchedulerType
    >,
    std::conditional_t<
        machine_model == Machine_Model::Related,
        std::conditional_t<
            release_model == Release_Model::List_Arrival,
            NotSupportedSchedulerType,
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