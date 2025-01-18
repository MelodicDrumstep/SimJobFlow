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

TEST(IntegrationTest, UnrelatedListBasicIntegration1) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/UnrelatedListBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/unrelated_config0.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>());
                
    try
    {
        ALGsolver.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

TEST(IntegrationTest, UnrelatedListBasicIntegration2) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/UnrelatedListBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/unrelated_config1.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>());
                
    try
    {
        ALGsolver.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

TEST(IntegrationTest, UnrelatedListBasicIntegration3) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/UnrelatedListBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::List_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/unrelated_config2.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>());
                
    try
    {
        ALGsolver.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

TEST(IntegrationTest, UnrelatedRealTimeBasicIntegration1) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/UnrelatedRealTimeBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/unrelated_config0.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>());
                
    try
    {
        ALGsolver.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

TEST(IntegrationTest, UnrelatedRealTimeBasicIntegration2) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/UnrelatedRealTimeBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/unrelated_config1.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>());
                
    try
    {
        ALGsolver.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

TEST(IntegrationTest, UnrelatedRealTimeBasicIntegration3) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/UnrelatedRealTimeBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::Real_Time_Arrival>;

    std::string config_path = PROJECT_ROOT_DIR "/assets/config/unrelated_config2.json";
    json config = parseJsonFile(config_path);

    ALGSolver<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> ALGsolver(config["Machine_Config"], 
                                       std::make_unique<SchedulerT>(), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config["Output_Path"]), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>());
                
    try
    {
        ALGsolver.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}