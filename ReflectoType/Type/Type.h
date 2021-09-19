#pragma once

#include "TypeExt.h"
namespace Reflecto
{
	namespace Reflection
	{
		class Type
		{
		public:
			Type(const std::string& name, typehash_t hash)
				: _name(name)
				, _hash(hash)
			{

			}

			const std::string GetName() const
			{
				return _name;
			}

			typehash_t GetHash() const
			{
				return _hash;
			}

			bool operator==(const Type& type) const
			{
				return _name == type._name && _hash == type._hash;
			}

			bool operator<(const  Type& type) const
			{
				return _hash < type.GetHash();
			}

		private:
			std::string _name;
			typehash_t _hash;
		};
	}
}