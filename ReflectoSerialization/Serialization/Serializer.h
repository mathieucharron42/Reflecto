#pragma once

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
			using strategy_map_t = std::map<Type::TypeDescriptorType, serialization_strategy_t>;

			Serializer(const Type::TypeLibrary& library, const strategy_map_t& strategy)
				: _typeLibrary(library)
				, _strategies(strategy)
			{ }


			template<typename serialization_writer_t>
			void Serialize(const Type::TypeDescriptorType& type, const void* value, serialization_writer_t& writer) const
			{
				const serialization_strategy_t* strategy = GetStrategy(type);
				assert(strategy);
				if (strategy)
				{
					Serialize(type, *strategy, value, writer);
				}
			}

			template<typename value_t, typename serialization_writer_t>
			void Serialize(const value_t& value, serialization_writer_t& writer) const
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				assert(type); 
				Serialize(*type, &value, writer);
			}

		private:
			template<typename serialization_writer_t>
			void Serialize(const Type::TypeDescriptorType& type, const serialization_strategy_t& strategy, const void* value, serialization_writer_t& writer) const
			{
				writer.WriteBeginObjectProperty("type");
				writer.WriteString(type.Name());
				writer.WriteEndObjectProperty();

				writer.WriteBeginObjectProperty("value");
				strategy(*this, value, writer);
				writer.WriteEndObjectProperty();
			}

			const serialization_strategy_t* GetStrategy(const Type::TypeDescriptorType& type) const
			{
				strategy_map_t::const_iterator found = _strategies.find(type);
				return found != _strategies.end() ? &(*found).second : nullptr;
			}

			Type::TypeLibrary _typeLibrary;
			strategy_map_t _strategies;
		};
	}
}