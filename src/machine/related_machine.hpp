#pragma once

#include <vector>
#include <string>

struct RelatedMachine
{
    int32_t machineId;
}

struct RelatedMachineNode
{
    RelatedMachine() : machineId(-1) {}
    int64_t processing_speed_;
    int32_t machineId;
    int32_t jobId;
    int64_t remaining_time_;
};