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
#include "schedule_step.hpp"

namespace SJF
{

using json = nlohmann::json;

template <Model model, typename SchedulerT, InputHandler<model> InputHandlerT, OutputHandler<model> OutputHandlerT, Timer<model> TimerT>
class SimJobFlow 
{
using JobT = typename ModelTraits<model>::JobT;
using MachineT = typename ModelTraits<model>::MachineT;

public:
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
        static_assert(/*Some traits to check the model and other staffs*/);
        input_handler_ -> checkValidity(num_of_machines_);
        scheduler -> initialize(num_of_machines_);
        initializeMachines(config);
    }

    void start()
    {
        while(!input_handler_ -> done())
        {
            auto timestamp = timer_ -> timestamp();
            std::optional<std::vector<JobT>> jobs_for_this_turn = input_handler_ -> getJobs(timestamp);
            std::vector<ScheduleStep> schedule_steps;
            if(jobs_for_this_turn.has_value())
            {
                schedule_steps = scheduler_ -> schedule(*jobs_for_this_turn, machines_, timestamp);
                jobs_.insert(jobs_.end(), jobs_for_this_turn -> begin(), jobs_for_this_turn -> end());
            }
            output_handler_ -> output(machines_, jobs_, timestamp, schedule_steps);
            scheduler -> maintainMachineState(machines_, timer_ -> tick(machines_));
        }
    }

private:
    std::unique_ptr<SchedulerT> scheduler_; 
    std::unique_ptr<InputHandlerT> input_handler_;
    std::unique_ptr<OutputHandlerT> output_handler_;
    std::unique_ptr<TimerT> timer_;
    std::vector<MachineT> machines_;
    std::vector<JobT> jobs_;
    int64_t num_of_machines_;

    static json parseJsonFile(std::string_view json_config_path) 
    {
        std::ifstream file(json_config_path.data());
        if(!file.is_open()) 
        {
            throw std::runtime_error("Failed to open file: " + std::string(config_path));
        }

        json config;
        file >> config;
        file.close();
        return config;
    }

    void initializeMachines(const json & config)
    {
        machines_.reserve(num_of_machines_);
        machine_free_list.reserve(num_of_machines_);
        if constexpr (model == Model::Related)
        {
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
        for(size_t i = 0; i < num_of_machines_; i++)
        {
            machine_free_list_.push_back(i);
        }
    }
};

}