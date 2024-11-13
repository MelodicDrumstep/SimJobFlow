#pragma once

#include <type_traits>

#include "model.hpp"
#include "job.hpp"
#include "machine.hpp"

namespace SJF
{

/**
 * @brief The traits of the machine_model, mapping machine_model type to job type and machine type.
 */
template <Machine_Model machine_model>
struct MachineModelTraits
{
/*
 if machine_model == Machine_Model::Indentical
    then MachineT = IndenticalMachine
        JobT = NormalJob
 else if machine_model == Machine_Model::Related
    then MachineT = RelatedMachine
        JobT = NormalJob
 else if machine_model == Machine_Model::UnRelated
    then MachineT = UnrelatedMachine
        JobT = UnrelatedJob
*/

using MachineT = std::conditional_t<
    machine_model == Machine_Model::Identical,
    IdenticalMachine,
    std::conditional_t<
        machine_model == Machine_Model::Related,
        RelatedMachine,
        UnrelatedMachine
    >
>;

using JobT = std::conditional_t<
    machine_model == Machine_Model::Unrelated,
    UnrelatedJob,
    NormalJob
>;
};

}