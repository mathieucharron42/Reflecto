#pragma once

#include "Common/Definitions.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace Reflecto
{
    namespace IOExt
    {
        inline void WriteToFile(const std::string& file, const std::vector<char> bytes)
        {
            std::ofstream outstream;
            outstream.open(file, std::ios::out | std::ios::trunc | std::ios::binary);
            std::copy(bytes.begin(), bytes.end(), std::ostream_iterator<char>(outstream));
        }

        inline std::vector<char> ReadFromFile(const std::string& file)
        {
            std::vector<char> bytes;
            {
                std::ifstream fileReader;
                fileReader.open(file, std::ios::in | std::ios::binary);
                fileReader.unsetf(std::ios::skipws);
                std::copy(
                    std::istream_iterator<char>(fileReader),
                    std::istream_iterator<char>(),
                    std::back_inserter(bytes)
                );
            }
            return bytes;
        }

		template <typename stream_t>
		static uint32_t WriteBytes(const char* addr, uint32_t size, stream_t& stream)
		{
			stream.write(addr, size);
			return size;
		}

		template <typename type, typename stream_t>
		static uint32_t WriteBytes(const char* addr, stream_t& stream)
		{
			constexpr uint32_t size = sizeof(type);
			return WriteBytes<stream_t>(addr, size, stream);
		}

		template <typename type, typename stream_t>
		static uint32_t WriteBytes(const type& value, stream_t& stream)
		{
			const char* addr = reinterpret_cast<const char*>(&value);
			return WriteBytes<type>(addr, stream);
		}

		template <typename stream_t>
		static uint32_t ReadBytes(char* addr, uint32_t size, stream_t& stream)
		{
			stream.read(addr, size);
			return size;
		}

		template <typename type, typename stream_t>
		static uint32_t ReadBytes(char* addr, stream_t& stream)
		{
			constexpr uint32_t size = sizeof(type);
			return ReadBytes<stream_t>(addr, size, stream);
		}

		template <typename type, typename stream_t>
		static uint32_t ReadBytes(type& value, stream_t& stream)
		{
			char* addr = reinterpret_cast<char*>(&value);
			return ReadBytes<type>(addr, stream);
		}
    }
}