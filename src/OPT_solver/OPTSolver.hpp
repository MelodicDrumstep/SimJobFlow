#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "basic_utils_in_one_header.hpp"

namespace SJF
{

using json = nlohmann::json;

template <Machine_Model machine_model, OPTInputHandler<machine_model> InputHandlerT, OutputHandler<machine_model> OutputHandlerT>
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
    }

    void backtrace(int64_t timestamp, size_t jobs_array_index)
    {   
        std::string accumulated_jobs_string;
        for(auto & job : accumulated_jobs_)
        {
            accumulated_jobs_string += job.toString() + "\t";
        }
        NANO_LOG(DEBUG, "[OPTSolver::backtrace] timestamp = %ld, jobs_array_index = %ld, accumulated_jobs_string is %s", 
            timestamp, jobs_array_index, accumulated_jobs_string.c_str());
        // No need to backtrace any further
        if(timestamp >= best_total_timespent_)
        {
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] tiemstamp >= best_total_timespent_, return directly.");
            return;
        }
        // Backtrace is done
        if((jobs_array_index == jobs_.size()) && done())
        {
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] Reach the end.");
            if((best_total_timespent_ == INVALID_TOTAL_TIMESPENT) || 
               (timestamp < best_total_timespent_))
            {
                best_total_timespent_ = timestamp;
                best_scheduler_steps_ = current_scheduler_steps_;
            }
            return;
        }
        // updateMachineState();
        // machines_backup_ = machines_;
        // accumulated_jobs_backup_ = accumulated_jobs_;

        // TODO: emplace back the new jobs to the accumulated jobs array
        while(jobs_array_index < jobs_.size() && jobs_[jobs_array_index].timestamp_ <= timestamp)
        {
            NANO_LOG(DEBUG, "[OPTSolver::backtrace] inside the jobs emplacing back loop. jobs_array_index is %ld", jobs_array_index);
            accumulated_jobs_.push_back(jobs_[jobs_array_index]);
            jobs_array_index++;
        }
        
        // std::vector<int> free_machine_indexes;
        // for(int i = 0; i < machines_.size(); i++)
        // {
        //     if(machines_[i].isFree())
        //     {
        //         free_machine_indexes.push_back(i);
        //     }
        // }
        // size_t num_free_machines = free_machine_indexes.size();
        // size_t num_accumulated_jobs = accumulated_jobs_.size();
        // size_t max_num_job_scheduled = std::min(num_free_machines, num_accumulated_jobs);

        // for(int num_job_scheduled = 0; num_job_scheduled <= max_num_job_scheduled; num_job_scheduled++)
        // {
        //     // enumerate the number of jobs to be schedule this turn
        //     for(auto & job_scheduled_indexes : job_schedule_combination)
        //     {
        //         for(auto & machine_scheduled_indexes : machine_schedule_combination)
        //         {
        //             // schedule it!
        //             for(int i = 0; i < num_job_scheduled; i++)
        //             {
        //                 auto & machine = machines_[free_machine_indexes[machine_scheduled_indexes[i]]];
        //                 machine.execute(accumulated_jobs_[job_scheduled_indexes[i]]);
        //                 backtrace(timestamp + 1, jobs_array_index);
        //                 machine.setFree();
        //             }
        //         }
        //     }
        // }
        // machines_ = machines_backup_;
        // accumulated_jobs_ = accumulated_jobs_backup_;
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
    std::vector<ScheduleStep> current_scheduler_steps_;
    std::vector<ScheduleStep> best_scheduler_steps_;
    int64_t best_total_timespent_ = INVALID_TOTAL_TIMESPENT;
    constexpr static int64_t INVALID_TOTAL_TIMESPENT = 0; 

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

    bool done()
    {
        // TODO: implement it.
        return true;
    }
};

}