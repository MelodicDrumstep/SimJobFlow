#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "ALGSolver.hpp"
#include "greedy_scheduler.hpp"
#include "ticking_timer.hpp"
#include "json_input_handler.hpp"
#include "file_output_handler.hpp"
#include "json_parser.hpp"

#include "basic_utils_in_one_header.hpp"

using namespace SJF;

TEST(IntegrationTest, RelatedListBasicIntegration1) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/RelatedListBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/related_config0.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, RelatedListBasicIntegration2) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/RelatedListBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/related_config1.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, RelatedListBasicIntegration3) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/RelatedListBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/related_config2.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, RelatedRealTimeBasicIntegration1) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/RelatedRealTimeBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/related_config0.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, RelatedRealTimeBasicIntegration2) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/RelatedRealTimeBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/related_config1.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>());
                
    ALGsolver.start();
}

TEST(IntegrationTest, RelatedRealTimeBasicIntegration3) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/RelatedRealTimeBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/related_config2.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>());
                
    ALGsolver.start();
}
