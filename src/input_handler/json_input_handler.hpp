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
#include "normal_job.hpp"
#include "unrelated_job.hpp"
#include "exceptions_in_one_header.hpp"

template <typename JobT>
class JsonInputHandler
{
public:
    JsonInputHandler(std::string_view json_job_path)
    {
        nlohmann::json job_metadata = parseJsonFile(json_job_path);
        job_type_ = job_metadata["Type"];
        auto & jobs = job_metadata["Jobs"];
        num_of_machines_ = job_metadata["Num_of_Machines"];
        if constexpr (std::is_same_v<JobT, UnrelatedJob>)
        {
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
            for(auto & job : jobs)
            {
                job_array_.emplace_back(job["timestamp"], job["workload"]);
            }
        }
        std::sort(job_array_.begin(), job_array_.end());
    }

    bool checkValidity(std::string_view job_type, int64_t num_of_machines)
    {
        if(job_type != job_type_)
        {
            throw InvalidJobType(job_type);
        }
        if(num_of_machines != num_of_machines_)
        {
            throw NumberOfMachinesMismatch(num_of_machines, num_of_machines_);
        }
        return true;
    }

    std::optional<std::vector<JobT>> getJobs(int64_t timestamp)
    {
        std::vector<JobT> jobs;
        while((tail_index_ < job_array_.size()) && (timestamp >= job_array_[tail_index_].timestamp_))
        {
            jobs.push_back(job_array_[tail_index_]);
            tail_index_++;
        }
        if(jobs.empty())
        {
            return std::nullopt;
        }
        return jobs;
    }

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
    size_t tail_index_ = 0;
};