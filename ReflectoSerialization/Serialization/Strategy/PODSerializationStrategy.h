#pragma once

#include "Common/Definitions.h"
#include "Serialization/Strategy/TypeSerializationStrategy.h"

#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		class PODSerializationStrategy : public TypeSerializationStrategy
		{
			virtual void Serialize(const void* obj, std::vector<byte>& bytes) const override
			{

			}
		};
	}
}