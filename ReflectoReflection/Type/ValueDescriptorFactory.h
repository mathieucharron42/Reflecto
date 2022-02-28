#pragma once

#include "ValueDescriptor.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"
#include "TypeLibrary.h"

#include <assert.h>

namespace Reflecto
{
	namespace Reflection
	{
		template <typename enum_t>
		class ValueDescriptorFactory
		{
		public:
			using enum_underlying_type = std::underlying_type_t<enum_t>;

			ValueDescriptorFactory(const TypeLibrary& typeLibrary, const enum_t& enumValue, const std::string& enumValueName)
				: _typeLibrary(typeLibrary)
				, _enum(enumValue)
				, _value(static_cast<enum_underlying_type>(enumValue))
				, _name(enumValueName)
			{ }

			ValueDescriptor Build()
			{
				const Type* type = _typeLibrary.Get<enum_t>();
				assert(type && "Unknown type!");
				return ValueDescriptor(*type, _name, _value);
			}

		private:
			TypeLibrary _typeLibrary;
			enum_t _enum;
			enum_underlying_type _value;
			std::string _name;
		};
	}
}