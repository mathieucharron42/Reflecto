#pragma once

#include "Common/Definitions.h"

#include <string>
#include <typeinfo>

namespace Reflecto
{
	namespace Reflection
	{
		using typehash_t = uint64_t;

		namespace TypeExt
		{
			template <typename type>
			constexpr const std::type_info& GetTypeInfo()
			{
				return typeid(type);
			}

			inline std::string GetClassName(const std::type_info& typeinfo)
			{
				std::string fullName = typeinfo.name();
				std::string clean = fullName.substr(fullName.find(" ") + 1);
				return clean;
			}

			template <typename type>
			std::string GetClassName()
			{
				return GetClassName(GetTypeInfo<type>());
			}

			inline typehash_t GetTypeHash(const std::type_info& typeinfo)
			{
				return typeinfo.hash_code();
			}

			template <typename type>
			typehash_t GetTypeHash()
			{
				return GetTypeHash(GetTypeInfo<type>());
			}

			template<typename object_t, typename member_pointer_owning_t, typename member_t>
			byte ComputeMemberOffset(const object_t& sampleObj, member_t typename member_pointer_owning_t::* memberPointer)
			{
				const byte* dummyAddr = reinterpret_cast<const byte*>(&sampleObj);
				const byte* memberAddr = reinterpret_cast<const byte*>(&(sampleObj.*memberPointer));
				const byte offset = static_cast<byte>(memberAddr - dummyAddr);
				return offset;
			}

			template<typename object_t, typename member_pointer_owning_t, typename member_t>
			byte ComputeMethodOffset(const object_t& sampleObj, member_t typename member_pointer_owning_t::* memberPointer)
			{
				const byte* dummyAddr = reinterpret_cast<const byte*>(&sampleObj);
				const byte* memberAddr = reinterpret_cast<const byte*>(&(sampleObj.*memberPointer));
				const byte offset = static_cast<byte>(memberAddr - dummyAddr);
				return offset;
			}
		}
	}
}