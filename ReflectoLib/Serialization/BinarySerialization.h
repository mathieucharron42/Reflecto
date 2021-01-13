#pragma once

#include "Common/Definitions.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
namespace Reflecto
{
    struct BinarySerialization
    {
        template <typename type>
        static uint32_t Serialize(const type& obj, std::vector<byte>& bytes)
        {
            const uint32_t objSize = sizeof(obj);

            const byte* begin = reinterpret_cast<const byte*>(&obj);
            const byte* end = begin + objSize;
            const auto dest = std::back_inserter(bytes);

            std::copy(begin, end, dest);

            return objSize;
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
            const uint32_t objSize = sizeof(obj);

            const std::vector<byte>::const_iterator begin = bytes.begin();
            const std::vector<byte>::const_iterator end = begin + objSize;
            byte* dest = reinterpret_cast<byte*>(&obj);

            std::copy(begin, end, dest);

            return objSize;
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
}