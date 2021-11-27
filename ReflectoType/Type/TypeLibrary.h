#pragma once

#include "Type/TypeFactory.h"
#include "Type/Type.h"
#include "Type/TypeExt.h"


#include <assert.h>
#include <map>
#include <vector>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeLibrary
		{
		public:
			TypeLibrary(const std::vector<Type>& types)
				: _types(types)
			{ }
			
			template<class value_t>
			const Type* Get() const
			{
				return GetByHash(TypeExt::GetTypeHash<value_t>());
			}

			template<class value_t>
			Type GetChecked() const
			{
				const Type* found = Get<value_t>();
				assert(found);
				return *found;
			}

			const Type* GetByHash(const typehash_t& hash) const
			{
				std::vector<Type>::const_iterator found = std::find_if(begin(_types), end(_types), [&](const auto& type) { return type.GetHash() == hash; });
				
				return found != _types.end() ? &(*found) : nullptr;
			}

			const Type* GetByName(const std::string& name) const
			{
				std::vector<Type>::const_iterator found = std::find_if(begin(_types), end(_types), [&](const auto& type) { return type.GetName() == name; });
				return found != _types.end() ? &(*found) : nullptr;
			}

		private:
			std::vector<Type> _types;
		};
	}
}