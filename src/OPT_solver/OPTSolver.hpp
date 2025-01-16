#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <limits>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "permutation_generator.hpp"
#include "basic_utils_in_one_header.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief This class computes the OPT value of the given schedule jobs setting.
 * It just use brute-force enumeration to find the optimal solution.
 * 
 * @tparam machine_model @see util/model/model.hpp
 * @tparam InputHandlerT The input handler, must satisfy the 'OPTInputHandler' requirements
 * @tparam OutputHandlerT The output handler, must satisfy the 'OPTOutputHandler' requirements
 */
template <Machine_Model machine_model, OPTInputHandler<machine_model> InputHandlerT, OPTOutputHandler<machine_model> OutputHandlerT>
class OPTSolver 
{
/**
 * MachineModelTraits, which located at "src/machine_model/model_trait.hpp, 
 * contains the job type and machine type for this machine_model"
 */
using JobT = typename MachineModelTraits<machine_model>::JobT;
using MachineT = typename MachineModelTraits<machine_model>::MachineT;

public:
    OPTSolver(const json & config,
               std::unique_ptr<InputHandlerT> input_handler,
               std::unique_ptr<OutputHandlerT> output_handler) :
        num_of_machines_(config["Num_of_Machines"]),
        input_handler_(std::move(input_handler)),
        output_handler_(std::move(output_handler))
    {
        input_handler_ -> checkValidity(num_of_machines_); 
        // checkValidity will check if the number of machines matches will the configuration there.
        initializeMachines(config);
        jobs_ = input_handler_ -> getJobArray();
    }

    /**
     * @brief Start the scheduling rountine
     */
    void start()
    {
        backtrace(0, 0);
        NANO_LOG(NOTICE, "[OPTSolver::start] OPTSolver finished, OPT is %ld, schedule_step is %s", 
            best_total_timespent_, toString(best_schedule_steps_).c_str());
        output_handler_ -> output(machines_, jobs_, best_schedule_steps_);
        output_handler_ -> output(best_total_timespent_);
    }

    void backtrace(int64_t timestamp, size_t jobs_array_index)
    {   
        // DEBUGING
        backtrace_times_++;
        NANO_LOG(DEBUG, " ");
        NANO_LOG(DEBUG, "[OPTSolver::backtrace] timestamp = %ld, jobs_array_index = %ld, backtrace_times_ = %ld", 
            timestamp, jobs_array_index, backtrace_times_);
        // DEBUGING

        // No need to backtrace any further
        if(timestamp >= best_total_timespent_)
        {
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] timestamp is %ld, best_total_timespent_ is %ld,"
                 "tiemstamp >= best_total_timespent_, return directly.", timestamp, best_total_timespent_);
            return;
        }
        updateMachineState();

        // Backtrace is done
        if(jobs_array_index == jobs_.size() && accumulated_jobs_.empty())
        {
            int64_t max_remaining_time = 0;
            for(auto & machine : machines_)
            {
                max_remaining_time = std::max(max_remaining_time, machine.remaining_time_);
            }
            timestamp += max_remaining_time;
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] Reach the end. max_remaining_time is %ld, timestamp is %ld, schedule_step is %s,",
                     max_remaining_time, timestamp, toString(current_schedule_steps_).c_str());
            if(timestamp < best_total_timespent_)
            {
                best_total_timespent_ = timestamp;
                best_schedule_steps_ = current_schedule_steps_;
                NANO_LOG(DEBUG, "[OPTSolver::backtrace] Update best_total_timespent_ to %ld, best_schedule_steps_ is %s",
                         best_total_timespent_, toString(best_schedule_steps_).c_str());
            }
            return;
        }

        // emplace back the new jobs to the accumulated jobs array
        while(jobs_array_index < jobs_.size() && jobs_[jobs_array_index].timestamp_ <= timestamp)
        {
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] inside the jobs emplacing back loop. jobs_array_index is %ld", jobs_array_index);
            accumulated_jobs_.push_back(jobs_[jobs_array_index]);
            jobs_array_index++;
        }
        
        // fill in the free machine list, used for enumeration
        std::vector<int> free_machine_indexes;
        for(int i = 0; i < machines_.size(); i++)
        {
            if(machines_[i].isFree())
            {
                free_machine_indexes.push_back(i);
            }
        }
        size_t num_accumulated_jobs = accumulated_jobs_.size();
        size_t num_free_machines = free_machine_indexes.size();
        size_t max_num_job_scheduled = std::min(num_free_machines, num_accumulated_jobs);

        NANO_LOG(DEBUG, "[OPTSolver::backtrace] num_accumulated_jobs is %ld, num_free_machines is %ld, max_num_job_scheduled is %ld",
            num_accumulated_jobs, num_free_machines, max_num_job_scheduled);

        auto machines_backup = machines_;
        auto accumulated_jobs_backup = accumulated_jobs_;

        // Firstly enumerate the number of jobs to be scheduled in this turn
        for(int num_job_scheduled = 0; num_job_scheduled <= max_num_job_scheduled; num_job_scheduled++)
        {
            // DEBUGING
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] num_job_scheduled is %d",
                num_job_scheduled);
            // DEBUGING
            auto job_schedule_combination = PermutationGenerator(num_accumulated_jobs, num_job_scheduled).generatePermutations();
            auto machine_schedule_combination = PermutationGenerator(num_free_machines, num_job_scheduled).generatePermutations();
            
            // Then enumerate the combination of jobs to be scheduled in this turn
            for(auto & job_scheduled_indexes : job_schedule_combination)
            {
                // And enumerate the combination of machines to be scheduled in this turn
                for(auto & machine_scheduled_indexes : machine_schedule_combination)
                {  
                    // schedule it!
                    NANO_LOG(DEBUG, "[OPTSolver::backtrace] job_scheduled_indexes: %s, machine_scheduled_indexes: %s", 
                        toString(job_scheduled_indexes).c_str(), toString(machine_scheduled_indexes).c_str());

                    for(int i = 0; i < num_job_scheduled; i++)
                    {
                        auto & machine = machines_[free_machine_indexes[machine_scheduled_indexes[i]]];
                        auto job = accumulated_jobs_[job_scheduled_indexes[i]];
                        machine.execute(job.id_, job.workload_);
                        // execute the job on the machine
                        current_schedule_steps_.emplace_back(timestamp, job.id_, machine.machineId_); 
                        // push the job to the schedule step list
                        accumulated_jobs_.erase(accumulated_jobs_.begin() + job_scheduled_indexes[i]); 
                        // erase the job from the accumulated_jobs_ list
                        backtrace(timestamp + 1, jobs_array_index);
                        
                        current_schedule_steps_.pop_back(); // pop the last step
                        // restore the state of machines_ and accumulated_jobs_. 
                        // This can also be done by passing them as a parameter to the function
                        // I do not consider too much about it here, maybe potentially it can be optimized
                        machines_ = machines_backup;
                        accumulated_jobs_ = accumulated_jobs_backup; 
                        if(accumulated_jobs_.size() != num_accumulated_jobs)
                        {
                            NANO_LOG(ERROR, "[OPTSolver::backtrace] accumulated_jobs_.size() != num_accumulated_jobs, accumulated_jobs_ = %s", 
                                toString(accumulated_jobs_).c_str());
                        }
                    }
                }
            }
        }

        backtrace(timestamp + 1, jobs_array_index); // The last case : schedule nothing in this turn
        NANO_LOG(DEBUG, "[OPTSolver::backtrace] End backtrace(%ld, %ld)", timestamp, jobs_array_index);
    }

private:
    std::unique_ptr<InputHandlerT> input_handler_;
    std::unique_ptr<OutputHandlerT> output_handler_;

    std::vector<MachineT> machines_;     // all machines
    std::vector<JobT> jobs_;             // all jobs. Store all the jobs here for indexing by jobId 
    std::vector<JobT> accumulated_jobs_; 
    int64_t num_of_machines_; 
    std::vector<ScheduleStep> current_schedule_steps_; 
    std::vector<ScheduleStep> best_schedule_steps_; 
    int64_t best_total_timespent_ = std::numeric_limits<int64_t>::max();
    int64_t backtrace_times_ = 0; // the number of times the backtrace function is called, used for logging

    /**
     * @brief Initialize the machine array.
     */
    void initializeMachines(const json & config)
    {  
        NANO_LOG(DEBUG, "[OPTSolver::initializeMachines]");

        machines_.reserve(num_of_machines_);
        if constexpr (machine_model == Machine_Model::Related)
        {
            // If the machine_model is Related, there's a processing speed field for the machine type
            // configure it using the field in json object
            std::vector<int64_t> processing_speed = config["Processing_Speed"].get<std::vector<int64_t>>();
            if(processing_speed.size() != num_of_machines_)
            {
                throw std::runtime_error("The number of processing speed is not equal to the number of machines");
            }
            for(int i = 0; i < num_of_machines_; i++)
            {
                machines_.emplace_back(i, processing_speed[i]);
            }
        }
        else
        {
            for(int i = 0; i < num_of_machines_; i++)
            {
                machines_.emplace_back(i);
            }
        }
    }

    /**
     * @brief Update the state of machines for 1 time step
     */
    void updateMachineState()
    {
        if constexpr (machine_model == Machine_Model::Identical)
        {
            for(auto & machine : machines_)
            {
                if(!machine.isFree())
                {
                    if(--machine.remaining_time_ == 0)
                    {
                        machine.setFree();
                    }
                }
            }
        }
        else if(machine_model == Machine_Model::Related)
        {
            // TODO: 
        }
        else
        {
            // TODO: 
        }
    }
};

}