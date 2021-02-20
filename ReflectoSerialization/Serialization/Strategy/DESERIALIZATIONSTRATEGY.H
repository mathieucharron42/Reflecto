#pragma once

#include "Serialization/Serializer.h"
#include "Serialization/Reader/ISerializationReader.h"

#include "Type/TypeDescriptor.h"
#include "Type/MemberDescriptor.h"

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
				object_t& valueObject = *reinterpret_cast<object_t*>(value);
				Type::Resolver<object_t> resolver(typeDesriptor);

				reader.ReadBeginObject();
				{
					while (reader.HasObjectPropertyRemaining())
					{
						std::string propertyName;
						reader.ReadBeginObjectProperty(propertyName);
						{
							const Type::MemberDescriptor* memberDescriptor = typeDesriptor.GetMemberByName(propertyName);
							assert(memberDescriptor);
							if (memberDescriptor)
							{
								void* member = resolver.ResolveMember(valueObject, *memberDescriptor);
								serializer.Deserialize(memberDescriptor->Type(), member, reader);
							}
						}
						reader.ReadEndObjectProperty();
					}
				}
				reader.ReadEndObject();
			}

			template<class object_t>
			void DeserializeCollection(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				using element_t = typename object_t::value_type;

				object_t& collection = *reinterpret_cast<object_t*>(value);
				
				reader.ReadBeginArray();
				{
					while (reader.HasArrayElementRemaining())
					{
						uint32_t index;
						reader.ReadBeginArrayElement(index);
						{
							element_t element;
							serializer.Deserialize<element_t>(element, reader);
							collection.push_back(element);
						}
						reader.ReadEndArrayElement();
					}
				}
				reader.ReadEndArray();
			}

			template<class object_t>
			void DeserializeAssociativeCollection(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				using element_t = typename object_t::value_type;
				using key_t = typename object_t::key_type;
				using value_t = typename object_t::mapped_type;

				object_t& collection = *reinterpret_cast<object_t*>(value);

				reader.ReadBeginArray();
				{
					while (reader.HasArrayElementRemaining())
					{
						uint32_t index;
						reader.ReadBeginArrayElement(index);
						{
							reader.ReadBeginObject();
							{
								key_t key;
								value_t value;
								while (reader.HasObjectPropertyRemaining())
								{
									std::string propertyName;
									reader.ReadBeginObjectProperty(propertyName);
									{
										if (propertyName == "key")
										{
											serializer.Deserialize<key_t>(key, reader);
										}
										else if (propertyName == "value")
										{
											serializer.Deserialize<value_t>(value, reader);
										}
									}
									reader.ReadEndObjectProperty();
								}
								collection.insert({key, value});
							}
							reader.ReadEndObject();
						}
						reader.ReadEndArrayElement();
					}
				}
				reader.ReadEndArray();
			
			}
		}
	}
}