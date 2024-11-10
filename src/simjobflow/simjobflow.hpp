#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

#include "model.hpp"
#include "objective.hpp"
#include "timer_trait.hpp"
#include "input_handler_trait.hpp"
#include "output_handler_trait.hpp"
#include "scheduler_trait.hpp"
#include "schedule_step.hpp"

namespace SJF
{

#define DEBUG_SJF

using json = nlohmann::json;

/**
 * @brief The interface for user of this simulated job scheduling system.
 *          usage : configure all parameters, construct one instance of SimJobFlow, and call "start" function.
 * 
 * @tparam model            An instance of enum class Model. There are 3 models supported : 
 *          1. Identical     2. Related      3. Unrelated.   For there specific definitions, @see README.md.
 * @tparam SchedulerT       Scheduler type. It's reponsible for scheduling jobs to free machines, 
 *          and update the machine state for each turn.
 * @tparam InputHandlerT    Input handler type. It's reponsible for getting the input of jobs for each turn.
 * @tparam OutputHandlerT   Output handler type. It's reponsible for outputing the scheduler steps.
 * @tparam TimerT           Timer type. It's reponsible for deciding the timestamp for the next turn, i.e. the elapsing time for each turn.
 */
template <Model model, Scheduler<model> SchedulerT, InputHandler<model> InputHandlerT, OutputHandler<model> OutputHandlerT, Timer<model> TimerT>
class SimJobFlow 
{
/**
 * ModelTraits, which located at "src/model/model_trait.hpp, contains the job type and machine type for this model"
 */
using JobT = typename ModelTraits<model>::JobT;
using MachineT = typename ModelTraits<model>::MachineT;

public:
    /**
     * @brief The constructor of SimJobFlow, the only parts that the user must pay attention to.
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
    SimJobFlow(const json & config,
               std::unique_ptr<SchedulerT> scheduler,
               std::unique_ptr<InputHandlerT> input_handler,
               std::unique_ptr<OutputHandlerT> output_handler,
               std::unique_ptr<TimerT> timer) :
        num_of_machines_(config["Num_of_Machines"]),
        scheduler_(std::move(scheduler)),
        input_handler_(std::move(input_handler)),
        output_handler_(std::move(output_handler)),
        timer_(std::move(timer))
    {
        input_handler_ -> checkValidity(num_of_machines_); 
        // checkValidity will check if the number of machines matches will the configuration there.
        scheduler_ -> initialize(num_of_machines_);
        initializeMachines(config);
    }

    void start()
    {
        while(!input_handler_ -> done())   
        {
            auto timestamp = timer_ -> timestamp(); // get the current timestamp
            std::optional<std::vector<JobT>> jobs_for_this_turn = input_handler_ -> getJobs(timestamp);
            // get the input jobs
            std::vector<ScheduleStep> schedule_steps;
            if(jobs_for_this_turn.has_value())
            {
                // If there are some input jobs for this turn, pass them to the scheduler
                schedule_steps = scheduler_ -> schedule(*jobs_for_this_turn, machines_, timestamp);
                jobs_.insert(jobs_.end(), jobs_for_this_turn -> begin(), jobs_for_this_turn -> end());
                // The scheduler may schedule the jobs to some machines, and fill in the schedule steps
            }
            output_handler_ -> output(machines_, jobs_, timestamp, schedule_steps); // output the scheduler steps
            scheduler_ -> updateMachineState(machines_, timer_ -> tick(machines_));
            // Scheduler is responsible for update the machine state, e.g, remaining time of the machine.
            // This task is given to the scheduler rather than the timer is because the scheduler 
            // can update some inner data structures at the same time.
        }
    }

private:
    std::unique_ptr<SchedulerT> scheduler_; 
    std::unique_ptr<InputHandlerT> input_handler_;
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
        machines_.reserve(num_of_machines_);
        if constexpr (model == Model::Related)
        {
            // If the model is Related, there's a processing speed field for the machine type
            // configure it using the field in json object
            std::vector<int64_t> processing_speed = config["Processing_Speed"];
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