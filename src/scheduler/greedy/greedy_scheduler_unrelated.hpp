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

class GreedySchedulerUnrelated
{

public:
    void initialize(int64_t num_of_machines)
    {

    }

    std::vector<ScheduleStep> schedule(const std::vector<UnrelatedJob> & jobs_for_this_turn,
                                       const std::vector<UnrelatedMachine> & machines,
                                       int64_t timestamp)
    {
        // TODO: implement it
    }

    void maintainMachineState(std::vector<UnrelatedMachine> & machines, int64_t elapsing_time)
    {
        // TODO: implement it
    }

private:
    std::vector<UnrelatedJob> accumulated_jobs_;
};

}