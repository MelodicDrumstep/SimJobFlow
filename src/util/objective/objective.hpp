#pragma once

namespace SJF
{

/**
 * @brief Objective type. Currently this is not in use and we only consider Makespan objective.
 */
enum class Objective
{
    Makespan, Total_Completion_Time, Total_Flow_Time
};

}