#pragma once

#include "Definitions.h"

#include <vector>

class ISerializationAdapter
{
public:
	virtual ~ISerializationAdapter();

	virtual uint32_t Serialize(const byte* obj, std::vector<byte>& bytes) = 0;

	virtual uint32_t Deserialize(const std::vector<byte>& bytes, const byte* obj) = 0;
};