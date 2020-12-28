#pragma once

#include "Definitions.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

struct TextSerialization
{
    template <typename T>
    static uint32_t Serialize(const T& obj, std::vector<byte>& bytes)
    {
        std::stringstream ss;
        ss << obj;
        std::string str = ss.str();
        return Serialize(str, bytes);
    }

    template <>
    static uint32_t Serialize<std::string>(const std::string& obj, std::vector<byte>& bytes)
    {
        const char* characters = obj.c_str();

        const byte* begin = reinterpret_cast<const byte*>(characters);
        const byte* end = begin + obj.length() + 1;
        const auto dest = std::back_inserter(bytes);

        std::copy(begin, end, dest);

        return static_cast<uint32_t>(obj.length() + 1);
    }

    template <typename T>
    static uint32_t Serialize(const byte* addr, std::vector<byte>& bytes)
    {
        const T& val = reinterpret_cast<const T&>(*addr);
        return Serialize<T>(val, bytes);
    }

    template <typename T>
    static uint32_t Deserialize(const std::vector<byte>& bytes, T& obj)
    {
        std::string str;
        uint32_t read = Deserialize(bytes, str);
        std::stringstream(str) >> obj;
        return read;
    }

    template <>
    static uint32_t Deserialize<std::string>(const std::vector<byte>& bytes, std::string& obj)
    {
        const char* characters = reinterpret_cast<const char*>(bytes.data());
        obj = std::string(characters);
        return static_cast<uint32_t>(obj.length() + 1);
    }

    template <typename T>
    static uint32_t Deserialize(const std::vector<byte>& bytes, byte* addr)
    {
        T& val = reinterpret_cast<T&>(*addr);
        return Deserialize<T>(bytes, val);
    }
};