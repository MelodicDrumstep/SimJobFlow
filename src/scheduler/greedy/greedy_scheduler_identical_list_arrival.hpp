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

namespace SJF
{
using json = nlohmann::json;

/**
 * @brief Greedy scheduler of Identical machine_model, and list arrival release model.
 */
class GreedySchedulerIdenticalListArrival
{

public:
    GreedySchedulerIdenticalListArrival(const json & config) {}

    /**
     * @brief Initialize the machine state array and the machine state heap
     * machine state heap is represented as a std::vector
     *  And we will use std::make_heap / std::push_heap / std::pop_heap to manipulate it
     */
    void initialize(int64_t num_of_machines, const std::vector<IdenticalMachine> & machines)
    {
        assert(num_of_machines == machines.size());
        machine_state_extended_array_.resize(num_of_machines);
        machine_state_heap_.resize(num_of_machines);
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
        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalListArrival::schedule] Inside schedule");

        std::vector<ScheduleStep> schedule_steps;
        size_t num_of_jobs = jobs_for_this_turn.size();
        if(num_of_jobs == 0)
        {
            return schedule_steps;
        }

        NANO_LOG(DEBUG, "Printing jobs_for_this_turn:");
        for(auto & job : jobs_for_this_turn)
        {
            NANO_LOG(DEBUG, "%s", job.toString().c_str());
        }

        fillMachineStateHeap();
        // It will copy the content from machine state array to machine state heap and heapify it

        for(size_t i = 0; i < num_of_jobs; i++)
        {
            NormalJob job = jobs_for_this_turn[i];
            int64_t machineId = machine_state_heap_[0].machineId_;
            // machine state heap[0] is the top of the heap
            // i.e. take out the machine with the least total pending time
            MachineState & machine_state = machine_state_extended_array_[machineId];
            IdenticalMachine & machine = machines[machineId];

            NANO_LOG(DEBUG, "machineId is %ld", machineId);
            NANO_LOG(DEBUG, "machine : %s", machine.toString().c_str());
            NANO_LOG(DEBUG, "machineState : %s", machine_state.toString().c_str());

            if(machine.isFree())
            {
                assert(machine_state.isFree());
                machine.execute(job.id_, job.workload_);
                NANO_LOG(DEBUG, "machine is free, schedule onto it. jobId is %ld, workload is %ld", job.id_, job.workload_);
            }
            else
            {
                machine_state.pending_jobs_.push_back(job);
            }
            machine_state.total_pending_time_ += job.workload_;

            std::pop_heap(machine_state_heap_.begin(), machine_state_heap_.end(), std::greater<MachineStateNode>());
            machine_state_heap_.back() = MachineStateNode(machineId, machine_state.total_pending_time_);
            std::push_heap(machine_state_heap_.begin(), machine_state_heap_.end(), std::greater<MachineStateNode>());

            schedule_steps.emplace_back(timestamp, job.id_, machineId);
        }

        NANO_LOG(DEBUG, "Printing the machines : ");
        for(auto & machine : machines)
        {
            NANO_LOG(DEBUG, "%s", machine.toString().c_str());
        }
        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalListArrival::schedule] Outside schedule");
        return schedule_steps;
    }

    /**
     * @brief Modify the remaining time / total pending time of the busy machines
     */
    void updateMachineState(std::vector<IdenticalMachine> & machines, int64_t elapsing_time)
    {
        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalListArrival::updateMachineState] Inside updateMachineState");
        NANO_LOG(DEBUG, "elapsing time : %ld", elapsing_time);

        bool done_flag = true;

        for(size_t i = 0; i < machines.size(); i++)
        {
            auto & machine = machines[i];
            auto & machine_state = machine_state_extended_array_[i];

            NANO_LOG(DEBUG, "machine : %s", machine.toString().c_str());
            NANO_LOG(DEBUG, "machineState : %s", machine_state.toString().c_str());

            if(machine.remaining_time_ != Invalid_Remaining_Time)
            {
                done_flag = false;
                int64_t job_executing_time = std::min(elapsing_time, machine.remaining_time_);
                machine.remaining_time_ -= job_executing_time;
                machine_state.total_pending_time_ -= job_executing_time;

                NANO_LOG(DEBUG, "machine is running");
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
                    done_flag = false;
                    assert(machine_state.pending_jobs_.size() > 0);
                    auto & top_pending_job = machine_state.pending_jobs_.front();
                    machine.execute(top_pending_job.id_, top_pending_job.workload_);

                    NANO_LOG(DEBUG, "schedule the pending job onto the machine.");
                    NANO_LOG(DEBUG, "job id : %ld and job workload : %ld", top_pending_job.id_, top_pending_job.workload_);

                    machine_state.pending_jobs_.pop_front();
                }
            }
        }
        is_done_ = done_flag;

        NANO_LOG(DEBUG, "[GreedySchedulerIdenticalListArrival::updateMachineState] Outside updateMachineState");
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
            for(auto & job : pending_jobs_)
            {
                ss << job.toString() << "\n";
            }
            return ss.str();
        }
    };

    /**
     * @brief The heap node for MachineState. Includes machine id and total pending time.
     *  It saves memory in heap because it does not store the pending job list.
     */
    struct MachineStateNode
    {
        int64_t machineId_;
        int64_t total_pending_time_ = 0;    // remaining time of the current job is counted in this variable

        MachineStateNode() = default;

        MachineStateNode(int64_t machineId, int64_t total_pending_time) 
            : machineId_(machineId), total_pending_time_(total_pending_time) {}
        MachineStateNode(const MachineStateNode & other) = default;
        MachineStateNode(MachineStateNode && other) = default;
        MachineStateNode & operator=(const MachineStateNode & other) = default;
        MachineStateNode & operator=(MachineStateNode && other) = default;

        bool operator<(const MachineStateNode & other) const
        {
            return total_pending_time_ < other.total_pending_time_;
        }

        bool operator>(const MachineStateNode & other) const
        {
            return total_pending_time_ > other.total_pending_time_;
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
            return ss.str();
        }
    };    

    std::vector<MachineState> machine_state_extended_array_;
    std::vector<MachineStateNode> machine_state_heap_;

    bool is_done_ = false;

    /**
     * @brief Copy the content from the machine state array to the machine state heap, and heapify it
     */
    void fillMachineStateHeap()
    {
        for(size_t i = 0; i < machine_state_extended_array_.size(); i++)
        {
            machine_state_heap_[i] = std::move(MachineStateNode(i, machine_state_extended_array_[i].total_pending_time_));
        }
        std::make_heap(machine_state_heap_.begin(), machine_state_heap_.end(), std::greater<MachineStateNode>());
    }
};

}
