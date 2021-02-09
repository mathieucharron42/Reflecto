#pragma once

#include "Serialization/Serializer.h"
#include "Serialization/Writer/JsonSerializationWriter.h"

#include "Type/TypeDescriptor.h"

#include <string>

namespace Reflecto
{
	namespace Serialization
	{
		namespace SerializationStrategy
		{
			void SerializeInt32(const Serializer& serializer, const Type::TypeDescriptor& typeDesriptor, const void* value, ISerializationWriter& writer)
			{
				const int32_t& valInt = *reinterpret_cast<const int32_t*>(value);
				writer.WriteInteger32(valInt);
			}

			void SerializeString(const Serializer& serializer, const Type::TypeDescriptor& typeDesriptor, const void* value, ISerializationWriter& writer)
			{
				const std::string& valueStr = *reinterpret_cast<const std::string*>(value);
				writer.WriteString(valueStr);
			}

			template<class object_t>
			void SerializeObject(const Serializer& serializer, const Type::TypeDescriptor& typeDesriptor, const void* value, ISerializationWriter& writer)
			{
				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				Type::Resolver<object_t> resolver(typeDesriptor);
				for (const Type::MemberDescriptor& member : typeDesriptor.Members())
				{
					writer.WriteBeginObjectProperty(member.Name());
					const void* value = resolver.ResolveMember(valueObject, member);
					serializer.Serialize(member.Type(), value, writer);
					writer.WriteEndObjectProperty();
				}
			}

			template<class object_t>
			void SerializeCollection(const Serializer& serializer, const Type::TypeDescriptor& typeDesriptor, const void* value, ISerializationWriter& writer)
			{
				using element_t = typename object_t::value_type;

				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				for (const element_t& element : valueObject)
				{
					writer.WriteBeginArrayElement();
					serializer.Serialize(element, writer);
					writer.WriteEndArrayElement();
				}
			}
		}
	}
}