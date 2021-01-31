#pragma once

#include "Serialization/SerializationMetaType.h"

#include <string>

namespace Reflecto
{
	namespace Serialization
	{
		namespace JsonSerializationStrategy
		{
			template<typename value_t>
			void Serialize(const value_t* value, std::string& serialized)
			{
				std::stringstream ss;
				if (value != nullptr)
				{
					ss << *value;
				}
				serialized = ss.str();
			}

			void SerializeInt(const void* value, TypeMetaType& metaType, std::string& serialized)
			{
				const int* valInt = reinterpret_cast<const int*>(value);
				metaType = TypeMetaType::Numerical;
				Serialize(valInt, serialized);
			}
		}
	}
}