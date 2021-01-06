#pragma once

#include "Resolver.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorType.h"
#include "TypeDescriptorTypeFactory.h"

template <typename object_t>
class TypeDescriptorFactory
{
public:
	TypeDescriptorFactory()
		: TypeDescriptorFactory(nullptr)
	{

	}

	TypeDescriptorFactory(const TypeDescriptor* parentTypeDescriptor)
		: _type(TypeDescriptorTypeFactory<object_t>().Build())
		, _parent(parentTypeDescriptor)
		, _dummy()
	{

	}

	template <typename member_type, typename object_t>
	TypeDescriptorFactory& Register(typename member_type typename object_t::* memberPointer, const std::string& memberName)
	{
		const TypeDescriptorType memberType = TypeDescriptorTypeFactory<member_type>().Build();
		const byte memberOffset = type::ComputeOffset(_dummy, memberPointer);
		_members.push_back(TypeDescriptorMember{ memberType, memberName, memberOffset });

		return *this;
	}

	TypeDescriptor Build()
	{
		return TypeDescriptor(_type, _parent, _members);
	}

private:
	object_t _dummy;
	TypeDescriptorType _type;
	const TypeDescriptor* _parent;
	std::vector<TypeDescriptorMember> _members;
};