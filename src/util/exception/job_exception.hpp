#pragma once

#include <exception>
#include <string>
#include <string_view>

class InvalidJobType : public std::exception 
{
public:
    InvalidJobType(std::string_view job_type)
        : job_type_(job_type) {}

    const char* what() const noexcept override 
    {
        return ("Invalid job type: " + job_type_).c_str();
    }

private:
    std::string job_type_;
};

class InvalidUnrelatedJobProcessingTime : public std::exception 
{
public:
    InvalidUnrelatedJobProcessingTime(size_t size_of_processing_time, size_t expected_size)
        : size_of_processing_time_(size_of_processing_time),
          expected_size_(expected_size) {}

    const char* what() const noexcept override 
    {
        return ("Invalid processing time size: " + std::to_string(size_of_processing_time_) + ", expected size: " + std::to_string(expected_size_)).c_str();
    }

private:
    size_t size_of_processing_time_;
    size_t expected_size_;
};