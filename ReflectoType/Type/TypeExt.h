#pragma once

#include "Common/Definitions.h"

#include <string>
#include <typeinfo>

namespace Reflecto
{
	namespace Type
	{
		using typehash_t = uint64_t;

		namespace TypeExt
		{
			template <typename type>
			std::string GetClassName()
			{
				std::string fullName = typeid(type).name();
				std::string clean = fullName.substr(fullName.find(" ") + 1);
				return clean;
			}

			template <typename type>
			typehash_t GetTypeHash()
			{
				return typeid(type).hash_code();
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