#pragma once

#include "Type.h"
#include "TypeExt.h"

namespace Reflecto
{
	namespace Reflection
	{
		template<class value_t>
		class TypeFactory
		{
		public:
			TypeFactory(const std::string& name)
				: _typeInfo(TypeExt::GetTypeInfo<value_t>())
				, _name(name)
				, _hash(TypeExt::GetTypeHash<value_t>())
			{

			}

			Type Build()
			{
				return Type{ _typeInfo, _name, _hash };
			}

		private:
			const std::type_info& _typeInfo;
			std::string _name;
			typehash_t _hash;
		};
	}
}