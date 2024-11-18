#pragma once

#include <vector>
#include <type_traits>
#include <cmath>
#include <queue>
#include <nlohmann/json.hpp>
#include <iostream>

#include "model_trait.hpp"
#include "schedule_step.hpp"
#include "job.hpp"
#include "machine.hpp"

namespace SJF
{

#define DEBUG_SCHED_ID

using json = nlohmann::json;

/**
 * @brief Greedy scheduler of Identical machine_model, and real time arrival release model.
 */
class GreedySchedulerRelatedRealTimeArrival
{
// JobHeap is a heap with a compare function based on the workload of the job
using JobHeap = std::priority_queue<NormalJob, std::vector<NormalJob>, NormalJobWorkloadCmp>;

public:
    GreedySchedulerRelatedRealTimeArrival(const json & config)  {}

    /**
     * @brief Initialize the machine_free_list
     */
    void initialize(int64_t num_of_machines)
    {
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
        // DEBUGING
        #ifdef DEBUG_SCHED_ID
            std::cout << "Inside GreedySchedulerRelated::schedule()\n";
        #endif
        // DEBUGING

        // push the newly coming jobs into the job heap
        // (We have to consider the new jobs along with the accmulated jobs together)
        for(auto & job : jobs_for_this_turn)
        {
            accumulated_jobs_.push(job);

            // DEBUGING
            #ifdef DEBUG_SCHED_ID
                std::cout << job.toString() << std::endl;
                std::cout << "Jobs : \n";
                auto job_array = jobHeap2Vector();
                for(auto & job_for_printing : job_array)
                {
                    std::cout << job_for_printing.toString() << std::endl;
                }
                std::cout << std::endl;
            #endif
            // DEBUGING
        }
        if(machine_free_list_.empty())
        {
            return {};
        }
        std::vector<ScheduleStep> schedule_steps;
        while((!machine_free_list_.empty()) && (!accumulated_jobs_.empty()))
        {
            // when there's free machine and remaining jobs, we can schedule the job at the top of the heap onto any machine
            const NormalJob & current_job = accumulated_jobs_.top();
            int64_t machine_id = machine_free_list_.back();
            machine_free_list_.pop_back();
            machines[machine_id].execute(current_job.id_, current_job.workload_);
            schedule_steps.emplace_back(timestamp, current_job.id_, machine_id);
            accumulated_jobs_.pop();

            // DEBUGING
            #ifdef DEBUG_SCHED_ID
                std::cout << "current_job_id is " << current_job.id_ << ", and machine_id is " << machine_id << std::endl;
                std::cout << "current job :" << current_job.toString() << std::endl;
            #endif
            // DEBUGING
        }
        return schedule_steps;
    }

    /**
     * @brief Modify the remaining time of the busy machines and add the machines that has done their job
     * to the machine free list 
     */
    void updateMachineState(std::vector<IdenticalMachine> & machines, int64_t elapsing_time)
    {
        for(size_t i = 0; i < machines.size(); i++)
        {
            auto & machine = machines[i];
            bool done_flag = true;
            if(machine.remaining_time_ != Invalid_Remaining_Time)
            {
                done_flag = false;
                machine.remaining_time_ = std::max(static_cast<int64_t>(0), machine.remaining_time_ - elapsing_time);
                if(machine.remaining_time_ == 0)
                {
                    machine.setFree();
                    machine_free_list_.push_back(i);
                }
            }
            is_done_ = done_flag;
            // If there's no running job on any machine, set is_done_ to true
        }
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
            // DEBUGING
            #ifdef DEBUG_SCHED_ID
                std::cout << "heap size is " << accumulated_jobs_.size() << std::endl;
            #endif
            // DEBUGING

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