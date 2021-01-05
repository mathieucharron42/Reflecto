#pragma once

#include "Resolver.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorType.h"
#include "TypeDescriptorTypeFactory.h"

template <typename ObjectType>
class TypeDescriptorFactory
{
public:
	TypeDescriptorFactory()
		: TypeDescriptorFactory(nullptr)
	{

	}

	TypeDescriptorFactory(const TypeDescriptor* parentTypeDescriptor)
		: _type(TypeDescriptorTypeFactory<ObjectType>().Build())
		, _parent(parentTypeDescriptor)
		, _dummy()
	{

	}

	template <typename MemberPointerType, typename ObjectType>
	TypeDescriptorFactory& Register(typename MemberPointerType typename ObjectType::* memberPointer, const std::string& memberName)
	{
		const TypeDescriptorType memberType = TypeDescriptorTypeFactory<MemberPointerType>().Build();
		const byte memberOffset = TypeExt::ComputeOffset(_dummy, memberPointer);
		_members.push_back(TypeDescriptorMember{ memberType, memberName, memberOffset });

		return *this;
	}

	TypeDescriptor Build()
	{
		return TypeDescriptor(_type, _parent, _members);
	}

private:
	ObjectType _dummy;
	TypeDescriptorType _type;
	const TypeDescriptor* _parent;
	std::vector<TypeDescriptorMember> _members;
};