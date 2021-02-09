#pragma once

#include "TypeDescriptorType.h"
#include "TypeExt.h"

namespace Reflecto
{
	namespace Type
	{
		template<class value_t>
		class TypeDescriptorTypeFactory
		{
		public:
			TypeDescriptorTypeFactory(const std::string& name)
				: _name(name)
				, _hash(TypeExt::GetTypeHash<value_t>())
			{

			}

			TypeDescriptorType Build()
			{
				return TypeDescriptorType{ _name, _hash };
			}

		private:
			std::string _name;
			typehash_t _hash;
		};
	}
}