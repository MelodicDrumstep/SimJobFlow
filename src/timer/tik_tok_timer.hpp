#pragma once

#include <nlohmann/json.hpp>
#include "timer_trait.hpp"

using json = nlohmann::json;

class TikTokTimer
{
public:
    TikTokTimer(const json & config) : timestamp_(0) {}

    void tick() 
    { 
        timestamp_++; 
    }
    int64_t timestamp() 
    { 
        return timestamp_; 
    }
private:
    int64_t timestamp_;
};