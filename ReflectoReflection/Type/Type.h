#pragma once

#include "TypeExt.h"

#include "Utils/StringExt.h"
#include "Utils/RelationalOperators.h"

namespace Reflecto
{
	namespace Reflection
	{
		class Type : public RelationalOperators<Type>
		{
		public:
			Type(const std::string& name, typehash_t hash)
				: _name(name)
				, _hash(hash)
			{ }

			const std::string GetName() const
			{
				return _name;
			}

			typehash_t GetHash() const
			{
				return _hash;
			}

			bool operator<(const Type& other) const
			{
				return std::tie(_hash, _name) < std::tie(other._hash, other._name);
			}

			std::string ToString() const
			{
				return StringExt::Format<std::string>("Type{Name=%s,Hash=%llu}", _name.c_str(), _hash);
			}

		private:
			std::string _name;
			typehash_t _hash;
		};
	}
}