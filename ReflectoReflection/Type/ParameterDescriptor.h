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
		using TypeDescriptorUniquePtr = std::unique_ptr<TypeDescriptor>;

		class ParameterDescriptor : RelationalOperators<ParameterDescriptor>
		{
		public:
			ParameterDescriptor(const TypeDescriptorPtr& type, const std::string& name);

			const TypeDescriptor& GetType() const;

			const std::string GetName() const;

			bool operator<(const ParameterDescriptor& other) const;

			std::string ToString() const;

		private:
			TypeDescriptorPtr _type;
			std::string _name;
		};
	}
}