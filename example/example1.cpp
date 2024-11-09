

#include "simjobflow.hpp"
#include "greedy_scheduler.hpp"
#include "ticking_timer.hpp"
#include "json_input_handler.hpp"
#include "file_output_handler.hpp"
#include "model.hpp"

int main()
{
    std::string json_config_path = "../assets/json/config/sample_config.json";
    std::string json_job_path = "../assets/json/job/sample_job.json";
    json config = SimJobFlow::parseJsonFile(json_config_path);
    SimJobFlow<Model::Identical, 
               GreedyScheduler, 
               JsonInputHandler, 
               FileOutputHandler, 
               TikTokTimer> simjobflow(config, 
                                       std::make_unique<GreedyScheduler>(config, json_job_path), 
                                       std::make_unique<JsonInputHandler>(config), 
                                       std::make_unique<FileOutputHandler>(config), 
                                       std::make_unique<TikTokTimer>(config));
}