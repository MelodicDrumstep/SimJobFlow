#pragma once

#include <vector>
#include <type_traits>
#include <cmath>
#include <queue>
#include <deque>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>

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
class GreedySchedulerIdenticalListArrival
{

public:
    GreedySchedulerIdenticalListArrival(const json & config) {}

    /**
     * @brief Initialize the machine_free_list
     */
    void initialize(int64_t num_of_machines)
    {
        machine_state_extended_.reserve(num_of_machines);
        for(size_t i = 0; i < num_of_machines; i++)
        {
            machine_state_extended_.emplace_back(i);
        }
    }

    /**
     * @brief Schedule the jobs onto free machines and output schedule steps
     * 
     * @param jobs_for_this_turn It's assured that the jobs is sorted by the operator<, i.e.
     *      bool operator<(const NormalJob & other) const
            {
                return (timestamp_ < other.timestamp_) || (timestamp_ == other.timestamp_ && workload_ > other.workload_);
            }
     */
    std::vector<ScheduleStep> schedule(const std::vector<NormalJob> & jobs_for_this_turn,
                                       std::vector<IdenticalMachine> & machines,
                                       int64_t timestamp)
    {   
        // DEBUGING
        #ifdef DEBUG_SCHED_ID
            std::cout << "Inside GreedySchedulerIdentical::schedule()\n";
        #endif
        // DEBUGING

        std::vector<ScheduleStep> schedule_steps;
        size_t num_of_jobs = jobs_for_this_turn.size();
        std::partial_sort(machine_state_extended_.begin(), machine_state_extended_.begin() + num_of_jobs, machine_state_extended_.end());
        for(size_t i = 0; i < num_of_jobs; i++)
        {
            const NormalJob & job = jobs_for_this_turn[i];
            MachineNode & machine_node = machine_state_extended_[i];
            int64_t machineId = machine_node.machineId_;
            IdenticalMachine & machine = machines[machineId];
            machine_node.total_pending_time_ += job.workload_;
            auto & pending_jobs = machine_node.pending_jobs_;
            pending_jobs.push_back(job);
            if(machine.isFree())
            {
                const NormalJob & top_pending_job = pending_jobs.front();
                pending_jobs.pop_front();
                machine.execute(top_pending_job.id_, top_pending_job.workload_);
                machine_node.total_pending_time_ -= top_pending_job.workload_;
            }
            schedule_steps.emplace_back(timestamp, job.id_, machineId);
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
            if(machine.remaining_time_ != Invalid_Remaining_Time)
            {
                machine.remaining_time_ = std::max(static_cast<int64_t>(0), machine.remaining_time_ - elapsing_time);
                if(machine.remaining_time_ == 0)
                {
                    machine.setFree();
                }
            }
        }
    }

    /**
     * @brief For debug only
     */

private:
    struct MachineNode
    {
        int64_t machineId_;
        int64_t total_pending_time_ = 0;    // remaining time of the current job is counted in this variable
        std::deque<NormalJob> pending_jobs_;

        MachineNode(int64_t machineId) : machineId_(machineId) {}
        MachineNode(const MachineNode & other) = delete;
        MachineNode(MachineNode && other) = default;
        MachineNode & operator=(const MachineNode & other) = delete;
        MachineNode & operator=(MachineNode && other) = default;

        bool operator<(const MachineNode & other) const
        {
            return total_pending_time_ < other.total_pending_time_;
        }

        bool isFree() const
        {
            return (total_pending_time_ == 0);
        }
    };

    std::vector<MachineNode> machine_state_extended_;
};

}