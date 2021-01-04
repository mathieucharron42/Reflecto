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

	void SetInstance(object_t& object)
	{
		_object = &object;
	}

	template<typename member_t>
	member_t* ResolveMember(const std::string& memberName)
	{
		member_t* memberAddr = nullptr;
		if (_object)
		{
			const TypeDescriptorMember* memberDescriptor = _typeDescriptor.GetMemberByName(memberName);
			if (memberDescriptor != nullptr)
			{
				byte* objRawAddr = reinterpret_cast<byte*>(_object);
				byte* memberRawAddr = objRawAddr + memberDescriptor->Offset();
				memberAddr = reinterpret_cast<member_t*>(memberRawAddr);
			}
		}
		return memberAddr;
	}

private:
	TypeDescriptor _typeDescriptor;
	object_t* _object;
};