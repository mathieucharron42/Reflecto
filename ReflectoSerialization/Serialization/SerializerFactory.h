#pragma once

#include "Serialization/Serializer.h"

#include "Type/TypeLibrary.h"

namespace Reflecto
{
	namespace Serialization
	{
		template<typename serialization_writer_t>
		class SerializerFactory
		{
		public:
			using SerializationStrategy = typename Serializer<serialization_writer_t>::serialization_strategy_t;

			SerializerFactory(const Type::TypeLibrary& library)
				: _typeLibrary(library)
			{

			}

			SerializerFactory<serialization_writer_t>& LearnType(const Type::TypeDescriptor& typeDescriptor, SerializationStrategy strategy)
			{
				const TypeInformation information = { typeDescriptor, strategy };
				_typeInformations.insert({ typeDescriptor.Type(), information });
				return *this;
			}

			Serializer<serialization_writer_t> Build()
			{
				return Serializer<serialization_writer_t>(_typeLibrary, _typeInformations);
			}

		private:
			using TypeInformation = typename Serializer<serialization_writer_t>::TypeInformation;
			
			Type::TypeLibrary _typeLibrary;
			std::map<Type::TypeDescriptorType, TypeInformation> _typeInformations;
		};
	}
}