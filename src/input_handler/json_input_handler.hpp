#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

#include "json_parser.hpp"

template <typename JobT>
class JsonInputHandler
{
public:
    JsonInputHandler(std::string_view json_job_path)
    {
        nlohmann::json job_metadata = parseJsonFile(json_job_path);
        job_type_ = job_metadata["Type"];
        auto & jobs = job_metadata["Jobs"];
        if constexpr (std::is_same_v<JobT, UnrelatedJob>)
        {
            // TODO: to be continued
        }
        else
        {
            for(auto & job : jobs)
            {
                job_array_.emplace_back(job["timestamp"], job["workload"]);
            }
        }
    }

    bool assert(std::string_view )
    {
        // TODO: to be continued
        return true;
    }

    std::optional<std::vector<JobT>> getJobs(int64_t timestamp)
    {
        std::vector<JobT> jobs;
        while((tail_index_ < job_array_.size()) && (timestamp >= job_array_[tail_index_].timestamp_))
        {
            jobs.emplace_back(job_array_[tail_index_].timestamp_, job_array_[tail_index_].workload_);
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

private:
    std::string job_type_;
    std::vector<JobT> job_array_;
    size_t tail_index_ = 0;
};