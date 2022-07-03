#pragma once

#include "Type.h"

#include "Utils/RelationalOperators.h"

#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class ParameterDescriptor : RelationalOperators<ParameterDescriptor>
		{
		public:
			ParameterDescriptor(const Type& type, const std::string& name)
				: _type(type)
				, _name(name)
			{ }

			const Type& GetType() const
			{
				return _type;
			}

			const std::string GetName() const
			{
				return _name;
			}

			bool operator<(const ParameterDescriptor& other) const
			{
				return std::tie(_type, _name) < std::tie(other._type, other._name);
			}

			std::string ToString() const
			{
				return StringExt::Format<std::string>("{Type=%s,Name=%s}", _type.ToString().c_str(), _name.c_str());
			}

		private:
			Type _type;
			std::string _name;
		};
	}
}