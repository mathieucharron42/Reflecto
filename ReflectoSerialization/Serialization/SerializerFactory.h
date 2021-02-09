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
			using SerializationStrategy = typename Serializer::serialization_strategy_t;

			SerializerFactory(const Type::TypeLibrary& library)
				: _typeLibrary(library)
			{

			}

			SerializerFactory& LearnType(const Type::TypeDescriptor& typeDescriptor, SerializationStrategy strategy)
			{
				const TypeInformation information = { typeDescriptor, strategy };
				_typeInformations.insert({ typeDescriptor.Type(), information });
				return *this;
			}

			Serializer Build()
			{
				return Serializer(_typeLibrary, _typeInformations);
			}

		private:
			using TypeInformation = typename Serializer::TypeInformation;
			
			Type::TypeLibrary _typeLibrary;
			std::map<Type::TypeDescriptorType, TypeInformation> _typeInformations;
		};
	}
}