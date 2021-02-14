#pragma once

#include "Serialization/Serializer.h"

#include "Type/TypeLibrary.h"

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
			{

			}

			SerializerFactory& LearnType(const Type::TypeDescriptorType& type, serialization_strategy_t serializationStrategy, deserialization_strategy_t deserializationStrategy)
			{
				_strategies.insert({ type, strategies_t(serializationStrategy, deserializationStrategy) });
				return *this;
			}

			template<class value_t>
			SerializerFactory& LearnType(serialization_strategy_t serializationStrategy, deserialization_strategy_t deserializationStrategy)
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				assert(type);
				return LearnType(*type, serializationStrategy, deserializationStrategy);
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