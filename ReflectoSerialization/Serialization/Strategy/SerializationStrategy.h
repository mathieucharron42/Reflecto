#pragma once

#include "Serialization/Serializer.h"

namespace Reflecto
{
	namespace Serialization
	{
		struct Int32SerializationStrategy
		{
			static void Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const int32_t& valInt = *reinterpret_cast<const int32_t*>(value);
				writer.WriteInteger32(valInt);
			}

			static void Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				int32_t& valInt = *reinterpret_cast<int32_t*>(value);
				reader.ReadInteger32(valInt);
			}
		};

		struct StringSerializationStrategy
		{
			static void Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const std::string& valueStr = *reinterpret_cast<const std::string*>(value);
				writer.WriteString(valueStr);
			}

			static void Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				std::string& valueStr = *reinterpret_cast<std::string*>(value);
				reader.ReadString(valueStr);
			}
		};

		template<class object_t>
		struct ObjectSerializationStrategy
		{
			static void Serialize(const Type::TypeDescriptor& typeDesriptor, const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				Type::Resolver<object_t> resolver(typeDesriptor);
				writer.WriteBeginObject();
				{
					for (const Type::MemberDescriptor& member : typeDesriptor.Members())
					{
						writer.WriteBeginObjectProperty(member.Name());
						{
							const void* value = resolver.ResolveMember(valueObject, member);
							serializer.Serialize(member.Type(), value, writer);
						}
						writer.WriteEndObjectProperty();
					}
				}
				writer.WriteEndObject();
			}

			static void Deserialize(const Type::TypeDescriptor& typeDesriptor, const Serializer& serializer, void* value, ISerializationReader& reader)
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
		};

		template<class object_t>
		struct VectorSerializationStrategy
		{
			static void Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				using element_t = typename object_t::value_type;

				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				writer.WriteBeginArray();
				{
					for (const element_t& element : valueObject)
					{
						writer.WriteBeginArrayElement();
						{
							serializer.Serialize(element, writer);
						}
						writer.WriteEndArrayElement();
					}
				}
				writer.WriteEndArray();
			}

			static void Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
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
		};

		template<class object_t>
		struct MapSerializationStrategy
		{
			static void Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				using element_t = typename object_t::value_type;

				const object_t& valueObject = *reinterpret_cast<const object_t*>(value);
				writer.WriteBeginArray();
				{
					for (const element_t& element : valueObject)
					{
						writer.WriteBeginArrayElement();
						{
							writer.WriteBeginObject();
							{
								writer.WriteBeginObjectProperty("key");
								{
									serializer.Serialize(element.first, writer);
								}
								writer.WriteEndObjectProperty();
								writer.WriteBeginObjectProperty("value");
								{
									serializer.Serialize(element.second, writer);
								}
								writer.WriteEndObjectProperty();
							}
							writer.WriteEndObject();
						}
						writer.WriteEndArrayElement();
					}
				}
				writer.WriteEndArray();
			}

			static void Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
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
								collection.insert({ key, value });
							}
							reader.ReadEndObject();
						}
						reader.ReadEndArrayElement();
					}
				}
				reader.ReadEndArray();
			}
		};
	}
}