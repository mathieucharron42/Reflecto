#pragma once

#include "MemberDescriptor.h"
#include "MemberDescriptorFactory.h"
#include "Resolver.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorType.h"

template <typename object_t>
class TypeDescriptorFactory
{
public:
	TypeDescriptorFactory()
		: TypeDescriptorFactory(nullptr)
	{

	}

	TypeDescriptorFactory(const TypeDescriptor* parentTypeDescriptor)
		: _sampleObj()
		, _type(TypeDescriptorTypeFactory<object_t>().Build())
		, _parent(parentTypeDescriptor)
	{

	}

	template <typename member_t, typename object_t>
	TypeDescriptorFactory& Register(typename member_t typename object_t::* memberPointer, const std::string& memberName)
	{
		MemberDescriptor member = MemberDescriptorFactory<object_t, member_t>(_sampleObj)
			.SetMember(memberPointer, memberName)
		.Build();

		_members.push_back(member);

		return *this;
	}

	TypeDescriptor Build()
	{
		return TypeDescriptor(_type, _parent, _members);
	}

private:
	object_t _sampleObj;
	TypeDescriptorType _type;
	const TypeDescriptor* _parent;
	std::vector<MemberDescriptor> _members;
};