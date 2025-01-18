#pragma once

#include <nlohmann/json.hpp>
#include "timer_trait.hpp"
#include "model_trait.hpp"

namespace SJF
{

using json = nlohmann::json;

/**
 * @brief The ticking timer. The elapsing time of each turn is just 1.
 */
template <Machine_Model machine_model>
class TickingTimer
{
using MachineT = typename MachineModelTraits<machine_model>::MachineT;

public:
    TickingTimer() {}

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