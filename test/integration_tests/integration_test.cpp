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

#define DEBUG_INTEG_TEST

TEST(IntegrationTest, RealTimeBasicIntegration1) 
{
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>;

    std::string json_config_path = "../../../assets/json/config/sample_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job1.json";
    std::string output_path = "../../../assets/output/real_time_output1.log";
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

TEST(IntegrationTest, RealTimeBasicIntegration2) 
{
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>;

    std::string json_config_path = "../../../assets/json/config/sample_config2.json";
    std::string json_job_path = "../../../assets/json/job/normal_job2.json";
    std::string output_path = "../../../assets/output/real_time_output2.log";
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

TEST(IntegrationTest, ListBasicIntegration1) 
{
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/sample_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job1.json";
    std::string output_path = "../../../assets/output/list_output1.log";
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

TEST(IntegrationTest, ListBasicIntegration2) 
{
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/sample_config2.json";
    std::string json_job_path = "../../../assets/json/job/normal_job2.json";
    std::string output_path = "../../../assets/output/list_output2.log";
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

TEST(IntegrationTest, ListBasicIntegration3) 
{
    using SchedulerT = GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/sample_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job3.json";
    std::string output_path = "../../../assets/output/list_output3.log";
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