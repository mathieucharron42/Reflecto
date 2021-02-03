#pragma once

#include "TypeDescriptorType.h"
#include "TypeExt.h"

#include <map>

namespace Reflecto
{
	namespace Type
	{
		class TypeLibrary
		{
		public:
			void Add(const TypeDescriptorType& type)
			{
				_typesMap.insert(std::make_pair(type.Hash(), type));
			}

			template<class type_t>
			const TypeDescriptorType* Get() const
			{
				return Get(TypeExt::GetTypeHash<type_t>());
			}

			const TypeDescriptorType* Get(const typehash_t& hash) const
			{
				const type_map_t::const_iterator found = _typesMap.find(hash);
				return found != _typesMap.end() ? &(found->second) : nullptr;
			}

		private:
			using type_map_t = std::map<typehash_t, TypeDescriptorType>;
			type_map_t _typesMap;
		};
	}
}