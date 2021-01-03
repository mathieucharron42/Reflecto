#pragma once

#include "../Definitions.h"
#include "TypeDescriptor.h"

#include <assert.h>

class Resolver
{
public:
	template<typename member_t, typename object_t>
	static member_t& Resolve(object_t& obj, const TypeDescriptorMember& member)
	{
		assert(Type::GetTypeHash<member_t>() == member.Type());
		byte* objAddr = reinterpret_cast<byte*>(&obj);
		byte* memberAddr = objAddr + member.Offset();
		return *reinterpret_cast<member_t*>(memberAddr);
	}
};