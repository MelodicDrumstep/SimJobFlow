#pragma once

#include <concepts>
#include <string>
#include <vector>

namespace SJF
{
    template <typename T>
    static std::string toString(const std::vector<T> & v);

    template <typename T>
    concept HasToString = requires(const T& t) {
        { t.toString() } -> std::convertible_to<std::string>;
    };

    // Overload for types that have a toString() member function
    template <typename T>
    requires HasToString<T>
    static std::string toString(const std::vector<T>& v) {
        std::string str = "[";
        for (const auto& elem : v) {
            str += elem.toString() + ", ";
        }
        if (!v.empty()) {
            str.erase(str.size() - 2); // Remove the trailing ", "
        }
        str += "]";
        return str;
    }

    // Overload for types that can be converted using std::to_string
    template <typename T>
    requires (!HasToString<T> && requires(const T& t) { std::to_string(t); })
    static std::string toString(const std::vector<T>& v) {
        std::string str = "[";
        for (const auto& elem : v) {
            str += std::to_string(elem) + ", ";
        }
        if (!v.empty()) {
            str.erase(str.size() - 2); // Remove the trailing ", "
        }
        str += "]";
        return str;
    }
};