#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

#include "timer_trait.hpp"
#include "input_handler_trait.hpp"
#include "output_handler_trait.hpp"
#include "scheduler_trait.hpp"

#include "basic_utils_in_one_header.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief The interface for user of this simulated job scheduling system.
 *          usage : configure all parameters, construct one instance of ALGSolver, and call "start" function.
 * 
 * @tparam machine_model            An instance of enum class Machine_Model. There are 3 models supported : 
 *          1. Identical     2. Related      3. Unrelated.   For there specific definitions, @see README.md.
 * @tparam SchedulerT       Scheduler type. It's reponsible for scheduling jobs to free machines, 
 *          and update the machine state for each turn.
 * @tparam InputHandlerT    Input handler type. It's reponsible for getting the input of jobs for each turn.
 * @tparam OutputHandlerT   Output handler type. It's reponsible for outputing the scheduler steps.
 * @tparam TimerT           Timer type. It's reponsible for deciding the timestamp for the next turn, i.e. the elapsing time for each turn.
 */
template <Machine_Model machine_model, Scheduler<machine_model> SchedulerT, InputHandler<machine_model> InputHandlerT, OutputHandler<machine_model> OutputHandlerT, Timer<machine_model> TimerT>
class ALGSolver 
{
/**
 * MachineModelTraits, which located at "src/machine_model/model_trait.hpp, contains the job type and machine type for this machine_model"
 */
using JobT = typename MachineModelTraits<machine_model>::JobT;
using MachineT = typename MachineModelTraits<machine_model>::MachineT;

public:
    /**
     * @brief The constructor of ALGSolver, the only parts that the user must pay attention to.
     * 
     * @param config            A json object containing the configuration information. 
     *          It must contain these fields:
     *              1. Num_of_Machines
     *          I choose to take in a json object rather than the json file path here as a parameter
     *          to avoid parse the same json file again and again, since this json object will be 
     *          passed to Scheduler, InputHandler, OutputHandler and Timer, too.
     * @param scheduler         An unique ptr to the scheduler object. User should pass "std::make_unique" here
     *          or "std::move(my_unique_ptr)" here.
     * @param input_handler     An unique ptr to the input handler object.
     * @param output_handler    An unique ptr to the output handler object.
     * @param timer             An unique ptr to the timer object.
     */
    ALGSolver(const json & config,
               std::unique_ptr<SchedulerT> scheduler,
               std::shared_ptr<InputHandlerT> input_handler,
               std::unique_ptr<OutputHandlerT> output_handler,
               std::unique_ptr<TimerT> timer) :
        num_of_machines_(config["Num_of_Machines"]),
        scheduler_(std::move(scheduler)),
        input_handler_(input_handler),
        output_handler_(std::move(output_handler)),
        timer_(std::move(timer))
    {
        input_handler_ -> checkValidity(num_of_machines_); 
        // checkValidity will check if the number of machines matches will the configuration there.
        initializeMachines(config);
        scheduler_ -> initialize(num_of_machines_, machines_);
    }

    /**
     * @brief Start the scheduling rountine
     * @return The time taken to complete the scheduling
     */
    int64_t start()
    {
        while((!input_handler_ -> done()) || (!scheduler_ -> done()))   
        {
            NANO_LOG(DEBUG, "[ALGSolver::start] Inside the while loop");

            auto timestamp = timer_ -> timestamp(); // get the current timestamp
            std::vector<JobT> jobs_for_this_turn = input_handler_ -> getJobs(timestamp);
            // get the input jobs
            
            std::vector<ScheduleStep> schedule_steps;
            schedule_steps = scheduler_ -> schedule(jobs_for_this_turn, machines_, timestamp);
            jobs_.insert(jobs_.end(), jobs_for_this_turn.begin(), jobs_for_this_turn.end());
            // The scheduler may schedule the jobs to some machines, and fill in the schedule steps

            output_handler_ -> output(machines_, jobs_, timestamp, schedule_steps); // output the scheduler steps
            scheduler_ -> updateMachineState(machines_, timer_ -> tick(machines_));
            // Scheduler is responsible for update the machine state, e.g, remaining time of the machine.
            // This task is given to the scheduler rather than the timer is because the scheduler 
            // can update some inner data structures at the same time.
        }
        output_handler_ -> output(timer_ -> timestamp());
        return timer_ -> timestamp();
    }

private:
    std::unique_ptr<SchedulerT> scheduler_; 
    std::shared_ptr<InputHandlerT> input_handler_;
    std::unique_ptr<OutputHandlerT> output_handler_;
    std::unique_ptr<TimerT> timer_;
    std::vector<MachineT> machines_;    // all machines
    std::vector<JobT> jobs_;            // all jobs. Store all the jobs here for indexing by jobId 
    int64_t num_of_machines_;

    /**
     * @brief Initialize the machine array.
     */
    void initializeMachines(const json & config)
    {  
        NANO_LOG(DEBUG, "[ALGSolver::initializeMachines]");

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
};

}