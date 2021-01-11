#pragma once

#include "Definitions.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace io
{
    inline void WriteToFile(const std::string& file, const std::vector<byte> bytes)
    {
        std::ofstream outstream;
        outstream.open(file, std::ios::out | std::ios::trunc | std::ios::binary);
        std::copy(bytes.begin(), bytes.end(), std::ostream_iterator<byte>(outstream));
    }

    inline std::vector<byte> ReadFromFile(const std::string& file)
    {
        std::vector<byte> bytes;
        {
            std::ifstream fileReader;
            fileReader.open(file, std::ios::in | std::ios::binary);
            fileReader.unsetf(std::ios::skipws);
            std::copy(
                std::istream_iterator<byte>(fileReader),
                std::istream_iterator<byte>(),
                std::back_inserter(bytes)
            );
        }
        return bytes;
    }
}