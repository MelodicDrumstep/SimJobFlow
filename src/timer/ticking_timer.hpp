#pragma once

#include <nlohmann/json.hpp>
#include "timer_trait.hpp"
#include "model_trait.hpp"
#include 

namespace SJF
{

using json = nlohmann::json;

template <Model model>
class TikTokTimer
{
using MachineT = typename ModelTraits<model>::MachineT;

public:
    TikTokTimer(const json & config) {}

    int64_t tick(const std::vector<MachineT> & machines) 
    { 
        timestamp_++; 
        return 1;
    }

    int64_t timestamp() 
    { 
        return timestamp_; 
    }
private:
    int64_t timestamp_{0};
};

}