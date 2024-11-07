#pragma once

#include "timer_trait.hpp"

class TikTokTimer
{
public:
    TikTokTimer() : timestamp_(0) {}

    void tick() { timestamp_++; }
    int64_t timestamp() { return timestamp_; }
private:
    int64_t timestamp_;
};