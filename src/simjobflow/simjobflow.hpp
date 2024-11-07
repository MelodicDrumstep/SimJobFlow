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

using json = nlohmann::json;

template <Model model, typename SchedulerT, typename InputHandlerT, typename OutputHandlerT, Timer TimerT>
class SimJobFlow 
{
/*
 if model == Model::Indentical
    then MachineT = IndenticalMachine
        JobT = NormalJob
 else if model == Model::Related
    then MachineT = RelatedMachine
        JobT = NormalJob
 else if model == Model::UnRelated
    then MachineT = UnrelatedMachine
        JobT = UnrelatedJob
*/
using MachineT = std::conditional_t<
    model == Model::Indentical,
    IndenticalMachine,
    std::conditional_t<
        model == Model::Unrelated,
        UnrelatedMachine,
        RelatedMachine
    >
>;

using JobT = std::conditional_t<
    model == Model::Unrelated,
    UnrelatedJob,
    NormalJob
>;

public:
    SimJobFlow(const json & config,
               std::unique_ptr<SchedulerT> scheduler,
               std::unique_ptr<InputHandlerT> input_handler,
               std::unique_ptr<OutputHandlerT> output_handler,
               std::unique_ptr<TimerT> timer) :
        scheduler_(std::move(scheduler)),
        input_handler_(std::move(input_handler)),
        output_handler_(std::move(output_handler)),
        timer_(std::move(timer))
    {
        static_assert(/*Some traits to check the model and other staffs*/);
        initialize(config);
    }

    void start()
    {
        while(!input_handler_ -> done())
        {
            auto timestamp = timer_ -> timestamp();
            std::optional<std::vector<JobT>> jobs = input_handler_ -> getJobs(timestamp);
            if(jobs)
            {
                scheduler_ -> schedule(*jobs, machines_, timestamp);
            }
            output_handler_ -> output(machines_, timestamp);
            timer_ -> tick();
        }
    }

private:
    std::unique_ptr<SchedulerT> scheduler_; 
    std::unique_ptr<InputHandlerT> input_handler_;
    std::unique_ptr<OutputHandlerT> output_handler_;
    std::vector<MachineT> machines_;
    std::unique_ptr<TimerT> timer_;

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
};