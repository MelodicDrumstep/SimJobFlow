#pragma once

#include <vector>
#include <type_traits>

#include "model_trait.hpp"
#include "greedy_scheduler_identical.hpp"
#include "greedy_scheduler_related.hpp"
#include "greedy_scheduler_unrelated.hpp"

namespace SJF
{

/**
 * @brief The scheduler using greedy strategy.
 * It will choose different back type corresponding to different model type.
 * 
 * @tparam model Identical / Related / Unrelated
 */
template <Model model>
using GreedyScheduler = std::conditional_t<
    model == Model::Identical,
    GreedySchedulerIdentical,
    std::conditional_t<
        model == Model::Related,
        GreedySchedulerRelated,
        GreedySchedulerUnrelated
    >
>;

}