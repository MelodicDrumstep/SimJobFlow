#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "simjobflow.hpp"
#include "greedy_scheduler.hpp"
#include "ticking_timer.hpp"
#include "json_input_handler.hpp"
#include "file_output_handler.hpp"
#include "json_parser.hpp"
#include "model.hpp"

using namespace SJF;

TEST(IntegrationTest, IdenticalListBasicIntegration1) 
{
    NanoLog::setLogFile("./IdenticalListBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job1.json";
    std::string output_path = "../../../assets/output/Ilist_output1.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, IdenticalListBasicIntegration2) 
{
    NanoLog::setLogFile("./IdenticalListBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config2.json";
    std::string json_job_path = "../../../assets/json/job/normal_job2.json";
    std::string output_path = "../../../assets/output/Ilist_output2.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, IdenticalListBasicIntegration3) 
{
    NanoLog::setLogFile("./IdenticalListBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job3.json";
    std::string output_path = "../../../assets/output/Ilist_output3.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>(config));
                
    simjobflow.start();
}


TEST(IntegrationTest, IdenticalRealTimeBasicIntegration1) 
{
    NanoLog::setLogFile("./IdenticalRealTimeBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job1.json";
    std::string output_path = "../../../assets/output/Ireal_time_output1.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, IdenticalRealTimeBasicIntegration2) 
{
    NanoLog::setLogFile("./IdenticalRealTimeBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>;

    std::string json_config_path = "../../../assets/json/config/normal_config2.json";
    std::string json_job_path = "../../../assets/json/job/normal_job2.json";
    std::string output_path = "../../../assets/output/Ireal_time_output2.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Identical, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>, 
               TickingTimer<Machine_Model::Identical>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Identical>>(config));
                
    simjobflow.start();
}