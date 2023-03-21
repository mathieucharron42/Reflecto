#pragma once

#include "Type/TypeDescriptor.h"
#include "Type/TypeExt.h"

#include <map>
#include <vector>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeDescriptor;

		class TypeLibrary
		{
		public:
			TypeLibrary(const std::vector<TypeDescriptorPtr>& typeDescriptors)
				: _typeDescriptors(typeDescriptors)
			{ }
			
			template<class value_t>
			TypeDescriptorPtr GetDescriptor() const
			{
				return GetDescriptorByHash(TypeExt::GetTypeHash<value_t>());
			}

			const TypeDescriptorPtr GetDescriptorByHash(const typehash_t& hash) const
			{
				std::vector<TypeDescriptorPtr>::const_iterator found = std::find_if(begin(_typeDescriptors), end(_typeDescriptors), [&](const auto& type) { return type->GetHash() == hash; });

				TypeDescriptorPtr type = found != _typeDescriptors.end() ? (*found) : nullptr;

				return type;
			}

			const TypeDescriptorPtr GetDescriptorByName(const std::string& name) const
			{
				std::vector<TypeDescriptorPtr>::const_iterator found = std::find_if(begin(_typeDescriptors), end(_typeDescriptors), [&](const auto& type) { return type->GetName() == name; });

				TypeDescriptorPtr type = found != _typeDescriptors.end() ? (*found) : nullptr;

				return type;
			}

		private:
			std::vector<TypeDescriptorPtr> _typeDescriptors;
		};
	}
}