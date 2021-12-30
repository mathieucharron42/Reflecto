#pragma once

#include "Serialization/Serializer.h"
#include "Serialization/Reader/ISerializationReader.h"
#include "Serialization/Writer/ISerializationWriter.h"

#include "Common/Ensure.h"
#include "Resolver/Resolver.h"
#include "Type/MemberDescriptor.h"

#include <cstdint>
#include <string>

namespace Reflecto
{
	namespace Serialization
	{
		struct Int32SerializationStrategy
		{
			static bool Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const int32_t& valInt = *static_cast<const int32_t*>(value);
				return writer.WriteInteger32(valInt);
			}

			static bool Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				int32_t& valInt = *static_cast<int32_t*>(value);
				return reader.ReadInteger32(valInt);
			}
		};

		struct StringSerializationStrategy
		{
			static bool Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const std::string& valueStr = *static_cast<const std::string*>(value);
				return writer.WriteString(valueStr);
			}

			static bool Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				std::string& valueStr = *static_cast<std::string*>(value);
				return reader.ReadString(valueStr);
			}
		};

		struct FloatSerializationStrategy
		{
			static bool Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const float& valueStr = *static_cast<const float*>(value);
				return writer.WriteFloat(valueStr);
			}

			static bool Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				float& valueStr = *static_cast<float*>(value);
				return reader.ReadFloat(valueStr);
			}
		};

		struct DoubleSerializationStrategy
		{
			static bool Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const double& valueStr = *static_cast<const double*>(value);
				return writer.WriteDouble(valueStr);
			}

			static bool Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				double& valueStr = *static_cast<double*>(value);
				return reader.ReadDouble(valueStr);
			}
		};

		struct BooleanSerializationStrategy
		{
			static bool Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				const bool& valueBoolean = *static_cast<const bool*>(value);
				return writer.WriteBoolean(valueBoolean);
			}

			static bool Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				bool& valueBoolean = *static_cast<bool*>(value);
				return reader.ReadBoolean(valueBoolean);
			}
		};

		template<class object_t>
		struct ObjectSerializationStrategy
		{
			static bool Serialize(const Reflection::TypeDescriptor& typeDesriptor, const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				bool success = true;
				const object_t& valueObject = *static_cast<const object_t*>(value);
				Reflection::Resolver<object_t> resolver(typeDesriptor);
				success &= writer.WriteBeginObject();
				{
					for (const Reflection::MemberDescriptor& member : typeDesriptor.GetMembers())
					{
						success &= writer.WriteBeginObjectProperty(member.GetName());
						{
							const void* value = resolver.ResolveMember(valueObject, member);
							success &= serializer.Serialize(member.GetType(), value, writer);
						}
						success &= writer.WriteEndObjectProperty();
					}
				}
				success &= writer.WriteEndObject();
				return success;
			}

			static bool Deserialize(const Reflection::TypeDescriptor& typeDesriptor, const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				bool success = true;
				object_t& valueObject = *static_cast<object_t*>(value);
				Reflection::Resolver<object_t> resolver(typeDesriptor);
				success &= reader.ReadBeginObject();
				{
					while (reader.HasObjectPropertyRemaining())
					{
						std::string propertyName;
						success &= reader.ReadBeginObjectProperty(propertyName);
						{
							const Reflection::MemberDescriptor* memberDescriptor = typeDesriptor.GetMemberByName(propertyName);
							if (ensure(memberDescriptor))
							{
								void* member = resolver.ResolveMember(valueObject, *memberDescriptor);
								success &= serializer.Deserialize(memberDescriptor->GetType(), member, reader);
							}
						}
						success &= reader.ReadEndObjectProperty();
					}
				}
				success &= reader.ReadEndObject();
				return success;
			}
		};

		template<class object_t>
		struct VectorSerializationStrategy
		{
			static bool Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				using element_t = typename object_t::value_type;

				bool success = true;
				const object_t& valueObject = *static_cast<const object_t*>(value);
				success &= writer.WriteBeginArray();
				{
					for (const element_t& element : valueObject)
					{
						success &= writer.WriteBeginArrayElement();
						{
							success &= serializer.Serialize(element, writer);
						}
						success &= writer.WriteEndArrayElement();
					}
				}
				success &= writer.WriteEndArray();
				return success;
			}

			static bool Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				using element_t = typename object_t::value_type;

				bool success = true;

				object_t& collection = *static_cast<object_t*>(value);
				success &= reader.ReadBeginArray();
				{
					while (reader.HasArrayElementRemaining())
					{
						uint32_t index;
						success &= reader.ReadBeginArrayElement(index);
						{
							element_t element;
							success &= serializer.Deserialize<element_t>(element, reader);
							collection.push_back(element);
						}
						success &= reader.ReadEndArrayElement();
					}
				}
				success &= reader.ReadEndArray();
				return success;
			}
		};

		template<class object_t>
		struct MapSerializationStrategy
		{
			static bool Serialize(const Serializer& serializer, const void* value, ISerializationWriter& writer)
			{
				using element_t = typename object_t::value_type;

				bool success = true;

				const object_t& valueObject = *static_cast<const object_t*>(value);
				success &= writer.WriteBeginArray();
				{
					for (const element_t& element : valueObject)
					{
						success &= writer.WriteBeginArrayElement();
						{
							success &= writer.WriteBeginObject();
							{
								success &= writer.WriteBeginObjectProperty("key");
								{
									success &= serializer.Serialize(element.first, writer);
								}
								success &= writer.WriteEndObjectProperty();
								success &= writer.WriteBeginObjectProperty("value");
								{
									success &= serializer.Serialize(element.second, writer);
								}
								success &= writer.WriteEndObjectProperty();
							}
							success &= writer.WriteEndObject();
						}
						success &= writer.WriteEndArrayElement();
					}
				}
				success &= writer.WriteEndArray();

				return success;
			}

			static bool Deserialize(const Serializer& serializer, void* value, ISerializationReader& reader)
			{
				using element_t = typename object_t::value_type;
				using key_t = typename object_t::key_type;
				using value_t = typename object_t::mapped_type;

				bool success = true;

				object_t& collection = *static_cast<object_t*>(value);
				success &= reader.ReadBeginArray();
				{
					while (reader.HasArrayElementRemaining())
					{
						uint32_t index;
						success &= reader.ReadBeginArrayElement(index);
						{
							success &= reader.ReadBeginObject();
							{
								key_t key;
								value_t value;
								while (reader.HasObjectPropertyRemaining())
								{
									std::string propertyName;
									success &= reader.ReadBeginObjectProperty(propertyName);
									{
										if (propertyName == "key")
										{
											success &= serializer.Deserialize<key_t>(key, reader);
										}
										else if (propertyName == "value")
										{
											success &= serializer.Deserialize<value_t>(value, reader);
										}
									}
									success &= reader.ReadEndObjectProperty();
								}
								collection.insert({ key, value });
							}
							success &= reader.ReadEndObject();
						}
						success &= reader.ReadEndArrayElement();
					}
				}
				success &= reader.ReadEndArray();
				return success;
			}
		};
	}
}