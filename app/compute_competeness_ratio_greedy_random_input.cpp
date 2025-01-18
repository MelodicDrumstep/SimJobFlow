
#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <exception>
#include "competeness_ratio_calculator.hpp"
#include "greedy_scheduler.hpp"
#include "ticking_timer.hpp"
#include "random_input_handler.hpp"
#include "sim_output_handler.hpp"
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
    NanoLog::setLogFile(PROJECT_ROOT_DIR "/logs/competeness_ratio.log");
    NanoLog::setLogLevel(NOTICE);
    json config = parseJsonFile(argv[1]);
    std::string machine_model = config["Machine_Model"].get<std::string>();
    std::string release_model = config["Release_Model"].get<std::string>();
    double result;
    if(machine_model == "Identical")
    {
        if(release_model == "List")
        {
            CompetenessRatioCalculator<Machine_Model::Identical, 
                GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>,
                RandomInputHandler<Machine_Model::Identical>, 
                SimOutputHandler<Machine_Model::Identical>, 
                TickingTimer<Machine_Model::Identical>> calculator(config);
            result = calculator.calculate();
        }
        else if(release_model == "RealTime")
        {
            CompetenessRatioCalculator<Machine_Model::Identical, 
                GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>,
                RandomInputHandler<Machine_Model::Identical>, 
                SimOutputHandler<Machine_Model::Identical>, 
                TickingTimer<Machine_Model::Identical>> calculator(config);
            result = calculator.calculate();
        }
        else
        {
            throw std::invalid_argument("Invalid release model");
        }
    }
    else if(machine_model == "Related")
    {
        if(release_model == "List")
        {
            CompetenessRatioCalculator<Machine_Model::Related, 
                GreedyScheduler<Machine_Model::Related, Release_Model::List_Arrival>,
                RandomInputHandler<Machine_Model::Related>, 
                SimOutputHandler<Machine_Model::Related>, 
                TickingTimer<Machine_Model::Related>> calculator(config);
            result = calculator.calculate();
        }
        else if(release_model == "RealTime")
        {
            CompetenessRatioCalculator<Machine_Model::Related, 
                GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>,
                RandomInputHandler<Machine_Model::Related>, 
                SimOutputHandler<Machine_Model::Related>, 
                TickingTimer<Machine_Model::Related>> calculator(config);
        }
        else
        {
            throw std::invalid_argument("Invalid release model");
        }
    }
    else if(machine_model == "Unrelated")
    {
        if(release_model == "List")
        {
            CompetenessRatioCalculator<Machine_Model::Unrelated, 
                GreedyScheduler<Machine_Model::Unrelated, Release_Model::List_Arrival>,
                RandomInputHandler<Machine_Model::Unrelated>, 
                SimOutputHandler<Machine_Model::Unrelated>,
                TickingTimer<Machine_Model::Unrelated>> calculator(config);
            result = calculator.calculate();
        }
        else if(release_model == "RealTime")
        {
            CompetenessRatioCalculator<Machine_Model::Unrelated, 
                GreedyScheduler<Machine_Model::Unrelated, Release_Model::Real_Time_Arrival>,
                RandomInputHandler<Machine_Model::Unrelated>, 
                SimOutputHandler<Machine_Model::Unrelated>,
                TickingTimer<Machine_Model::Unrelated>> calculator(config);
            result = calculator.calculate();
        }
        else
        {
            throw std::invalid_argument("Invalid release model");
        }
    }
    else
    {
        throw std::invalid_argument("Invalid machine model");
    }
                
    std::cout << "competeness ratio : " << result << std::endl;
    return 0;
}