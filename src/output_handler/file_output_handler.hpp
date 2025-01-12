#pragma once

#include <vector>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

#include "model_trait.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief The output handler that directly output the string representation
 *  of the schedule steps.
 * 
 * @tparam machine_model Identical / Related / Unrelated
 */
template <Machine_Model machine_model>
class FileOutputHandler
{
using JobT = typename MachineModelTraits<machine_model>::JobT;
using MachineT = typename MachineModelTraits<machine_model>::MachineT;

public:
    /**
     * @brief Initialize the std::ofstream based on the output path
     * 
     * @param config A json object, containing configuration information.
     * @param output_path The path of the output file.
     */
    FileOutputHandler(const json & config, std::string_view output_path)
    {
        output_file_.open(output_path.data(), std::ios::out | std::ios::trunc);
        if(!output_file_.is_open())
        {
            throw std::runtime_error("Failed to open file: " + std::string(output_path));
        }
    }

    /**
     * @brief Just dump the string representation of the schedule steps to the file.
     */
    void output(const std::vector<MachineT> & machines,
                const std::vector<JobT> & jobs,
                int64_t timestamp,
                const std::vector<ScheduleStep> & schedule_steps)
    {
        output_file_ << timestamp << '\n';
        for(auto & step : schedule_steps)
        {
            output_file_ << "Job : " << jobs[step.jobId_].toString() << "\n";
            output_file_ << "Machine : " << machines[step.machineId_].toString() << "\n";
        }
        output_file_ << "SnapShot :\n" <<  toSnapShot<MachineT>(machines) << "\n\n";
    }

    void output(const std::vector<MachineT> & machines,
                const std::vector<JobT> & jobs,
                const std::vector<ScheduleStep> & schedule_steps)
    {
        for(auto & step : schedule_steps)
        {
            output_file_ << "Job : " << jobs[step.jobId_].toString() << "\n";
            output_file_ << "Machine : " << machines[step.machineId_].toString() << "\n";
        }
    }

    void output(const int64_t timestamp) 
    {
        output_file_ << "\n\nTotal Executing Time : " << timestamp << "\n"; 
    }

private:
    std::ofstream output_file_;
};

}