#pragma once

#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

namespace SJF
{

nlohmann::json parseJsonFile(std::string_view json_config_path) 
{
    std::ifstream file(json_config_path.data());
    if(!file.is_open()) 
    {
        throw std::runtime_error("Failed to open file: " + std::string(json_config_path));
    }

    nlohmann::json config;
    file >> config;
    file.close();
    return config;
}
}