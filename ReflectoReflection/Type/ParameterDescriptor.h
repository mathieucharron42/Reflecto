#pragma once

#include "Utils/RelationalOperators.h"

#include <memory>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeDescriptor;
		using TypeDescriptorPtr = std::shared_ptr<TypeDescriptor>;

		class ParameterDescriptor : RelationalOperators<ParameterDescriptor>
		{
		public:
			ParameterDescriptor(const TypeDescriptorPtr& type, const std::string& name)
				: _type(type)
				, _name(name)
			{

			}

			const TypeDescriptorPtr& GetType() const
			{
				return _type;
			}

			const std::string GetName() const
			{
				return _name;
			}

			bool operator<(const ParameterDescriptor& other) const;

			std::string ToString() const;

		private:
			TypeDescriptorPtr _type;
			std::string _name;
		};
	}
}