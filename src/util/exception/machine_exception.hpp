#pragma once

#include <exception>
#include <string>
#include <string_view>

class NumberOfMachinesMismatch : public std::exception 
{
public:
    NumberOfMachinesMismatch(size_t num_of_machines, size_t expected_num_of_machines)
        : num_of_machines_(num_of_machines), expected_num_of_machines_(expected_num_of_machines) {}

    const char* what() const noexcept override 
    {
        return ("Number of machines mismatch: " + std::to_string(num_of_machines_) + ", expected number of machines: " + std::to_string(expected_num_of_machines_)).c_str();
    }

private:
    size_t num_of_machines_;
    size_t expected_num_of_machines_;
};