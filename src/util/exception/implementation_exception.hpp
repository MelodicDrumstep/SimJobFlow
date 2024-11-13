#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace SJF
{
class NotImplementedError : public std::exception 
{
public:
    NotImplementedError() {}

    const char* what() const noexcept override 
    {
        return "Not implemented.";
    }

private:
};

}