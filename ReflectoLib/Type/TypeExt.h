#pragma once

#include "Definitions.h"

#include <string>

using typehash_t = uint64_t;

namespace TypeExt
{
	template <typename T>
	std::string GetClassName()
	{
		std::string fullName = typeid(T).name();
		std::string clean = fullName.substr(fullName.find(" ")+1);
		return clean;
	}

	template <typename TypeExt>
	typehash_t GetTypeHash()
	{
		return typeid(TypeExt).hash_code();
	}

	template<typename ObjectType, typename MemberPointerType>
	byte ComputeOffset(const ObjectType& sampleObj, MemberPointerType typename ObjectType::* memberPointer)
	{
		const byte* dummyAddr = reinterpret_cast<const byte*>(&sampleObj);
		const byte* memberAddr = reinterpret_cast<const byte*>(&(sampleObj.*memberPointer));
		const byte offset = static_cast<byte>(memberAddr - dummyAddr);
		return offset;
	}
}