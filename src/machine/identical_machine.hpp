#pragma once

#include <vector>
#include <memory>
#include <string>

struct IdenticalMachine
{
    int32_t machineId;
};

struct IdenticalMachineNode
{
    int32_t machineId;
    int32_t jobId;
    int64_t remaining_time_;
};