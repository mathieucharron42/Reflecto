#pragma once

#include "Type.h"
#include "TypeExt.h"

#include "Common/Definitions.h"
#include "Utils/RelationalOperators.h"

#include <stdint.h>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class ValueDescriptor : public RelationalOperators<ValueDescriptor>
		{
		public:
			using underlying_vale_type = uint64_t;

			ValueDescriptor(Type type, const std::string& name, underlying_vale_type value)
				: _type(type)
				, _name(name)
				, _underlyingValue(value)
			{ }

			const Type& GetType() const
			{
				return _type;
			}

			const std::string& GetName() const
			{
				return _name;
			}

			template <typename enum_t>
			const enum_t GetValue() const
			{
				return static_cast<enum_t>(GetUnderlyingValue());
			}

			const underlying_vale_type GetUnderlyingValue() const
			{
				return _underlyingValue;
			}

			bool operator<(const ValueDescriptor& other) const
			{
				return _type < other._type
					&& _name < other._name
					&& _underlyingValue < other._underlyingValue;
			}

			std::string ToString() const
			{
				return StringExt::Format<std::string>("{Type=%s,Name=%s,Value=%llu", _type.ToString().c_str(), _name.c_str(), _underlyingValue);
			}

		private:
			Type _type;
			std::string _name;
			underlying_vale_type _underlyingValue;
		};
	}
}