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
class GreedySchedulerUnrelatedRealTimeArrival
{
// JobHeap is a heap with a compare function based on the workload of the job
using JobHeap = std::priority_queue<NormalJob, std::vector<NormalJob>, NormalJobWorkloadCmp>;

public:
    GreedySchedulerUnrelatedRealTimeArrival(const json & config)  {}

    /**
     * @brief Initialize the machine_free_list
     */
    void initialize(int64_t num_of_machines, const std::vector<UnrelatedMachine> & machines)
    {
        assert(num_of_machines == machines.size());
        for(size_t i = 0; i < num_of_machines; i++)
        {
            machine_free_heap_.emplace(i, machines[i].processing_speed_);
        }
    }

    /**
     * @brief Schedule the jobs onto free machines and output schedule steps
     */
    std::vector<ScheduleStep> schedule(const std::vector<NormalJob> & jobs_for_this_turn,
                                       std::vector<UnrelatedMachine> & machines,
                                       int64_t timestamp)
    {   
        NANO_LOG(DEBUG, "[GreedySchedulerUnrelatedRealTimeArrival::schedule] Inside schedule");

        // push the newly coming jobs into the job heap
        // (We have to consider the new jobs along with the accmulated jobs together)
        for(auto & job : jobs_for_this_turn)
        {
            accumulated_jobs_.push(job);

            // DEBUGING
            NANO_LOG(DEBUG, "%s", job.toString().c_str());
            NANO_LOG(DEBUG, "Jobs : ");
            auto job_array = jobHeap2Vector();
            for(auto & job_for_printing : job_array)
            {
                NANO_LOG(DEBUG, "%s", job_for_printing.toString().c_str());
            }
            NANO_LOG(DEBUG, "\n");
        }
        if(machine_free_heap_.empty())
        {
            return {};
        }
        std::vector<ScheduleStep> schedule_steps;
        while((!machine_free_heap_.empty()) && (!accumulated_jobs_.empty()))
        {
            // when there's free machine and remaining jobs, we can schedule the job at the top of the heap onto any machine
            const NormalJob & current_job = accumulated_jobs_.top();
            auto & machine_state_node = machine_free_heap_.top();
            int64_t machine_id = machine_state_node.machineId_;
            int64_t processing_speed = machine_state_node.processing_speed_;
            machines[machine_id].execute(current_job.id_, (current_job.workload_ + processing_speed - 1) / processing_speed);
            schedule_steps.emplace_back(timestamp, current_job.id_, machine_id);
            machine_free_heap_.pop();
            accumulated_jobs_.pop();
            
            NANO_LOG(DEBUG, "current_job_id is %ld, machine_id is %ld", current_job.id_, machine_id);
            NANO_LOG(DEBUG, "current job : %s", current_job.toString().c_str());
        }

        NANO_LOG(DEBUG, "[GreedySchedulerUnrelatedRealTimeArrival::schedule] outside schedule");
        return schedule_steps;
    }

    /**
     * @brief Modify the remaining time of the busy machines and add the machines that has done their job
     * to the machine free list 
     */
    void updateMachineState(std::vector<UnrelatedMachine> & machines, int64_t elapsing_time)
    {
        NANO_LOG(DEBUG, "[GreedySchedulerUnrelatedRealTimeArrival::updateMachineState] Inside updateMachineState");
        NANO_LOG(DEBUG, "elapsing time : %ld", elapsing_time);
        NANO_LOG(DEBUG, "Printing the machines : ");
        for(auto & machine : machines)
        {
            NANO_LOG(DEBUG, "%s", machine.toString().c_str());
        }
        NANO_LOG(DEBUG, "\n");

        NANO_LOG(DEBUG, "Printing the machine_free_heap_");
        NANO_LOG(DEBUG, "machine_free_heap_.size() is %ld", machine_free_heap_.size());
        auto temp_heap1 = machine_free_heap_;
        while(!temp_heap1.empty())
        {
            auto & node = temp_heap1.top();
            temp_heap1.pop();
            NANO_LOG(DEBUG, "%s", node.toString().c_str());
        }
        NANO_LOG(DEBUG, "\n");

        NANO_LOG(DEBUG, "Printing the accumulated_jobs_");
        NANO_LOG(DEBUG, "accumulated_jobs_.size() is %ld", accumulated_jobs_.size());
        auto temp_heap2 = accumulated_jobs_;
        while(!temp_heap2.empty())
        {
            auto & job = temp_heap2.top();
            temp_heap2.pop();
            NANO_LOG(DEBUG, "%s", job.toString().c_str());
        }
        NANO_LOG(DEBUG, "\n");

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
                    machine_free_heap_.emplace(i, machines[i].processing_speed_);
                }
            }
        }
        is_done_ = done_flag;
        // If there's no running job on any machine, set is_done_ to true

        NANO_LOG(DEBUG, "[GreedySchedulerUnrelatedRealTimeArrival::updateMachineState] Outside updateMachineState");
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

private:
    /**
     * @brief The node for machine temp array. Includes machine id, total pending time and the corresponding processing speed.
     */
    struct MachineStateNode
    {
        int64_t machineId_;
        int64_t processing_speed_;

        MachineStateNode() = default;

        MachineStateNode(int64_t machineId, int64_t processing_speed) 
            : machineId_(machineId), processing_speed_(processing_speed) {}
        MachineStateNode(const MachineStateNode & other) = default;
        MachineStateNode(MachineStateNode && other) = default;
        MachineStateNode & operator=(const MachineStateNode & other) = default;
        MachineStateNode & operator=(MachineStateNode && other) = default;

        bool operator<(const MachineStateNode & other) const
        {
            return processing_speed_ < other.processing_speed_;
        }

        std::string toString() const
        { 
            std::stringstream ss;
            ss << "machineId : " << machineId_ << "\n";
            ss << "processing_speed : " << processing_speed_ << "\n";
            return ss.str();
        }
    };  

    JobHeap accumulated_jobs_;    // a job heap storing all accumulated jobs
    std::priority_queue<MachineStateNode> machine_free_heap_;    // store the id of all free machines
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
