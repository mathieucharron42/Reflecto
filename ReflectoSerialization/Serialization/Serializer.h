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
			using serialization_strategy_t = typename std::function<void(const Serializer&, const void*, ISerializationWriter& writer)>;
			using deserialization_strategy_t = typename std::function<void(const Serializer&, void*, ISerializationReader& reader)>;
			using strategies_t = std::tuple<serialization_strategy_t, deserialization_strategy_t>;
			using strategy_map_t = std::map<Type::TypeDescriptorType, strategies_t>;

			Serializer(const Type::TypeLibrary& library, const strategy_map_t& strategy)
				: _typeLibrary(library)
				, _strategies(strategy)
			{ }

			void Serialize(const Type::TypeDescriptorType& type, const void* value, ISerializationWriter& writer) const
			{
				const serialization_strategy_t* strategy = GetSerializationStrategy(type);
				if (ensure(strategy))
				{
					Serialize(type, *strategy, value, writer);
				}
			}

			template<typename value_t>
			void Serialize(const value_t& value, ISerializationWriter& writer) const
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				if (ensure(type))
				{
					Serialize(*type, &value, writer);
				}
			}

			void Deserialize(const Type::TypeDescriptorType& type, void* value, ISerializationReader& reader) const
			{
				const deserialization_strategy_t* strategy = GetDeserializationStrategy(type);
				if (ensure(strategy))
				{
					Deserialize(type, *strategy, value, reader);
				}
			}

			template<typename value_t>
			void Deserialize(value_t& value, ISerializationReader& reader) const
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				if (ensure(type))
				{
					Deserialize(*type, &value, reader);
				}
			}

		private:
			void Serialize(const Type::TypeDescriptorType& type, const serialization_strategy_t& strategy, const void* value, ISerializationWriter& writer) const
			{
				writer.WriteBeginObject();
				{
					writer.WriteBeginObjectProperty("type");
					{
						writer.WriteString(type.GetName());
					}
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("value");
					{
						strategy(*this, value, writer);
					}
					writer.WriteEndObjectProperty();
				}
			}

			void Deserialize(const Type::TypeDescriptorType& type, const deserialization_strategy_t& strategy, void* value, ISerializationReader& reader) const
			{
				reader.ReadBeginObject();
				{
					while (reader.HasObjectPropertyRemaining())
					{
						std::string property;
						reader.ReadBeginObjectProperty(property);
						{
							if (property == "type")
							{
								std::string actualType;
								reader.ReadString(actualType);
								assert(type.GetName() == actualType);
							}
							else if (property == "value")
							{
								strategy(*this, value, reader);
							}
						}
						reader.ReadEndObjectProperty();
					}
				}
				reader.ReadEndObject();
			}

			const serialization_strategy_t* GetSerializationStrategy(const Type::TypeDescriptorType& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &std::get<serialization_strategy_t>((*found).second) : nullptr;
			}

			const deserialization_strategy_t* GetDeserializationStrategy(const Type::TypeDescriptorType& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &std::get<deserialization_strategy_t>((*found).second) : nullptr;
			}

			Type::TypeLibrary _typeLibrary;
			strategy_map_t _strategies;
		};
	}
}