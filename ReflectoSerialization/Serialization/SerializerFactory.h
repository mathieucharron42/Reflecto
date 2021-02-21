#pragma once

#include "Serialization/Serializer.h"

#include "Common/Ensure.h"
#include "Type/TypeLibrary.h"

using namespace std::placeholders;

namespace Reflecto
{
	namespace Serialization
	{
		class SerializerFactory
		{
		public:
			using serialization_strategy_t = typename Serializer::serialization_strategy_t;
			using deserialization_strategy_t = typename Serializer::deserialization_strategy_t;
			using strategies_t = typename Serializer::strategies_t;

			SerializerFactory(const Type::TypeLibrary& library)
				: _typeLibrary(library)
			{ }

			SerializerFactory& LearnType(const Type::TypeDescriptorType& type, const serialization_strategy_t& serializationStrategy, const deserialization_strategy_t& deserializationStrategy)
			{
				_strategies.insert({ type, strategies_t(serializationStrategy, deserializationStrategy) });
				return *this;
			}

			template<typename value_t, typename strategy_t, typename ... args_t>
			SerializerFactory& LearnType(args_t ... args)
			{
				serialization_strategy_t serializationStrategy = std::bind(&strategy_t::Serialize, args..., _1, _2, _3);
				deserialization_strategy_t deserializationStrategy = std::bind(&strategy_t::Deserialize, args..., _1, _2, _3);
				return LearnType<value_t>(serializationStrategy, deserializationStrategy);
			}

			template<class value_t>
			SerializerFactory& LearnType(serialization_strategy_t serializationStrategy, deserialization_strategy_t deserializationStrategy)
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				if (ensure(type))
				{
					LearnType(*type, serializationStrategy, deserializationStrategy);
				}
				return *this;
			}

			Serializer Build()
			{
				return Serializer(_typeLibrary, _strategies);
			}

		private:
			Type::TypeLibrary _typeLibrary;
			Serializer::strategy_map_t _strategies;
		};
	}
}