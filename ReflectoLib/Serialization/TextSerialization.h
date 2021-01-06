#pragma once

#include "Definitions.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

struct TextSerialization
{
    template <typename type>
    static uint32_t Serialize(const type& obj, std::vector<byte>& bytes)
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

    template <typename type>
    static uint32_t Serialize(const byte* addr, std::vector<byte>& bytes)
    {
        const type& val = reinterpret_cast<const type&>(*addr);
        return Serialize<type>(val, bytes);
    }

    template <typename type>
    static uint32_t Deserialize(const std::vector<byte>& bytes, type& obj)
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

    template <typename type>
    static uint32_t Deserialize(const std::vector<byte>& bytes, byte* addr)
    {
        type& val = reinterpret_cast<type&>(*addr);
        return Deserialize<type>(bytes, val);
    }
};