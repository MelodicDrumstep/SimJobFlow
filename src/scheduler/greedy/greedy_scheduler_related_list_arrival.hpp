#pragma once

#include <vector>
#include <type_traits>
#include <cmath>
#include <queue>
#include <deque>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>

#include "basic_utils_in_one_header.hpp"
#include "NanoLogCpp17.h"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief Greedy scheduler of Related machine_model, and list arrival release model.
 */
class GreedySchedulerRelatedListArrival
{

public:
    GreedySchedulerRelatedListArrival() {}

    /**
     * @brief Initialize the machine state extended array and the machine state temp array
     * machine state extended array contains some extended information for machine state
     * such as total pending time and pendng job list
     * machine state temp array is just used for selecting the machine to schedule the job
     * each time a jobs arrives
     */
    void initialize(int64_t num_of_machines, const std::vector<RelatedMachine> & machines)
    {
        assert(num_of_machines == machines.size());
        machine_state_extended_array_.resize(num_of_machines);
        machine_state_temp_array_.resize(num_of_machines);
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
                                       std::vector<RelatedMachine> & machines,
                                       int64_t timestamp)
    {   
        NANO_LOG(DEBUG, "[GreedySchedulerRelatedListArrival::schedule] Inside schedule");

        std::vector<ScheduleStep> schedule_steps;
        size_t num_of_jobs = jobs_for_this_turn.size();
        if(num_of_jobs == 0)
        {
            return schedule_steps;
        }

        for(size_t i = 0; i < num_of_jobs; i++)
        {
            const NormalJob & job = jobs_for_this_turn[i];
            int64_t machineId = findSuitableMachine(job.workload_, machines);
            // findSuitableMachine will select the correct machine based on greedy
            MachineState & machine_state = machine_state_extended_array_[machineId];
            RelatedMachine & machine = machines[machineId];
            int64_t expected_job_running_time = (job.workload_ + machine.processing_speed_ - 1) / machine.processing_speed_;

            NANO_LOG(DEBUG, "machine : \n%s", machine.toString().c_str());
            NANO_LOG(DEBUG, "machineState : \n%s", machine_state.toString().c_str());

            if(machine.isFree())
            {
                assert(machine_state.isFree());
                // if machine is free, then it's assured that there's no pending jobs on it
                // because if so, updateMachineState will execute that pending job on this machine.
                NANO_LOG(DEBUG, "machine is free, schedule onto it");
                machine.execute(job);
            }
            else
            {
                // the machine is busy, push back the job to the pending job list
                machine_state.pending_jobs_.push_back(job);
            }
            machine_state.total_pending_time_ += expected_job_running_time;
            schedule_steps.emplace_back(timestamp, job.id_, machineId);
        }

        NANO_LOG(DEBUG, "[GreedySchedulerRelatedListArrival::schedule] Outside schedule");
        NANO_LOG(DEBUG, "After schedule, printing the machine_state_extended_array");
        for(auto & machine_state : machine_state_extended_array_)
        {
            NANO_LOG(DEBUG, "%s", machine_state.toString().c_str());
        }

        return schedule_steps;
    }

    /**
     * @brief Modify the remaining time / total pending time of the busy machines
     */
    void updateMachineState(std::vector<RelatedMachine> & machines, int64_t elapsing_time)
    {
        NANO_LOG(DEBUG, "[GreedySchedulerRelatedListArrival::updateMachineState] Inside updateMachineState");
        NANO_LOG(DEBUG, "elapsing time : %ld", elapsing_time);

        bool done_flag = true;

        for(size_t i = 0; i < machines.size(); i++)
        {
            auto & machine = machines[i];
            auto & machine_state = machine_state_extended_array_[i];

            NANO_LOG(DEBUG, "machine : \n%s", machine.toString().c_str());
            NANO_LOG(DEBUG, "machineState : \n%s", machine_state.toString().c_str());

            if(machine.remaining_time_ != Invalid_Remaining_Time)
            {
                // there's job on the machine
                // modify remaining_time and total_pending_time
                done_flag = false;
                int64_t job_executing_time = std::min(elapsing_time, machine.remaining_time_);
                machine.remaining_time_ -= job_executing_time;
                machine_state.total_pending_time_ -= job_executing_time;

                NANO_LOG(DEBUG, "job_executing_time : %ld", job_executing_time);
            }
            if(machine.remaining_time_ == 0)
            {
                if(machine_state.total_pending_time_ == 0)
                {
                    machine.setFree();
                }
                else
                {
                    // if the machine becomes free and there's pending jobs
                    // execute the top pending job on the machine
                    done_flag = false;
                    assert(machine_state.pending_jobs_.size() > 0);
                    auto & top_pending_job = machine_state.pending_jobs_.front();
                    // We must consider the processing speed of the machine
                    machine.execute(top_pending_job);
                    machine_state.pending_jobs_.pop_front();
                }
            }
        }
        is_done_ = done_flag;
        // If there's no running job on any machine and no pending jobs, set is_done_ to true

        NANO_LOG(DEBUG, "[GreedySchedulerRelatedListArrival::updateMachineState] Outside updateMachineState");
        NANO_LOG(DEBUG, "After updateMachineState, printing the machine_state_extended_array");
        for(auto & machine_state : machine_state_extended_array_)
        {
            NANO_LOG(DEBUG, "%s", machine_state.toString().c_str());
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

private:
    /**
     * @brief Extended information about the machine. Contains the total pending time and the pending job list.
     */
    struct MachineState
    {
        int64_t total_pending_time_ = 0;    // remaining time of the current job is counted in this variable
        // for job i and machine j, the time to execute the job is workload_i / processing_speed_j
        std::deque<NormalJob> pending_jobs_;

        MachineState() = default;
        MachineState(const MachineState & other) = delete;
        MachineState(MachineState && other) = default;
        MachineState & operator=(const MachineState & other) = delete;
        MachineState & operator=(MachineState && other) = default;

        bool isFree() const
        {
            return (total_pending_time_ == 0);
        }

        std::string toString() const
        { 
            std::stringstream ss;
            ss << "total_pending_time : " << total_pending_time_ << "\n";
            ss << "pending_jobs_.size() : " << pending_jobs_.size() << "\n";
            for(auto & job : pending_jobs_)
            {
                ss << job.toString() << "\n";
            }
            return ss.str();
        }
    };

    /**
     * @brief The node for machine temp array. Includes machine id, total pending time and the corresponding processing speed.
     */
    struct MachineStateNode
    {
        int64_t machineId_;
        int64_t total_pending_time_ = 0;    // remaining time of the current job is counted in this variable
        int64_t processing_speed_;

        MachineStateNode() = default;

        MachineStateNode(int64_t machineId, int64_t total_pending_time, int64_t processing_speed) 
            : machineId_(machineId), total_pending_time_(total_pending_time), processing_speed_(processing_speed) {}
        MachineStateNode(const MachineStateNode & other) = default;
        MachineStateNode(MachineStateNode && other) = default;
        MachineStateNode & operator=(const MachineStateNode & other) = default;
        MachineStateNode & operator=(MachineStateNode && other) = default;

        bool operator<(const MachineStateNode & other) const
        {
            if(total_pending_time_ != other.total_pending_time_)
            {
                return total_pending_time_ < other.total_pending_time_;
            }
            return processing_speed_ < other.processing_speed_;
        }

        bool isFree() const
        {
            return (total_pending_time_ == 0);
        }

        std::string toString() const
        { 
            std::stringstream ss;
            ss << "machineId : " << machineId_ << "\n";
            ss << "total_pending_time : " << total_pending_time_ << "\n";
            ss << "processing_speed : " << processing_speed_ << "\n";
            return ss.str();
        }
    };    

    std::vector<MachineState> machine_state_extended_array_;
    // the machine state array that can be indexed through the machine id
    std::vector<MachineStateNode> machine_state_temp_array_;
    // the temp array will be filled each time we have to select a machine to execute the certain job

    bool is_done_ = false;

    /**
     * @brief Fill the machine state temp array, and use std::min_element to get the most suitable element in O(n) time.
     */
    int64_t findSuitableMachine(int64_t workload, const std::vector<RelatedMachine> & machines)
    {
        for(size_t i = 0; i < machine_state_extended_array_.size(); i++)
        {
            auto & machine = machines[i];
            machine_state_temp_array_[i] = std::move(MachineStateNode(i, 
                machine_state_extended_array_[i].total_pending_time_ + 
                (workload + machine.processing_speed_ - 1) / machine.processing_speed_,
                machine.processing_speed_));
        }

        // DEBUGING
        #ifdef DEBUG_SCHED_RL_LIST
            std::cout << "Inside findSuitableMachine. The min element in machne state temp array is : \n";
            std::cout << std::min_element(machine_state_temp_array_.begin(), machine_state_temp_array_.end()) -> toString() << "\n";
        #endif
        // DEBUGING

        return std::min_element(machine_state_temp_array_.begin(), machine_state_temp_array_.end()) -> machineId_;
    }
};

}