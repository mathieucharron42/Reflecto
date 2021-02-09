#pragma once

#include "TypeDescriptorType.h"
#include "TypeLibrary.h"

#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>

namespace Reflecto
{
	namespace Type
	{
		class TypeLibraryFactory
		{
		public:
			template<class value_t>
			TypeLibraryFactory& Add(const std::string& typeName)
			{
				const TypeDescriptorType type = TypeDescriptorTypeFactory<value_t>(typeName).Build();
				Add(type);
				return *this;
			}

			TypeLibraryFactory& Add(const TypeDescriptorType& type)
			{
				assert(std::none_of(_types.begin(), _types.end(), [&](const auto& t) { return t.Name() == type.Name(); }));
				assert(std::none_of(_types.begin(), _types.end(), [&](const auto& t) { return t.Hash() == type.Hash(); }));
				_types.push_back(type);
				return *this;
			}

			TypeLibrary Build()
			{
				return TypeLibrary(_types);
			}

		private:
			std::vector<TypeDescriptorType> _types;
		};
	}
}