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

template <Machine_Model machine_model, OPTInputHandler<machine_model> InputHandlerT, OPTOutputHandler<machine_model> OutputHandlerT>
class OPTSolver 
{
/**
 * MachineModelTraits, which located at "src/machine_model/model_trait.hpp, contains the job type and machine type for this machine_model"
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
        NANO_LOG(DEBUG, "Before emplacing back jobs, accumulated_jobs_ is %s", toString(accumulated_jobs_).c_str());
        NANO_LOG(DEBUG, "Before emplacing back jobs, machines_ is %s", toString(machines_).c_str());
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
            }
            return;
        }

        // TODO: emplace back the new jobs to the accumulated jobs array
        while(jobs_array_index < jobs_.size() && jobs_[jobs_array_index].timestamp_ <= timestamp)
        {
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] inside the jobs emplacing back loop. jobs_array_index is %ld", jobs_array_index);
            accumulated_jobs_.push_back(jobs_[jobs_array_index]);
            jobs_array_index++;
        }

        // DEBUGING
        NANO_LOG(DEBUG, "[OPTSolver::backtrace] After emplacing back jobs, accumulated_jobs_ is %s", 
            toString(accumulated_jobs_).c_str());
        // DEBUING
        
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

        // DEBUGING
        auto machines_backup = machines_;
        auto accumulated_jobs_backup = accumulated_jobs_;
        // DEBUGING

        for(int num_job_scheduled = 0; num_job_scheduled <= max_num_job_scheduled; num_job_scheduled++)
        {
            auto job_schedule_combination = PermutationGenerator(num_accumulated_jobs, num_job_scheduled).generatePermutations();
            auto machine_schedule_combination = PermutationGenerator(num_free_machines, num_job_scheduled).generatePermutations();

            // enumerate the number of jobs to be schedule this turn
            for(auto & job_scheduled_indexes : job_schedule_combination)
            {
                for(auto & machine_scheduled_indexes : machine_schedule_combination)
                {            
                    // schedule it!
                    // DEBUGING
                    NANO_LOG(DEBUG, "[OPTSolver::backtrace] job_scheduled_indexes: %s, machine_scheduled_indexes: %s", 
                        toString(job_scheduled_indexes).c_str(), toString(machine_scheduled_indexes).c_str());
                    // DEBUGING 

                    for(int i = 0; i < num_job_scheduled; i++)
                    {
                        machines_backup_ = machines_;
                        accumulated_jobs_backup_ = accumulated_jobs_;

                        auto & machine = machines_[free_machine_indexes[machine_scheduled_indexes[i]]];
                        auto job = accumulated_jobs_[job_scheduled_indexes[i]];
                        NANO_LOG(DEBUG, "[OPTSolver::backtrace] Inside the inner loop, i is %d, job: %s, machine: %s",
                            i, job.toString().c_str(), machine.toString().c_str());
                        machine.execute(job.id_, job.workload_);

                        NANO_LOG(DEBUG, "[OPTSolver::backtrace] Before cleaning up, accumulated_jobs_: %s", 
                            toString(accumulated_jobs_).c_str());
                        current_schedule_steps_.emplace_back(timestamp, job.id_, machine.machineId_);
                        accumulated_jobs_.erase(accumulated_jobs_.begin() + job_scheduled_indexes[i]);
                        free_machine_indexes.erase(free_machine_indexes.begin() + machine_scheduled_indexes[i]);
                        NANO_LOG(DEBUG, "[OPTSolver::backtrace] After cleaning up, accumulated_jobs_: %s", 
                            toString(accumulated_jobs_).c_str());
                        NANO_LOG(DEBUG, "[OPTSolver::backtrace] job \"%s\" scheduled on machine \"%s\"", job.toString().c_str(), machine.toString().c_str());
                        
                        backtrace(timestamp + 1, jobs_array_index);
                        
                        free_machine_indexes.insert(free_machine_indexes.begin() + machine_scheduled_indexes[i], machine.machineId_);
                        accumulated_jobs_.insert(accumulated_jobs_.begin() + job_scheduled_indexes[i], job);
                        current_schedule_steps_.pop_back();
                        machine.setFree();
                        NANO_LOG(DEBUG, "[OPTSolver::backtrace] timestamp = %ld,  Finish backtrace(%ld, %ld), job \"%s\" unscheduled from machine \"%s\"", 
                            timestamp, timestamp + 1, jobs_array_index, job.toString().c_str(), machine.toString().c_str());

                        // // DEBUGING, check if machines and accumulated_jobs are both unchanged
                        // if((machines_ != machines_backup) || (accumulated_jobs_ != accumulated_jobs_backup))
                        // {
                        //     NANO_LOG(ERROR, "[OPTSolver::backtrace] machines_ or accumulated_jobs_ changed during backtrace");
                        //     NANO_LOG(DEBUG, "[OPTSolver::backtrace] machines_ = %s", toString(machines_).c_str());
                        //     NANO_LOG(DEBUG, "[OPTSolver::backtrace] machines_backup = %s", toString(machines_backup).c_str());
                        //     NANO_LOG(DEBUG, "[OPTSolver::backtrace] accumulated_jobs_ = %s", toString(accumulated_jobs_).c_str());
                        //     NANO_LOG(DEBUG, "[OPTSolver::backtrace] accumulated_jobs_backup = %s", toString(accumulated_jobs_backup).c_str());
                        //     exit(1);
                        // }

                        machines_ = machines_backup;
                        accumulated_jobs_ = accumulated_jobs_backup;
                        // DEBUGING
                    }
                }
            }
        }

        backtrace(timestamp + 1, jobs_array_index); // schedule nothing in this turn

        // // DEBUGING
        // NANO_LOG(DEBUG, "[OPTSolver::backtrace] After backtrace: timestamp = %ld", timestamp);
        // NANO_LOG(DEBUG, "[OPTSolver::backtrace] After backtrace: accumulated_jobs_ is %s", toString(accumulated_jobs_).c_str());
        // NANO_LOG(DEBUG, "[OPTSolver::backtrace] After backtrace: machines_ is %s", toString(machines_).c_str());
        // NANO_LOG(DEBUG, "[OPTSolver::backtrace] After backtrace: current_schedule_steps_ is %s", toString(current_schedule_steps_).c_str());
        NANO_LOG(DEBUG, "[OPTSolver::backtrace] End backtrace(%ld, %ld)", timestamp, jobs_array_index);
    }

private:
    std::unique_ptr<InputHandlerT> input_handler_;
    std::unique_ptr<OutputHandlerT> output_handler_;

    std::vector<MachineT> machines_;    // all machines
    std::vector<JobT> jobs_;            // all jobs. Store all the jobs here for indexing by jobId 
    std::vector<JobT> accumulated_jobs_;
    size_t last_accumulated_job_index_ = -1;
    std::vector<MachineT> machines_backup_;
    std::vector<JobT> accumulated_jobs_backup_;
    int64_t num_of_machines_;
    std::vector<ScheduleStep> current_schedule_steps_;
    std::vector<ScheduleStep> best_schedule_steps_;
    int64_t best_total_timespent_ = std::numeric_limits<int64_t>::max();
    int64_t backtrace_times_ = 0;

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