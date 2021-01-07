#pragma once

#include "../Definitions.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorTypeFactory.h"
#include "Utils/NonCopyable.h"

#include <assert.h>
#include <optional>

template<class object_t>
class Resolver : private NonCopyable
{
public:
	Resolver(const TypeDescriptor& typeDescriptor)
		: _typeDescriptor(typeDescriptor)
	{
		assert(TypeDescriptorTypeFactory<object_t>().Build() == typeDescriptor.Type());
	}

	
	template<typename member_t>
	member_t* ResolveMember(object_t& object, const std::string& memberName)
	{
		member_t* memberAddr = nullptr;
		const TypeDescriptorMember* memberDescriptor = _typeDescriptor.GetMemberByNameRecursive(memberName);
		if (memberDescriptor)
		{
			memberAddr = ResolveMember<member_t>(object, *memberDescriptor);
		}
		return memberAddr;
	}

	template<typename member_t>
	member_t* ResolveMember(object_t& object, const TypeDescriptorMember& memberDescriptor)
	{
		member_t* memberAddr = nullptr;
		byte* objRawAddr = reinterpret_cast<byte*>(&object);
		byte* memberRawAddr = objRawAddr + memberDescriptor.Offset();
		memberAddr = reinterpret_cast<member_t*>(memberRawAddr);
		return memberAddr;
	}

private:
	TypeDescriptor _typeDescriptor;
};