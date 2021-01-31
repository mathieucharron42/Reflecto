#pragma once

#include "Common/Definitions.h"

#include <vector>

namespace Reflecto
{
	namespace Type
	{
		class TypeDescriptorType;
	}

	namespace Serialization
	{
		class TypeSerializationStrategy;

		class ISerializationWriter
		{
		public:
			virtual void WriteBeginObject() = 0;

			virtual void WriteObjectType(const std::string& type) = 0;

			virtual void WriteObjectValue(const std::string& value) = 0;

			virtual void WriteEndObject() = 0;

			virtual void Transpose(std::vector<byte>& bytes) = 0;
		};
	}
}