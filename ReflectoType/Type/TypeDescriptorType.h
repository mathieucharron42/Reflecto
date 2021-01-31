#pragma once

#include "TypeExt.h"
namespace Reflecto
{
	namespace Type
	{
		class TypeDescriptorType
		{
		public:
			TypeDescriptorType(const std::string& name, typehash_t hash)
				: _name(name)
				, _hash(hash)
			{

			}

			const std::string Name() const
			{
				return _name;
			}

			typehash_t Hash() const
			{
				return _hash;
			}

			bool operator==(const TypeDescriptorType& type) const
			{
				return _name == type._name && _hash == type._hash;
			}

			bool operator<(const  TypeDescriptorType& type) const
			{
				return _hash < type.Hash();
			}

		private:
			std::string _name;
			typehash_t _hash;
		};
	}
}