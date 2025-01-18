#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <type_traits>
#include <random>
#include <nlohmann/json.hpp>
#include "basic_utils_in_one_header.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief The input handler that generate the input jobs by random.
 * I use Possion distribution to generate the inter-arrival time.
 * And I just use uniform distribution to generate the processing time.
 * 
 * @tparam machine_model    Identical / Related / Unrelated
 */
template <Machine_Model machine_model>
class RandomInputHandler
{
public:
using JobT = typename MachineModelTraits<machine_model>::JobT;

    /**
     * @brief Generate the input jobs by random.
     */
    RandomInputHandler(const json & job_metadata)
    {
        job_type_ = job_metadata["Type"];
        num_of_machines_ = job_metadata["Num_of_Machines"];
        int num_jobs_lower_bound = job_metadata.value("Num_of_Jobs_Lower_Bound", 1);
        int num_jobs_upper_bound = job_metadata.value("Num_of_Jobs_Upper_Bound", 5);
        double lambda = job_metadata.value("Possion_Lambda", 2.0);
        int processing_time_lower_bound = job_metadata.value("Processing_Time_Lower_Bound", 1);
        int processing_time_upper_bound = job_metadata.value("Processing_Time_Upper_Bound", 10);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> num_jobs_dist(num_jobs_lower_bound, num_jobs_upper_bound);
        std::poisson_distribution<> poisson_dist(lambda);
        std::uniform_int_distribution<> processing_time_dist(processing_time_lower_bound, processing_time_upper_bound);
        int num_jobs = num_jobs_dist(gen);
        job_array_.reserve(num_jobs);

        if constexpr (std::is_same_v<JobT, UnrelatedJob>)
        {
            int num_machines = job_metadata["Num_of_Machines"];
            if(job_type_ != "Unrelated")
            {
                throw InvalidJobType(job_type_);
            }
            for(int i = 0; i < num_jobs; i++)
            {
                std::vector<int64_t> processing_time(num_machines);
                for(int j = 0; j < num_machines; j++)
                {
                    processing_time[j] = processing_time_dist(gen);
                }
                job_array_.emplace_back(poisson_dist(gen), std::move(processing_time), i);
            }
        }
        else
        {
            if(job_type_ != "Normal")
            {
                throw InvalidJobType(job_type_);
            }
            for(int i = 0; i < num_jobs; i++)
            {
                job_array_.emplace_back(poisson_dist(gen), processing_time_dist(gen), i);
            }
        }

        // Sort the job array by timestamp, and fill in the jobId field one by one.
        std::sort(job_array_.begin(), job_array_.end());
        for(size_t i = 0; i < job_array_.size(); i++)
        {
            job_array_[i].id_ = i;
        }

        NANO_LOG(DEBUG, "[RandomInputHandler::Constructor] printing the job array : ");
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

        NANO_LOG(DEBUG, "[RandomInputHandler::getJobs] printing the job array : ");
        for(auto & job : job_array_)
        {
            NANO_LOG(DEBUG, "[RandomInputHandler::getJobs] %s", job.toString().c_str());
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

    void refresh()
    {
        tail_index_ = 0;
    }
private:
    std::string job_type_;      
    int64_t num_of_machines_;
    std::vector<JobT> job_array_;
    size_t tail_index_ = 0; // the next job element to be considered
};

}