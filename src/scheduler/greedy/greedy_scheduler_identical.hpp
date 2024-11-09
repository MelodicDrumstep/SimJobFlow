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

class GreedySchedulerIdentical
{
using JobHeap = std::priority_queue<NormalJob, std::vector<NormalJob>, NormalJobWorkloadCmp>;

public:
    void initialize(int64_t num_of_machines)
    {
        machine_free_list_.reserve(num_of_machines);
        for(size_t i = 0; i < num_of_machines; i++)
        {
            machine_free_list_.push_back(i);
        }
    }

    std::vector<ScheduleStep> schedule(const std::vector<NormalJob> & jobs_for_this_turn,
                                       std::vector<IdenticalMachine> & machines,
                                       int64_t timestamp)
    {   
        for(auto & job : jobs_for_this_turn)
        {
            accumulated_jobs_.push(job);
        }
        if(machine_free_list_.empty())
        {
            return {};
        }
        std::vector<ScheduleStep> schedule_steps;
        while((!machine_free_list_.empty()) && (!accumulated_jobs_.empty()))
        {
            const NormalJob & current_job = accumulated_jobs_.top();
            accumulated_jobs_.pop();
            int64_t machine_id = machine_free_list_.back();
            machine_free_list_.pop_back();
            machines[machine_id].execute(current_job.id_, current_job.workload_);
            schedule_steps.emplace_back(timestamp, current_job.id_, machine_id);
        }
        return schedule_steps;
    }

    void maintainMachineState(std::vector<IdenticalMachine> & machines, int64_t elapsing_time)
    {
        for(size_t i = 0; i < machines.size(); i++)
        {
            auto & machine = machines[i];
            if(machine.remaining_time_ != Invalid_Remaining_Time)
            {
                machine.remaining_time_ = std::max(static_cast<int64_t>(0), machine.remaining_time_ - elapsing_time);
                if(machine.remaining_time_ == 0)
                {
                    machine.setFree();
                    machine_free_list_.push_back(i);
                }
            }
        }
    }

    /**
     * @brief For debug only
     */
    const JobHeap & getAccumulatedJobs() const
    {
        return accumulated_jobs_;
    }

    const std::vector<int64_t> & getMachineFreeList() const
    {
        return machine_free_list_;
    }

private:
    JobHeap accumulated_jobs_;
    std::vector<int64_t> machine_free_list_;
};

}