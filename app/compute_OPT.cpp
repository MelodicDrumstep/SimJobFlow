
#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "simjobflow.hpp"
#include "OPTSolver.hpp"
#include "ticking_timer.hpp"
#include "json_input_handler.hpp"
#include "file_output_handler.hpp"
#include "json_parser.hpp"
#include "model.hpp"

using namespace SJF;

int main()
{
    NanoLog::setLogFile("./IdenticalListBasicIntegration1.log");
    NanoLog::setLogLevel(NOTICE);

    std::string json_config_path = "../../assets/json/config/normal_config2.json";
    std::string json_job_path = "../../assets/json/job/normal_job2.json";
    std::string output_path = "../../assets/output/OPT1.log";
    json config = parseJsonFile(json_config_path);

    OPTSolver<Machine_Model::Identical, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>> opt_solver(config, 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config, json_job_path), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config, output_path));
                
    opt_solver.start();
    return 0;
}