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
#include "ALGSolver.hpp"
#include "OPTSolver.hpp"
#include "json_parser.hpp"
#include "basic_utils_in_one_header.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @tparam machine_model            An instance of enum class Machine_Model. There are 3 models supported : 
 *          1. Identical     2. Related      3. Unrelated.   For there specific definitions, @see README.md.
 * @tparam SchedulerT       Scheduler type. It's reponsible for scheduling jobs to free machines, 
 *          and update the machine state for each turn.
 * @tparam InputHandlerT    Input handler type. It's reponsible for getting the input of jobs for each turn.
 * @tparam OutputHandlerT   Output handler type. It's reponsible for outputing the scheduler steps.
 * @tparam TimerT           Timer type. It's reponsible for deciding the timestamp for the next turn, i.e. the elapsing time for each turn.
 */
template <Machine_Model machine_model, Scheduler<machine_model> SchedulerT, OPTInputHandler<machine_model> InputHandlerT, 
                                        OPTOutputHandler<machine_model> OutputHandlerT, Timer<machine_model> TimerT>
class CompetenessRatioCalculator
{
/**
 * MachineModelTraits, which located at "src/machine_model/model_trait.hpp, contains the job type and machine type for this machine_model"
 */
using JobT = typename MachineModelTraits<machine_model>::JobT;
using MachineT = typename MachineModelTraits<machine_model>::MachineT;
using ALGSolverT = ALGSolver<machine_model, SchedulerT, InputHandlerT, OutputHandlerT, TimerT>;
using OPTSolverT = OPTSolver<machine_model, InputHandlerT, OutputHandlerT>;

public:
    CompetenessRatioCalculator(json & config) 
        : input_handler_(std::make_shared<InputHandlerT>(config["Job_Config"])),
        alg_solver_(config["Machine_Config"], 
            std::make_unique<SchedulerT>(), 
            input_handler_, 
            std::make_unique<OutputHandlerT>(config["Output_Path"]), 
            std::make_unique<TimerT>()),
        opt_solver_(config["Machine_Config"], 
            input_handler_, 
            std::make_unique<OutputHandlerT>(config["Output_Path"])) {}

    /**
     * @brief Calculate the competeness ratio. It use "ALG / OPT"
     */
    double calculate() {
        int64_t ALG = alg_solver_.start();
        input_handler_ -> refresh();
        int64_t OPT = opt_solver_.start();
        NANO_LOG(NOTICE, "[CompetenessRatioCalculator::calculate] ALG: %ld, OPT: %ld", ALG, OPT);
        return ALG * 1.0 / OPT;
    }

private:
    std::shared_ptr<InputHandlerT> input_handler_;
    ALGSolverT alg_solver_;
    OPTSolverT opt_solver_;
};
}