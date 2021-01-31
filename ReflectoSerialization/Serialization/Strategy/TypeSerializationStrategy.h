#pragma once

#include "Common/Definitions.h"

#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		class TypeSerializationStrategy
		{
		public:
			virtual void Serialize(const void* obj, std::vector<byte>& bytes) const = 0;
		};
	}
}
