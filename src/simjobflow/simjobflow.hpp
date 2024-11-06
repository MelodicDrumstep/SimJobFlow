#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>

#include "model.hpp"
#include "objective.hpp"

template <Model model, typename SchedulerT, typename InputHandlerT, typename OutputHandlerT, size_t Num_of_Machines>
class SimJobFlow 
{
/*
 if model == Model::Indentical
    then MachineT = IndenticalMachine
        JobT = NormalJob
 else if model == Model::Unrelated
    then MachineT = UnrelatedMachine
        JobT = NormalJob
 else if model == Model::Related
    then MachineT = RelatedMachine
        JobT = RelatedJob
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
    model == Model::Related,
    RelatedJob,
    NormalJob
>;

public:
    SimJobFlow(/*...*/)
    {
        static_assert(/*Some traits to check the model and other staffs*/);
    }

    void start()
    {
        while(!input_handler_ -> done())
        {
            std::optional<JobT> job = input_handler_ -> getJob();
            if(job)
            {
                scheduler_ -> schedule(*job);
            }
            output_handler_ -> output(machines_);
        }
    }

private:
    std::unique_ptr<SchedulerT> scheduler_; 
    std::unique_ptr<InputHandlerT> input_handler_;
    std::unique_ptr<OutputHandlerT> output_handler_;
    std::array<MachineT, Num_of_Machines> machines_;
    std::vector<JobT> jobs_;
};