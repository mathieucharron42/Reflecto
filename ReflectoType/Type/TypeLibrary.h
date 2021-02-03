#pragma once

#include "TypeDescriptorType.h"
#include "TypeExt.h"

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
			void Add(const TypeDescriptorType& type)
			{
				assert(!GetByHash(type.Hash()));
				assert(!GetByName(type.Name()));
				_types.push_back(type);
			}

			template<class type_t>
			const TypeDescriptorType* Get() const
			{
				return GetByHash(TypeExt::GetTypeHash<type_t>());
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