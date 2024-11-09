#pragma once

#include <nlohmann/json.hpp>
#include "timer_trait.hpp"
#include "model_trait.hpp"

namespace SJF
{

using json = nlohmann::json;

template <Model model>
class TickingTimer
{
using MachineT = typename ModelTraits<model>::MachineT;

public:
    TickingTimer(const json & config) {}

    int64_t tick(const std::vector<MachineT> & machines) 
    { 
        timestamp_++; 
        return 1;
    }

    int64_t timestamp() const
    { 
        return timestamp_; 
    }
private:
    int64_t timestamp_{0};
};

}