#pragma once

#include <vector>
#include <type_traits>
#include <cmath>
#include <queue>

#include "model_trait.hpp"
#include "schedule_step.hpp"
#include "job.hpp"
#include "machine.hpp"

namespace SJF
{

class GreedySchedulerRelated
{

public:
    void initialize(int64_t num_of_machines)
    {

    }

    std::vector<ScheduleStep> schedule(const std::vector<NormalJob> & jobs_for_this_turn,
                                       const std::vector<RelatedMachine> & machines,
                                       int64_t timestamp)
    {
        // TODO: implement it
    }

    void maintainMachineState(std::vector<RelatedMachine> & machines, int64_t elapsing_time)
    {
        // TODO: implemen it
    }

private:
    std::vector<NormalJob> accumulated_jobs_;
};

}