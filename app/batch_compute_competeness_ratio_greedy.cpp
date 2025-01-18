
#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <exception>
#include "competeness_ratio_calculator.hpp"
#include "greedy_scheduler.hpp"
#include "ticking_timer.hpp"
#include "random_input_handler.hpp"
#include "sim_output_handler.hpp"
#include "json_parser.hpp"
#include "model.hpp"

using namespace SJF;

double calculateMean(const std::vector<double>& data);
double calculateVariance(const std::vector<double>& data, double mean);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }
    json config = parseJsonFile(argv[1]);
    int test_times = config["Test_Times"].get<int>();
    std::string machine_model = config["Machine_Model"].get<std::string>();
    std::string release_model = config["Release_Model"].get<std::string>();
    std::string output_path = config["Output_Path"].get<std::string>();
    std::fstream output_file(output_path,std::ios::app);
    NanoLog::setLogFile((std::string(PROJECT_ROOT_DIR) + "/logs/" + machine_model + "_" + release_model + ".log").c_str());
    NanoLog::setLogLevel(NOTICE);

    std::vector<double> results;
    for(int i = 0; i < test_times; i++)
    {
        if(machine_model == "Identical")
        {
            if(release_model == "List")
            {
                CompetenessRatioCalculator<Machine_Model::Identical, 
                    GreedyScheduler<Machine_Model::Identical, Release_Model::List_Arrival>,
                    RandomInputHandler<Machine_Model::Identical>, 
                    SimOutputHandler<Machine_Model::Identical>, 
                    TickingTimer<Machine_Model::Identical>> calculator(config);
                double result = calculator.calculate();
                NANO_LOG(NOTICE, "Turn %d, result is %lf", i, result);
                std::cout << "Turn " << i << ", result is " << result << std::endl;
                results.push_back(result);
            }
            else if(release_model == "RealTime")
            {
                CompetenessRatioCalculator<Machine_Model::Identical, 
                    GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival>,
                    RandomInputHandler<Machine_Model::Identical>, 
                    SimOutputHandler<Machine_Model::Identical>, 
                    TickingTimer<Machine_Model::Identical>> calculator(config);
                double result = calculator.calculate();
                NANO_LOG(NOTICE, "Turn %d, result is %lf", i, result);
                std::cout << "Turn " << i << ", result is " << result << std::endl;
                results.push_back(result);
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
                double result = calculator.calculate();
                NANO_LOG(NOTICE, "Turn %d, result is %lf", i, result);
                std::cout << "Turn " << i << ", result is " << result << std::endl;
                results.push_back(result);
            }
            else if(release_model == "RealTime")
            {
                CompetenessRatioCalculator<Machine_Model::Related, 
                    GreedyScheduler<Machine_Model::Related, Release_Model::Real_Time_Arrival>,
                    RandomInputHandler<Machine_Model::Related>, 
                    SimOutputHandler<Machine_Model::Related>, 
                    TickingTimer<Machine_Model::Related>> calculator(config);
                double result = calculator.calculate();
                NANO_LOG(NOTICE, "Turn %d, result is %lf", i, result);
                std::cout << "Turn " << i << ", result is " << result << std::endl;
                results.push_back(result);
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
                double result = calculator.calculate();
                NANO_LOG(NOTICE, "Turn %d, result is %lf", i, result);
                std::cout << "Turn " << i << ", result is " << result << std::endl;
                results.push_back(result);
            }
            else if(release_model == "RealTime")
            {
                CompetenessRatioCalculator<Machine_Model::Unrelated, 
                    GreedyScheduler<Machine_Model::Unrelated, Release_Model::Real_Time_Arrival>,
                    RandomInputHandler<Machine_Model::Unrelated>, 
                    SimOutputHandler<Machine_Model::Unrelated>,
                    TickingTimer<Machine_Model::Unrelated>> calculator(config);
                double result = calculator.calculate();
                NANO_LOG(NOTICE, "Turn %d, result is %lf", i, result);
                std::cout << "Turn " << i << ", result is " << result << std::endl;
                results.push_back(result);
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
    }

    double mean = calculateMean(results);
    double variance = calculateVariance(results, mean);
    json result_json;
    result_json["Machine_Model"] = machine_model;
    result_json["Release_Model"] = release_model;
    auto & job_config = config["Job_Config"];
    result_json["Num_of_Jobs_Lower_Bound"] = job_config["Num_of_Jobs_Lower_Bound"];
    result_json["Num_of_Jobs_Upper_Bound"] = job_config["Num_of_Jobs_Upper_Bound"];
    result_json["Possion_Lambda"] = job_config["Possion_Lambda"];
    result_json["Processing_Time_Lower_Bound"] = job_config["Processing_Time_Lower_Bound"];
    result_json["Processing_Time_Upper_Bound"] = job_config["Processing_Time_Upper_Bound"];
    result_json["result_mean"] = mean;
    result_json["result_variance"] = variance;
    output_file << result_json.dump(4);

    std::cout << "Result : ";
    for(auto & result : results)
    {
        std::cout << result << " ";
    }
    std::cout << std::endl;
    std::cout << "Mean : " << mean << " Variance : " << variance << std::endl;
    return 0;
}

double calculateMean(const std::vector<double>& data) 
{
    if (data.empty()) 
    {
        return 0.0;
    }
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double calculateVariance(const std::vector<double>& data, double mean) 
{
    if (data.empty()) 
    {
        return 0.0; 
    }
    double variance = 0.0;
    for (double value : data) 
    {
        variance += std::pow(value - mean, 2);
    }
    return variance / data.size();
}