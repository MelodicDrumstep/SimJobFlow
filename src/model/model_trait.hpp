#pragma once

#include <type_traits>

#include "model.hpp"
#include "job.hpp"
#include "machine.hpp"

namespace SJF
{

/**
 * @brief The traits of the model, mapping model type to job type and machine type.
 */
template <Model model>
struct ModelTraits
{
/*
 if model == Model::Indentical
    then MachineT = IndenticalMachine
        JobT = NormalJob
 else if model == Model::Related
    then MachineT = RelatedMachine
        JobT = NormalJob
 else if model == Model::UnRelated
    then MachineT = UnrelatedMachine
        JobT = UnrelatedJob
*/

using MachineT = std::conditional_t<
    model == Model::Identical,
    IdenticalMachine,
    std::conditional_t<
        model == Model::Related,
        RelatedMachine,
        UnrelatedMachine
    >
>;

using JobT = std::conditional_t<
    model == Model::Unrelated,
    UnrelatedJob,
    NormalJob
>;
};

}