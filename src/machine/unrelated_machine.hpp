#pragma once

#include <vector>
#include <string>

struct UnrelatedMachine
{
    int32_t machineId;
};

struct UnrelatedMachineNode
{
    UnrelatedMachine() : machineId(-1) {}
    int32_t machineId;
    int32_t jobId;
    int64_t remaining_time_;
};