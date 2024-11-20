#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "simjobflow.hpp"
#include "greedy_scheduler.hpp"
#include "ticking_timer.hpp"
#include "json_input_handler.hpp"
#include "file_output_handler.hpp"
#include "json_parser.hpp"

#include "basic_utils_in_one_header.hpp"

using namespace SJF;

TEST(IntegrationTest, UnrelatedListBasicIntegration1) 
{
    NanoLog::setLogFile("./UnrelatedListBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config1.json";
    std::string json_job_path = "../../../assets/json/job/unrelated_job1.json";
    std::string output_path = "../../../assets/output/URlist_output1.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>(config));
                
    try
    {
        simjobflow.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

TEST(IntegrationTest, UnrelatedListBasicIntegration2) 
{
    NanoLog::setLogFile("./UnrelatedListBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config2.json";
    std::string json_job_path = "../../../assets/json/job/unrelated_job2.json";
    std::string output_path = "../../../assets/output/URlist_output2.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>(config));
                
    try
    {
        simjobflow.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

TEST(IntegrationTest, UnrelatedListBasicIntegration3) 
{
    NanoLog::setLogFile("./UnrelatedListBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Unrelated, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config2.json";
    std::string json_job_path = "../../../assets/json/job/unrelated_job2.json";
    std::string output_path = "../../../assets/output/URlist_output3.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Unrelated, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Unrelated>, 
               FileOutputHandler<Machine_Model::Unrelated>, 
               TickingTimer<Machine_Model::Unrelated>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Unrelated>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Unrelated>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Unrelated>>(config));
                
    try
    {
        simjobflow.start();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}