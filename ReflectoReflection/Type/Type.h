#pragma once

#include "TypeExt.h"

#include "Utils/StringExt.h"
#include "Utils/RelationalOperators.h"

#include <any>
#include <functional>

namespace Reflecto
{
	namespace Reflection
	{
		class Type : public RelationalOperators<Type>
		{
		public:
			Type(const std::type_info& typeInfo, const std::string& name, typehash_t hash)
				: _typeInfo(&typeInfo)
				, _name(name)
				, _hash(hash)
			{ }
			
			const std::type_info& GetInfo() const
			{
				return *_typeInfo;
			}

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
			const std::type_info* _typeInfo;
			std::string _name;
			typehash_t _hash;
		};
	}
}