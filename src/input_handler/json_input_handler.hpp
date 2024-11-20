#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <type_traits>
#include <nlohmann/json.hpp>

#include "json_parser.hpp"
#include "basic_utils_in_one_header.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief The input handler that takes all the jobs from a json file.
 * 
 * @tparam machine_model    Identical / Related / Unrelated
 */
template <Machine_Model machine_model>
class JsonInputHandler
{
public:
using JobT = typename MachineModelTraits<machine_model>::JobT;

    /**
     * @brief Parse the json file, do some checking, store all the jobs in a vector.
     */
    JsonInputHandler(const json & config, std::string_view json_job_path)
    {
        json job_metadata = parseJsonFile(json_job_path);
        job_type_ = job_metadata["Type"];
        auto & jobs = job_metadata["Jobs"];
        num_of_machines_ = job_metadata["Num_of_Machines"];
        if constexpr (std::is_same_v<JobT, UnrelatedJob>)
        {
            if(job_type_ != "Unrelated")
            {
                throw InvalidJobType(job_type_);
            }
            for(auto & job : jobs)
            {
                std::vector<int64_t> processing_speeds = job["processing_time"];
                if(processing_speeds.size() != num_of_machines_)
                {
                    throw InvalidUnrelatedJobProcessingTime(processing_speeds.size(), num_of_machines_);
                }
                job_array_.emplace_back(job["timestamp"], std::move(processing_speeds));
            }
        }
        else
        {
            if(job_type_ != "Normal")
            {
                throw InvalidJobType(job_type_);
            }
            for(auto & job : jobs)
            {
                job_array_.emplace_back(job["timestamp"], job["workload"]);
            }
        }

        // Sort the job array by timestamp, and fill in the jobId field one by one.
        std::sort(job_array_.begin(), job_array_.end());
        for(size_t i = 0; i < job_array_.size(); i++)
        {
            job_array_[i].id_ = i;
        }

        NANO_LOG(DEBUG, "[JsonInputHandler::Constructor] printing the job array : ");
        for(auto & job : job_array_)
        {
            NANO_LOG(DEBUG, "%s", job.toString().c_str());
        }
    }

    /**
     * @brief Check if the number of machines matches.
     */
    bool checkValidity(int64_t num_of_machines)
    {
        if(num_of_machines != num_of_machines_)
        {
            throw NumberOfMachinesMismatch(num_of_machines, num_of_machines_);
        }
        return true;
    }

    /**
     * @brief Get the jobs for this turn. It will check the tail_index_ and the timestamp
     * and fill in the job array.
     */
    std::vector<JobT> getJobs(int64_t timestamp)
    {
        std::vector<JobT> jobs;
        while((tail_index_ < job_array_.size()) && (timestamp >= job_array_[tail_index_].timestamp_))
        {
            // the time window is [job_array[tail_index_].timestamp, timestamp]
            jobs.push_back(job_array_[tail_index_]);
            tail_index_++;
        }
        if(jobs.empty())
        {
            return jobs;
        }

        NANO_LOG(DEBUG, "[JsonInputHandler::getJobs] printing the job array : ");
        for(auto & job : job_array_)
        {
            NANO_LOG(DEBUG, "[JsonInputHandler::getJobs] %s", job.toString().c_str());
        }

        return jobs;
    }

    /**
     * @brief Check if there's no more jobs.
     * 
     * @return true is there's no more jobs, false otherwise.
     */
    bool done() const
    {
        return tail_index_ >= job_array_.size();
    }

    /**
     * @brief For debug only
     */
    const std::vector<JobT> & getJobArray() const
    {
        return job_array_;
    }

private:
    std::string job_type_;      
    int64_t num_of_machines_;
    std::vector<JobT> job_array_;
    size_t tail_index_ = 0; // the next job element to be considered
};

}