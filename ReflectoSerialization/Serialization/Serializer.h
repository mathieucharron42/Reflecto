#pragma once

#include "Serialization/Reader/ISerializationReader.h"
#include "Serialization/Writer/ISerializationWriter.h"

#include "Common/Definitions.h"
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
				assert(strategy);
				if (strategy)
				{
					Serialize(type, *strategy, value, writer);
				}
			}

			template<typename value_t>
			void Serialize(const value_t& value, ISerializationWriter& writer) const
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				assert(type); 
				Serialize(*type, &value, writer);
			}

			void Deserialize(const Type::TypeDescriptorType& type, void* value, ISerializationReader& reader) const
			{
				const deserialization_strategy_t* strategy = GetDeserializationStrategy(type);
				assert(strategy);
				if (strategy)
				{
					Deserialize(type, *strategy, value, reader);
				}
			}

			template<typename value_t>
			void Deserialize(const value_t& value, ISerializationReader& writer) const
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				assert(type);
				Deserialize(*type, &value, reader);
			}

		private:
			void Serialize(const Type::TypeDescriptorType& type, const serialization_strategy_t& strategy, const void* value, ISerializationWriter& writer) const
			{
				writer.WriteBeginObject();
				{
					writer.WriteBeginObjectProperty("type");
					{
						writer.WriteString(type.Name());
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
				//writer.WriteBeginObjectProperty("type");
				//writer.WriteString(type.Name());
				//writer.WriteEndObjectProperty();

				//writer.WriteBeginObjectProperty("value");
				//strategy(*this, value, writer);
				//writer.WriteEndObjectProperty();
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