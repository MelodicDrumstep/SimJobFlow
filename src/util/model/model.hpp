#pragma once

/**
 * @brief enum class for 3 different models
 */
enum class Machine_Model : uint8_t
{
    Identical, Related, Unrelated
};

enum class Release_Model : uint8_t
{
    List_Arrival, Real_Time_Arrival
};