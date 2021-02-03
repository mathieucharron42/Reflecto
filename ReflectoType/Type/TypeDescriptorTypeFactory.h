#pragma once

#include "TypeDescriptorType.h"
#include "TypeExt.h"

namespace Reflecto
{
	namespace Type
	{
		template<class type_t>
		class TypeDescriptorTypeFactory
		{
		public:
			TypeDescriptorTypeFactory()
				: TypeDescriptorTypeFactory(TypeExt::GetClassName<type_t>())
			{

			}

			TypeDescriptorTypeFactory(const std::string& name)
				: _name(name)
				, _hash(TypeExt::GetTypeHash<type_t>())
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