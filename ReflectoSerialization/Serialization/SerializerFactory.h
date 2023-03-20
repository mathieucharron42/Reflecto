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

			SerializerFactory(const Reflection::TypeLibrary& library)
				: _typeLibrary(library)
				, _format(SerializationFormat::Descriptive)
			{ }

			SerializerFactory& LearnType(const Reflection::TypeDescriptorPtr& type, const serialization_strategy_t& serializationStrategy, const deserialization_strategy_t& deserializationStrategy)
			{
				_strategies.insert({ type, strategies_t(serializationStrategy, deserializationStrategy) });
				return *this;
			}

			template<typename value_t, typename strategy_t>
			SerializerFactory& LearnType()
			{
				const Reflection::TypeDescriptorPtr& type = _typeLibrary.GetDescriptor<value_t>();

				serialization_strategy_t serializationStrategy = std::bind(&strategy_t::Serialize, type, _1, _2, _3);
				deserialization_strategy_t deserializationStrategy = std::bind(&strategy_t::Deserialize, type, _1, _2, _3);

				return LearnType<value_t>(serializationStrategy, deserializationStrategy);
			}

			template<class value_t>
			SerializerFactory& LearnType(serialization_strategy_t serializationStrategy, deserialization_strategy_t deserializationStrategy)
			{
				const Reflection::TypeDescriptorPtr type = _typeLibrary.GetDescriptor<value_t>();
				LearnType(type, serializationStrategy, deserializationStrategy);
				return *this;
			}

			SerializerFactory& SetFormat(SerializationFormat format)
			{
				_format = format;
				return *this;
			}

			Serializer Build()
			{
				return Serializer(_typeLibrary, _strategies, _format);
			}

		private:
			Reflection::TypeLibrary _typeLibrary;
			Serializer::strategy_map_t _strategies;
			SerializationFormat _format;
		};
	}
}