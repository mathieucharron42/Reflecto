#pragma once

#include "Serialization/Writer/JsonSerializationWriter.h"

#include "Type/TypeDescriptor.h"

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

			void SerializeInt32(const Serializer<JsonSerializationWriter> serializer, const Type::TypeDescriptor& typeDesriptor, const void* value, JsonSerializationWriter& writer)
			{
				const int32_t& valInt = *reinterpret_cast<const int32_t*>(value);
				writer.WriteInteger32(valInt);
			}

			void SerializeString(const Serializer<JsonSerializationWriter> serializer, const Type::TypeDescriptor& typeDesriptor, const void* value, JsonSerializationWriter& writer)
			{
				const std::string& valueStr = *reinterpret_cast<const std::string*>(value);
				writer.WriteString(valueStr);
			}

			template<class object_t>
			void SerializeObject(const Serializer<JsonSerializationWriter> serializer, const Type::TypeDescriptor& typeDesriptor, const void* value, JsonSerializationWriter& writer)
			{
				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				Type::Resolver<object_t> resolver{ typeDesriptor };
				for (const Type::MemberDescriptor& member : typeDesriptor.Members())
				{
					writer.WriteBeginObjectProperty(member.Name());
					const void* value = resolver.ResolveMember(valueObject, member);
					serializer.Serialize(member.Type(), value, writer);
					writer.WriteEndObjectProperty();
				}
			}
		}
	}
}