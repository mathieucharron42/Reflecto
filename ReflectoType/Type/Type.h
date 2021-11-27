#pragma once

#include "TypeExt.h"

#include "Utils/StringExt.h"

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

			std::string ToString() const
			{
				return Utils::StringExt::Format<std::string>("Type{Name=%s,Hash=%llu}", _name.c_str(), _hash);
			}

		private:
			std::string _name;
			typehash_t _hash;
		};
	}
}