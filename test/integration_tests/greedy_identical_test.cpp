#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "ALGSolver.hpp"
#include "greedy_scheduler.hpp"
#include "ticking_timer.hpp"
#include "json_input_handler.hpp"
#include "file_output_handler.hpp"
#include "json_parser.hpp"
#include "model.hpp"

using namespace SJF;

TEST(IntegrationTest, IdenticalListBasicIntegration1) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/IdenticalListBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/id_config0.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, IdenticalListBasicIntegration2) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/IdenticalListBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/id_config1.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, IdenticalListBasicIntegration3) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/IdenticalListBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/id_config1.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>());
                
    ALGsolver.start();
}


TEST(IntegrationTest, IdenticalRealTimeBasicIntegration1) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/IdenticalRealTimeBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/id_config0.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, IdenticalRealTimeBasicIntegration2) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/IdenticalRealTimeBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/id_config1.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>());
                
    ALGsolver.start();
}