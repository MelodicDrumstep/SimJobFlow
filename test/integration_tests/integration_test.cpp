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

TEST(IntegrationTest, BasicIntegration1) 
{
    std::string json_config_path = "../../../assets/json/config/sample_config1.json";
    std::string json_job_path = "../../../assets/json/job/normal_job1.json";
    std::string output_path = "../../../assets/output/output1.log";
    json config = parseJsonFile(json_config_path);
    auto scheduler = std::make_unique<GreedyScheduler<Model::Identical>>(config);

    // DEBUGING
    #ifdef DEBUG_INTEG_TEST
        std::cout << "Inside integration test .cpp file.\n";
        std::cout << "&(*scheduler) is " << &(*scheduler) << std::endl;
        auto & machine_free_list = scheduler -> getMachineFreeList();
        std::cout << "&machine_free_list is " << &machine_free_list << std::endl;
        std::cout << "The initial size of the machine free list of the scheduler is " << machine_free_list.size() << "\n\n";
    #endif
    // DEBUGING

    SimJobFlow<Model::Identical, 
               GreedyScheduler<Model::Identical>, 
               JsonInputHandler<Model::Identical>, 
               FileOutputHandler<Model::Identical>, 
               TickingTimer<Model::Identical>> simjobflow(config, 
                                       std::move(scheduler), 
                                       std::make_unique<JsonInputHandler<Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Model::Identical>>(config, output_path), 
                                       std::make_unique<TickingTimer<Model::Identical>>(config));
                
    simjobflow.start();
}

TEST(IntegrationTest, BasicIntegration2) 
{
    std::string json_config_path = "../../../assets/json/config/sample_config2.json";
    std::string json_job_path = "../../../assets/json/job/normal_job2.json";
    std::string output_path = "../../../assets/output/output2.log";
    json config = parseJsonFile(json_config_path);
    auto scheduler = std::make_unique<GreedyScheduler<Model::Identical>>(config);

    // DEBUGING
    #ifdef DEBUG_INTEG_TEST
        std::cout << "Inside integration test .cpp file.\n";
        std::cout << "&(*scheduler) is " << &(*scheduler) << std::endl;
        auto & machine_free_list = scheduler -> getMachineFreeList();
        std::cout << "&machine_free_list is " << &machine_free_list << std::endl;
        std::cout << "The initial size of the machine free list of the scheduler is " << machine_free_list.size() << "\n\n";
    #endif
    // DEBUGING

    SimJobFlow<Model::Identical, 
               GreedyScheduler<Model::Identical>, 
               JsonInputHandler<Model::Identical>, 
               FileOutputHandler<Model::Identical>, 
               TickingTimer<Model::Identical>> simjobflow(config, 
                                       std::move(scheduler), 
                                       std::make_unique<JsonInputHandler<Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Model::Identical>>(config, output_path), 
                                       std::make_unique<TickingTimer<Model::Identical>>(config));
                
    simjobflow.start();
}