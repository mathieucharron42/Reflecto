#pragma once

#include "Serialization/Reader/ISerializationReader.h"
#include "Serialization/Writer/ISerializationWriter.h"

#include "Common/Definitions.h"
#include "Common/Ensure.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeLibrary.h"

#include <cassert>
#include <map>
#include <memory>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		enum class SerializationFormat
		{
			Short,
			Descriptive
		};

		class Serializer
		{
		public:				
			using serialization_strategy_t = typename std::function<bool(const Serializer&, const void*, ISerializationWriter& writer)>;
			using deserialization_strategy_t = typename std::function<bool(const Serializer&, void*, ISerializationReader& reader)>;
			using any_cast_raw_strategy_t = typename std::function<void*(std::any&)>;
			using strategies_t = std::tuple<serialization_strategy_t, deserialization_strategy_t, any_cast_raw_strategy_t>;
			using strategy_map_t = std::map<Reflection::TypeDescriptorPtr, strategies_t>;

			Serializer(const Reflection::TypeLibrary& library, const strategy_map_t& strategy)
				: Serializer(library, strategy, SerializationFormat::Descriptive)
			{ }

			Serializer(const Reflection::TypeLibrary& library, const strategy_map_t& strategy, SerializationFormat serializationFormat)
				: _typeLibrary(library)
				, _strategies(strategy)
				, _serializationFormat(serializationFormat)
			{ }

			void SetSerializationFormat(SerializationFormat serializationFormat)
			{
				_serializationFormat = serializationFormat;
			}

			bool Serialize(const Reflection::TypeDescriptorPtr& type, const void* value, ISerializationWriter& writer) const
			{
				bool success = false;
				const serialization_strategy_t* strategy = GetSerializationStrategy(type);
				if (strategy)
				{
					success = Serialize(type, *strategy, value, writer);
				}
				return success;
			}

			template<typename value_t>
			bool Serialize(const value_t& value, ISerializationWriter& writer) const
			{
				bool success = false;
				Reflection::TypeDescriptorPtr type = _typeLibrary.GetDescriptor<value_t>();
				if (type)
				{
					success = Serialize(type, &value, writer);
				}
				return success;
			}

			bool RawSerialize(const Reflection::TypeDescriptorPtr& type, const void* value, ISerializationWriter& writer) const
			{
				bool success = false;
				const serialization_strategy_t* strategy = GetSerializationStrategy(type);
				if (strategy)
				{
					success = RawSerialize(type, *strategy, value, writer);
				}
				return success;
			}

			template<typename value_t>
			bool RawSerialize(const value_t& value, ISerializationWriter& writer) const
			{
				bool success = false;
				Reflection::TypeDescriptorPtr type = _typeLibrary.GetDescriptor<value_t>();
				success = RawSerialize(type, &value, writer);
				return success;
			}

			template<typename value_t>
			bool Deserialize(value_t& value, ISerializationReader& reader) const
			{
				bool success = false;
				Reflection::TypeDescriptorPtr type = _typeLibrary.GetDescriptor<value_t>();
				success = Deserialize(type, &value, reader);
				return success;
			}

			bool Deserialize(const Reflection::TypeDescriptorPtr& type, void* value, ISerializationReader& reader) const
			{
				bool success = false;
				const deserialization_strategy_t* strategy = GetDeserializationStrategy(type);
				if (ensure(strategy))
				{
					success = Deserialize(type, *strategy, value, reader);
				}
				return success;
			}

			template<typename value_t>
			bool RawDeserialize(value_t& value, ISerializationReader& reader) const
			{
				bool success = false;
				Reflection::TypeDescriptorPtr type = _typeLibrary.GetDescriptor<value_t>();
				if (type)
				{
					success = RawDeserialize(*type, &value, reader);
				}
				return success;
			}

			bool RawDeserialize(const Reflection::TypeDescriptorPtr& type, void* value, ISerializationReader& reader) const
			{
				bool success = false;
				const deserialization_strategy_t* strategy = GetDeserializationStrategy(type);
				if (ensure(strategy))
				{
					success = RawDeserialize(type, *strategy, value, reader);
				}
				return success;
			}

			bool RawDeserialize(const Reflection::TypeDescriptorPtr& type, std::any& value, ISerializationReader& reader) const
			{
				bool success = false;
				const deserialization_strategy_t* deserialization_strategy = GetDeserializationStrategy(type);
				const any_cast_raw_strategy_t* any_cast_raw_strategy = GetAnyCastRawStrategy(type);
				if (ensure(deserialization_strategy) && ensure(any_cast_raw_strategy))
				{
					success = RawDeserialize(type, *deserialization_strategy, *any_cast_raw_strategy, value, reader);
				}
				return success;
			}

		private:
			bool RawSerialize(const Reflection::TypeDescriptorPtr& type, const serialization_strategy_t& strategy, const void* value, ISerializationWriter& writer) const
			{
				return strategy(*this, value, writer);
			}

			bool Serialize(const Reflection::TypeDescriptorPtr& type, const serialization_strategy_t& strategy, const void* value, ISerializationWriter& writer) const
			{
				bool success = true;
				if (_serializationFormat == SerializationFormat::Descriptive)
				{
					success &= writer.WriteBeginObject();
					{
						if (type)
						{
							success &= writer.WriteBeginObjectProperty("type");
							{
								success &= writer.WriteString(type->GetName());
							}
							success &= writer.WriteEndObjectProperty();

							success &= writer.WriteBeginObjectProperty("value");
							{
								success &= RawSerialize(type, strategy, value, writer);
							}
							success &= writer.WriteEndObjectProperty();
						}
						else
						{
							success = false;
						}
					}
				}
				else if (_serializationFormat == SerializationFormat::Short)
				{
					success &= RawSerialize(type, strategy, value, writer);
				}
				return success;
			}

			bool RawDeserialize(const Reflection::TypeDescriptorPtr& type, const deserialization_strategy_t& deserialization_strategy, void* value, ISerializationReader& reader) const
			{
				// Deserialize at address
				return deserialization_strategy(*this, value, reader);
			}

			bool RawDeserialize(const Reflection::TypeDescriptorPtr& type, const deserialization_strategy_t& deserialization_strategy, const any_cast_raw_strategy_t& any_cast_raw_strategy, std::any& value, ISerializationReader& reader) const
			{
				// Instanciate to retrieve address
				value = *(type->GetConstructor()->NewWeakInstance());

				// Retrieve address
				void* address = any_cast_raw_strategy(value);

				// Deserialize at address
				return RawDeserialize(type, deserialization_strategy, address, reader);
			}

			bool Deserialize(const Reflection::TypeDescriptorPtr& type, const deserialization_strategy_t& strategy, void* value, ISerializationReader& reader) const
			{
				bool success = true;
				SerializationFormat currentFormatSerializationFormat = _serializationFormat;
				if (currentFormatSerializationFormat == SerializationFormat::Descriptive)
				{
					success &= reader.ReadBeginObject();
					{
						while (reader.HasObjectPropertyRemaining())
						{
							if (type)
							{
								std::string property;
								success &= reader.ReadBeginObjectProperty(property);
								{
									if (property == "type")
									{
										std::string actualType;
										success &= reader.ReadString(actualType);
										assert(type->GetName() == actualType);
									}
									else if (property == "value")
									{
										success &= RawDeserialize(type, strategy, value, reader);
									}
								}
								success &= reader.ReadEndObjectProperty();
							}
							else
							{
								success = false;
							}
						}
					}
					success &= reader.ReadEndObject();
				}
				else if (currentFormatSerializationFormat == SerializationFormat::Short)
				{
					success &= RawDeserialize(type, strategy, value, reader);
				}
				return success;
			}

			const serialization_strategy_t* GetSerializationStrategy(const Reflection::TypeDescriptorPtr& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &std::get<serialization_strategy_t>((*found).second) : nullptr;
			}

			const deserialization_strategy_t* GetDeserializationStrategy(const Reflection::TypeDescriptorPtr& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &std::get<deserialization_strategy_t>((*found).second) : nullptr;
			}

			const any_cast_raw_strategy_t* GetAnyCastRawStrategy(const Reflection::TypeDescriptorPtr& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &std::get<any_cast_raw_strategy_t>((*found).second) : nullptr;
			}

			Reflection::TypeLibrary _typeLibrary;
			strategy_map_t _strategies;
			SerializationFormat _serializationFormat;
		};
	}
}