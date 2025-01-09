#pragma once

#include <vector>
#include <type_traits>
#include <cmath>
#include <queue>
#include <deque>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>

#include "basic_utils_in_one_header.hpp"

namespace SJF
{
using json = nlohmann::json;

/**
 * @brief MR scheduler of Identical machine_model, and list arrival release model.
 */
class MRSchedulerIdenticalListArrival
{

public:
    MRSchedulerIdenticalListArrival(const json & config) {}

    /**
     * @brief Initialize the machine state array and the machine state heap
     * machine state heap is represented as a std::vector
     *  And we will use std::make_heap / std::push_heap / std::pop_heap to manipulate it
     */
    void initialize(int64_t num_of_machines, const std::vector<IdenticalMachine> & machines)
    {
        assert(num_of_machines == machines.size());

        /*
        TODO: TO BE CONTINUED
        */
    }

    /**
     * @brief Schedule the jobs onto free machines and output schedule steps
     * 
     * @param jobs_for_this_turn It's assured that the jobs is sorted by the operator<, i.e.
     *      bool operator<(const NormalJob & other) const
            {
                return (timestamp_ < other.timestamp_) || (timestamp_ == other.timestamp_ && workload_ > other.workload_);
            }
     */
    std::vector<ScheduleStep> schedule(const std::vector<NormalJob> & jobs_for_this_turn,
                                       std::vector<IdenticalMachine> & machines,
                                       int64_t timestamp)
    {   
        NANO_LOG(DEBUG, "[MRSchedulerIdenticalListArrival::schedule] Inside schedule");

        std::vector<ScheduleStep> schedule_steps;
        size_t num_of_jobs = jobs_for_this_turn.size();
        if(num_of_jobs == 0)
        {
            return schedule_steps;
        }

        /*
        TODO: TO BE CONTINUED
        */

        NANO_LOG(DEBUG, "[MRSchedulerIdenticalListArrival::schedule] Outside schedule");
        return schedule_steps;
    }

    /**
     * @brief Modify the remaining time / total pending time of the busy machines
     */
    void updateMachineState(std::vector<IdenticalMachine> & machines, int64_t elapsing_time)
    {
        NANO_LOG(DEBUG, "[MRSchedulerIdenticalListArrival::updateMachineState] Inside updateMachineState");
        NANO_LOG(DEBUG, "elapsing time : %ld", elapsing_time);

        /*
        TODO: TO BE CONTINUED
        */

        NANO_LOG(DEBUG, "[MRSchedulerIdenticalListArrival::updateMachineState] Outside updateMachineState");
    }

    /**
     * @brief Check if there's no more jobs.
     * 
     * @return true is there's no more jobs, false otherwise.
     */
    bool done() const
    {
        return is_done_;
    }

private:
    /*
    TODO: TO BE CONTINUED
    */

    bool is_done_ = false;
};

}
