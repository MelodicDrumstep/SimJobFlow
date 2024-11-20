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

TEST(IntegrationTest, RelatedListBasicIntegration1) 
{
    NanoLog::setLogFile("./RelatedListBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/related_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job1.json";
    std::string output_path = "../../../assets/output/Rlist_output1.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, RelatedListBasicIntegration2) 
{
    NanoLog::setLogFile("./RelatedListBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/related_config2.json";
    std::string json_job_path = "../../../assets/json/job/normal_job2.json";
    std::string output_path = "../../../assets/output/Rlist_output2.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, RelatedListBasicIntegration3) 
{
    NanoLog::setLogFile("./RelatedListBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::List_Arrival>;

    std::string json_config_path = "../../../assets/json/config/related_config3.json";
    std::string json_job_path = "../../../assets/json/job/normal_job3.json";
    std::string output_path = "../../../assets/output/Rlist_output3.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, RelatedRealTimeBasicIntegration1) 
{
    NanoLog::setLogFile("./RelatedRealTimeBasicIntegration1.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>;

    std::string json_config_path = "../../../assets/json/config/related_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job1.json";
    std::string output_path = "../../../assets/output/Rreal_time_output1.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, RelatedRealTimeBasicIntegration2) 
{
    NanoLog::setLogFile("./RelatedRealTimeBasicIntegration2.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>;

    std::string json_config_path = "../../../assets/json/config/related_config2.json";
    std::string json_job_path = "../../../assets/json/job/normal_job2.json";
    std::string output_path = "../../../assets/output/Rreal_time_output2.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, RelatedRealTimeBasicIntegration3) 
{
    NanoLog::setLogFile("./RelatedRealTimeBasicIntegration3.log");
    NanoLog::setLogLevel(DEBUG);
    using SchedulerT = GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>;

    std::string json_config_path = "../../../assets/json/config/related_config3.json";
    std::string json_job_path = "../../../assets/json/job/normal_job3.json";
    std::string output_path = "../../../assets/output/Rreal_time_output3.log";
    json config = parseJsonFile(json_config_path);

    SimJobFlow<Machine_Model::Related, 
               SchedulerT, 
               JsonInputHandler<Machine_Model::Related>, 
               FileOutputHandler<Machine_Model::Related>, 
               TickingTimer<Machine_Model::Related>> simjobflow(config, 
                                       std::make_unique<SchedulerT>(config), 
                                       std::make_unique<JsonInputHandler<Machine_Model::Related>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Related>>(config, output_path), 
                                       std::make_unique<TickingTimer<Machine_Model::Related>>(config));
                
    simjobflow.start();
}
