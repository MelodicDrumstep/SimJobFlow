
#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "ALGSolver.hpp"
#include "OPTSolver.hpp"
#include "ticking_timer.hpp"
#include "json_input_handler.hpp"
#include "file_output_handler.hpp"
#include "json_parser.hpp"
#include "model.hpp"

using namespace SJF;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/sample.log");
    NanoLog::setLogLevel(NOTICE);
    json config = parseJsonFile(argv[1]);
    
    OPTSolver<Machine_Model::Identical, 
               JsonInputHandler<Machine_Model::Identical>, 
               FileOutputHandler<Machine_Model::Identical>> opt_solver(config["Machine_Config"], 
                                       std::make_unique<JsonInputHandler<Machine_Model::Identical>>(config["Job_Config"]), 
                                       std::make_unique<FileOutputHandler<Machine_Model::Identical>>(config["Output_Path"]));
                
    opt_solver.start();
    return 0;
}