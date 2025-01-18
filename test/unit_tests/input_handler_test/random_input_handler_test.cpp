#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "json_parser.hpp"
#include "basic_utils_in_one_header.hpp"
#include "random_input_handler.hpp"

using namespace SJF;

using json = nlohmann::json;

TEST(RandomInputHandlerTest, IdGetJobs) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/IdGetJobs.log");
    NanoLog::setLogLevel(DEBUG);
    std::string config_path = PROJECT_ROOT_DIR "/assets/config/id_config0.json";
    json config = parseJsonFile(config_path);
    RandomInputHandler<Machine_Model::Identical> handler(config["Job_Config"]);

    auto & jobs = handler.getJobArray();
    for(auto & job : jobs)
    {
        std::cout << job.toString() << std::endl;
    }
}

TEST(RandomInputHandlerTest, RelatedGetJobs) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/RelatedGetJobs.log");
    NanoLog::setLogLevel(DEBUG);
    std::string config_path = PROJECT_ROOT_DIR "/assets/config/related_config0.json";
    json config = parseJsonFile(config_path);
    RandomInputHandler<Machine_Model::Related> handler(config["Job_Config"]);

    auto & jobs = handler.getJobArray();
    for(auto & job : jobs)
    {
        std::cout << job.toString() << std::endl;
    }
}

TEST(RandomInputHandlerTest, UnrelatedGetJobs) 
{
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/UnrelatedGetJobs.log");
    NanoLog::setLogLevel(DEBUG);
    std::string config_path = PROJECT_ROOT_DIR "/assets/config/unrelated_config0.json";
    json config = parseJsonFile(config_path);
    RandomInputHandler<Machine_Model::Unrelated> handler(config["Job_Config"]);

    auto & jobs = handler.getJobArray();
    for(auto & job : jobs)
    {
        std::cout << job.toString() << std::endl;
    }
}