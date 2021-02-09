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

			SerializerFactory(const Type::TypeLibrary& library)
				: _typeLibrary(library)
			{

			}

			SerializerFactory& LearnType(const Type::TypeDescriptorType& type, serialization_strategy_t strategy)
			{
				_strategies.insert({ type, strategy });
				return *this;
			}

			template<class value_t>
			SerializerFactory& LearnType(serialization_strategy_t strategy)
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				assert(type);
				return LearnType(*type, strategy);
			}

			Serializer Build()
			{
				return Serializer(_typeLibrary, _strategies);
			}

		private:
			Type::TypeLibrary _typeLibrary;
			std::map<Type::TypeDescriptorType, serialization_strategy_t> _strategies;
		};
	}
}