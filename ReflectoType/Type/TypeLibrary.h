#pragma once

#include "Type/TypeDescriptorTypeFactory.h"
#include "Type/TypeDescriptorType.h"
#include "Type/TypeExt.h"


#include <assert.h>
#include <map>
#include <vector>

namespace Reflecto
{
	namespace Type
	{
		class TypeLibrary
		{
		public:
			TypeLibrary(const std::vector<TypeDescriptorType>& types)
				: _types(types)
			{

			}
			
			template<class type_t>
			const TypeDescriptorType* Get() const
			{
				return GetByHash(TypeExt::GetTypeHash<type_t>());
			}

			template<class type_t>
			TypeDescriptorType GetChecked() const
			{
				const TypeDescriptorType* found = Get<type_t>();
				assert(found);
				return *found;
			}

			const TypeDescriptorType* GetByHash(const typehash_t& hash) const
			{
				std::vector<TypeDescriptorType>::const_iterator found = std::find_if(begin(_types), end(_types), [&](const auto& type) { return type.Hash() == hash; });
				
				return found != _types.end() ? &(*found) : nullptr;
			}

			const TypeDescriptorType* GetByName(const std::string& name) const
			{
				std::vector<TypeDescriptorType>::const_iterator found = std::find_if(begin(_types), end(_types), [&](const auto& type) { return type.Name() == name; });
				return found != _types.end() ? &(*found) : nullptr;
			}

		private:
			std::vector<TypeDescriptorType> _types;
		};
	}
}