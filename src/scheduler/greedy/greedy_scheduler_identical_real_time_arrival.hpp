#pragma once

#include <vector>
#include <type_traits>
#include <cmath>
#include <queue>
#include <nlohmann/json.hpp>
#include <iostream>

#include "basic_utils_in_one_header.hpp"
#include "NanoLogCpp17.h"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief Greedy scheduler of Identical machine_model, and real time arrival release model.
 */
class GreedySchedulerIdenticalRealTimeArrival
{
// JobHeap is a heap with a compare function based on the workload of the job
using JobHeap = std::priority_queue<NormalJob, std::vector<NormalJob>, NormalJobWorkloadCmp>;

public:
    GreedySchedulerIdenticalRealTimeArrival(const json & config)  {}

    /**
     * @brief Initialize the machine_free_list
     */
    void initialize(int64_t num_of_machines, const std::vector<IdenticalMachine> & machines)
    {
        assert(num_of_machines == machines.size());
        machine_free_list_.reserve(num_of_machines);
        for(size_t i = 0; i < num_of_machines; i++)
        {
            machine_free_list_.push_back(i);
        }
    }

    /**
     * @brief Schedule the jobs onto free machines and output schedule steps
     */
    std::vector<ScheduleStep> schedule(const std::vector<NormalJob> & jobs_for_this_turn,
                                       std::vector<IdenticalMachine> & machines,
                                       int64_t timestamp)
    {   
        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalRealTimeArrival::schedule] Inside schedule");

        // push the newly coming jobs into the job heap
        // (We have to consider the new jobs along with the accmulated jobs together)
        for(auto & job : jobs_for_this_turn)
        {
            accumulated_jobs_.push(job);

            NANO_LOG(DEBUG, "%s", job.toString().c_str());
            NANO_LOG(DEBUG, "Jobs : ");
            auto job_array = jobHeap2Vector();
            for(auto & job_for_printing : job_array)
            {
                NANO_LOG(DEBUG, "%s", job_for_printing.toString().c_str());
            }
            NANO_LOG(DEBUG, "\n");
        }
        if(machine_free_list_.empty())
        {
            return {};
        }
        std::vector<ScheduleStep> schedule_steps;
        while((!machine_free_list_.empty()) && (!accumulated_jobs_.empty()))
        {
            NANO_LOG(DEBUG, "Inside schedule while loop");

            // when there's free machine and remaining jobs, we can schedule the job at the top of the heap onto any machine
            const NormalJob & current_job = accumulated_jobs_.top();
            int64_t machine_id = machine_free_list_.back();
            machines[machine_id].execute(current_job.id_, current_job.workload_);
            schedule_steps.emplace_back(timestamp, current_job.id_, machine_id);
            NANO_LOG(DEBUG, "current_job_id is %ld, and machine_id is %ld", current_job.id_, machine_id);
            NANO_LOG(DEBUG, "current job : %s", current_job.toString().c_str());
            machine_free_list_.pop_back();
            accumulated_jobs_.pop();
        }

        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalRealTimeArrival::schedule] Outside schedule");
        return schedule_steps;
    }

    /**
     * @brief Modify the remaining time of the busy machines and add the machines that has done their job
     * to the machine free list 
     */
    void updateMachineState(std::vector<IdenticalMachine> & machines, int64_t elapsing_time)
    {
        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalRealTimeArrival::updateMachineState] Inside updateMachineState");
        NANO_LOG(DEBUG, "elapsing time : %ld", elapsing_time);

        bool done_flag = accumulated_jobs_.empty();
        for(size_t i = 0; i < machines.size(); i++)
        {
            auto & machine = machines[i];
            if(machine.remaining_time_ != Invalid_Remaining_Time)
            {
                done_flag = false;
                machine.remaining_time_ = std::max(static_cast<int64_t>(0), machine.remaining_time_ - elapsing_time);
                if(machine.remaining_time_ == 0)
                {
                    machine.setFree();
                    machine_free_list_.push_back(i);
                    NANO_LOG(DEBUG, "machines[%ld] is free now. Add it to the free list.", i);
                }
            }
        }
        is_done_ = done_flag;
        // If there's no running job on any machine, set is_done_ to true

        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalRealTimeArrival::updateMachineState] Outside updateMachineState");
    }

    /**
     * @brief Check if there's no more jobs.
     * 
     * @return true is there's no more jobs, false otherwise.
     */
    bool done() const
    {
        return is_done_;
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
    JobHeap accumulated_jobs_;    // a job heap storing all accumulated jobs
    std::vector<int64_t> machine_free_list_;    // store the id of all free machines
    bool is_done_ = false;  

    /**
     * @brief For debug only.
     */
    std::vector<NormalJob> jobHeap2Vector()
    {
        std::vector<NormalJob> jobs;
        while(!accumulated_jobs_.empty())
        {
            NANO_LOG(DEBUG, "heap size is %ld", accumulated_jobs_.size());
            jobs.push_back(accumulated_jobs_.top());
            accumulated_jobs_.pop();
        }
        for(auto & job : jobs)
        {
            accumulated_jobs_.push(job);
        }

        return jobs;
    }
};

}
