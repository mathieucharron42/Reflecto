#pragma once

#include "MemberDescriptor.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"

template <typename object_t, typename member_t>
class MemberDescriptorFactory
{
public:
	MemberDescriptorFactory(object_t& sampleObj)
		: _sampleObj(sampleObj)
	{ }

	MemberDescriptorFactory& SetMember(typename member_t object_t::* memberPointer, const std::string& memberName)
	{
		_name = memberName;
		_offset = TypeExt::ComputeOffset(_sampleObj, memberPointer);
		return *this;
	}

	MemberDescriptor Build()
	{
		TypeDescriptorType type = TypeDescriptorTypeFactory<member_t>().Build();

		return MemberDescriptor{ type, _name, _offset };
	}

private:
	object_t _sampleObj;
	std::string _name;
	byte _offset;
};