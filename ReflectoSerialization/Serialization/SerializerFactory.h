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
			SerializerFactory(const Type::TypeLibrary& library)
				: _typeLibrary(library)
			{

			}

			Serializer<serialization_writer_t> Build()
			{
				return Serializer<serialization_writer_t>(_typeLibrary);
			}

		private:
			Type::TypeLibrary _typeLibrary;
		};
	}
}