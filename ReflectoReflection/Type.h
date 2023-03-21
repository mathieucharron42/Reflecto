#pragma once

#include "Type/TypeExt.h"

namespace Reflecto
{
	namespace Reflection
	{
		class Type
		{
		public:
			Type(const std::string& name, const std::type_info& typeInfo, typehash_t hash)
				: _name(name)
				, _typeInfo(&typeInfo)
				, _hash(hash)
			{
			}

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

			template<typename type_t>
			bool Is() const
			{
				return TypeExt::GetTypeHash<type_t>() == GetHash();
			}

			bool operator<(const Type& other) const
			{
				return std::tie(_hash, _name) < std::tie(other._hash, other._name);
			}

			std::string ToString() const
			{
				return StringExt::Format<std::string>("Type{Name=%s}", _name.c_str());
			}
		private:
			const std::type_info* _typeInfo;
			const std::string _name;
			typehash_t _hash;
		};
	}
}
