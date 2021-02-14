#pragma once

#include "Serialization/Serializer.h"
#include "Serialization/Reader/ISerializationReader.h"

#include "Type/TypeDescriptor.h"

#include <string>

namespace Reflecto
{
	namespace Serialization
	{
		namespace DeserializationStrategy
		{
			void DeserializeInt32(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				int32_t& valInt = *reinterpret_cast<int32_t*>(value);
				reader.ReadInteger32(valInt);
			}

			void DeserializeString(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				std::string& valueStr = *reinterpret_cast<std::string*>(value);
				reader.ReadString(valueStr);
			}

			template<class object_t>
			void DeserializeObject(const Type::TypeDescriptor& typeDesriptor, const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				/*const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				Type::Resolver<object_t> resolver(typeDesriptor);
				for (const Type::MemberDescriptor& member : typeDesriptor.Members())
				{
					reader.ReadBeginObjectProperty(member.Name());
					const void* value = resolver.ResolveMember(valueObject, member);
					serializer.Serialize(member.Type(), value, writer);
					reader.WriteEndObjectProperty();
				}*/
			}

			template<class object_t>
			void DeserializeCollection(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				/*using element_t = typename object_t::value_type;

				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				for (const element_t& element : valueObject)
				{
					writer.WriteBeginArrayElement();
					serializer.Serialize(element, writer);
					writer.WriteEndArrayElement();
				}*/
			}

			template<class object_t>
			void DeserializeAssociativeCollection(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				/*using element_t = typename object_t::value_type;

				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				for (const element_t& element : valueObject)
				{
					writer.WriteBeginArrayElement();
					writer.WriteBeginObjectProperty("key");
					serializer.Serialize(element.first, writer);
					writer.WriteEndObjectProperty();
					writer.WriteBeginObjectProperty("value");
					serializer.Serialize(element.second, writer);
					writer.WriteEndObjectProperty();
					writer.WriteEndArrayElement();
				}*/
			}
		}
	}
}