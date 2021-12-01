#pragma once

#include "Type.h"
#include "TypeLibrary.h"

#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeLibraryFactory
		{
		public:
			template<class value_t>
			TypeLibraryFactory& Add(const std::string& typeName)
			{
				const Type type = TypeFactory<value_t>(typeName).Build();
				return Add(type);
			}

			TypeLibraryFactory& Add(const Type& type)
			{
				if (std::find(_types.begin(), _types.end(), type) == _types.end())
				{
					_types.push_back(type);
				}
				return *this;
			}

			TypeLibrary Build()
			{
				return TypeLibrary(_types);
			}

		private:
			
			std::vector<Type> _types;
		};
	}
}