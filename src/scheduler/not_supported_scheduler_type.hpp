#pragma once

#include <type_traits>

#include "exceptions_in_one_header.hpp"

namespace SJF
{

class NotSupportedSchedulerType
{
    NotSupportedSchedulerType()
    {
        throw NotImplementedError();
    }
};

}