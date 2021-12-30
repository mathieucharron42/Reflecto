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
		class Serializer
		{
		public:				
			using serialization_strategy_t = typename std::function<bool(const Serializer&, const void*, ISerializationWriter& writer)>;
			using deserialization_strategy_t = typename std::function<bool(const Serializer&, void*, ISerializationReader& reader)>;
			using strategies_t = std::tuple<serialization_strategy_t, deserialization_strategy_t>;
			using strategy_map_t = std::map<Reflection::Type, strategies_t>;

			Serializer(const Reflection::TypeLibrary& library, const strategy_map_t& strategy)
				: _typeLibrary(library)
				, _strategies(strategy)
			{ }

			bool Serialize(const Reflection::Type& type, const void* value, ISerializationWriter& writer) const
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
				const Reflection::Type* type = _typeLibrary.Get<value_t>();
				if (type)
				{
					success = Serialize(*type, &value, writer);
				}
				return success;
			}

			bool RawSerialize(const Reflection::Type& type, const void* value, ISerializationWriter& writer) const
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
				const Reflection::Type* type = _typeLibrary.Get<value_t>();
				if (type)
				{
					success = RawSerialize(*type, &value, writer);
				}
				return success;
			}

			template<typename value_t>
			bool Deserialize(value_t& value, ISerializationReader& reader) const
			{
				bool success = false;
				const Reflection::Type* type = _typeLibrary.Get<value_t>();
				if (type)
				{
					success = Deserialize(*type, &value, reader);
				}
				return success;
			}

			bool Deserialize(const Reflection::Type& type, void* value, ISerializationReader& reader) const
			{
				bool success = false;
				const deserialization_strategy_t* strategy = GetDeserializationStrategy(type);
				if (strategy)
				{
					success = Deserialize(type, *strategy, value, reader);
				}
				return success;
			}

			template<typename value_t>
			bool RawDeserialize(value_t& value, ISerializationReader& reader) const
			{
				bool success = false;
				const Reflection::Type* type = _typeLibrary.Get<value_t>();
				if (type)
				{
					success = RawDeserialize(*type, &value, reader);
				}
				return success;
			}

			bool RawDeserialize(const Reflection::Type& type, void* value, ISerializationReader& reader) const
			{
				bool success = false;
				const deserialization_strategy_t* strategy = GetDeserializationStrategy(type);
				if (ensure(strategy))
				{
					success = RawDeserialize(type, *strategy, value, reader);
				}
				return success;
			}

		private:
			bool RawSerialize(const Reflection::Type& type, const serialization_strategy_t& strategy, const void* value, ISerializationWriter& writer) const
			{
				return strategy(*this, value, writer);
			}

			bool Serialize(const Reflection::Type& type, const serialization_strategy_t& strategy, const void* value, ISerializationWriter& writer) const
			{
				bool success = true;
				success &= writer.WriteBeginObject();
				{
					success &= writer.WriteBeginObjectProperty("type");
					{
						success &= writer.WriteString(type.GetName());
					}
					success &= writer.WriteEndObjectProperty();

					success &= writer.WriteBeginObjectProperty("value");
					{
						success &= RawSerialize(type, strategy, value, writer);
					}
					success &= writer.WriteEndObjectProperty();
				}
				return success;
			}

			bool RawDeserialize(const Reflection::Type& type, const deserialization_strategy_t& strategy, void* value, ISerializationReader& reader) const
			{
				return strategy(*this, value, reader);
			}

			bool Deserialize(const Reflection::Type& type, const deserialization_strategy_t& strategy, void* value, ISerializationReader& reader) const
			{
				bool success = true;
				success &= reader.ReadBeginObject();
				{
					while (reader.HasObjectPropertyRemaining())
					{
						std::string property;
						success &= reader.ReadBeginObjectProperty(property);
						{
							if (property == "type")
							{
								std::string actualType;
								success &= reader.ReadString(actualType);
								assert(type.GetName() == actualType);
							}
							else if (property == "value")
							{
								success &= RawDeserialize(type, strategy, value, reader);
							}
						}
						success &= reader.ReadEndObjectProperty();
					}
				}
				success &= reader.ReadEndObject();
				return success;
			}

			const serialization_strategy_t* GetSerializationStrategy(const Reflection::Type& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &std::get<serialization_strategy_t>((*found).second) : nullptr;
			}

			const deserialization_strategy_t* GetDeserializationStrategy(const Reflection::Type& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &std::get<deserialization_strategy_t>((*found).second) : nullptr;
			}

			Reflection::TypeLibrary _typeLibrary;
			strategy_map_t _strategies;
		};
	}
}