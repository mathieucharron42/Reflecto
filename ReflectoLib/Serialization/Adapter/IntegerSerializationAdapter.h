#pragma once

#include "Definitions.h"
#include "ISerializationAdapter.h"

#include <vector>

class IntegerBinarySerializationAdapter : public ISerializationAdapter
{
public:
	uint32_t Serialize(const byte* obj, std::vector<byte>& bytes) override
	{
		
	}

	uint32_t Deserialize(const std::vector<byte>& bytes, T& obj)
	{
		
	}
	
};