#pragma once

#include <string>

namespace Reflecto
{
	namespace Type
	{
		class MethodDescriptor
		{
		public:
			MethodDescriptor(const std::string& name)
				: _name(name)
			{ }

			const std::string Name() const
			{
				return _name;
			}

			bool operator==(const MethodDescriptor& other) const
			{
				return _name == other._name;
			}
		private:
			std::string _name;
		};
	}
}