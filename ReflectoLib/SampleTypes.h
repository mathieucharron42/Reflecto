#pragma once

#include <cstdint>
#include <string>

class Simple
{
public:
    int32_t ValueInt32;
    uint64_t ValueUInt64;
};

#pragma pack(push, 1)
class SimpleNoPadding
{
public:
    int32_t ValueInt32;
    uint64_t ValueUInt64;
};
#pragma pack(pop)

class SimpleWithReference
{
public:
    int32_t ValueInt32;
    uint64_t ValueUInt64;
    std::string ValueStr;
};

class SimpleInheritance : public Simple
{
public:
    virtual void Method() { }
};