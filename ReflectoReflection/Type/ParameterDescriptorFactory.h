#pragma once

#include "TypeDescriptor.h"
#include "TypeLibrary.h"

#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		template <typename type_t>
		class ParameterDescriptorFactory
		{
		public:
			ParameterDescriptorFactory(const TypeLibrary& typeLibrary, const std::string& name)
				: _typeLibrary(typeLibrary)
				, _name(name)
			{ }

			ParameterDescriptor Build()
			{
				const Type type = *_typeLibrary.Get<type_t>();
				
				return ParameterDescriptor{ type, _name};
			}

		private:
			TypeLibrary _typeLibrary;
			std::string _name;
		};
	}
}