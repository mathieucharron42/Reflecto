#pragma once

#include "Resolver.h"
#include "TypeDescriptor.h"

template <typename ObjectType>
class TypeDescriptorFactory
{
public:
	TypeDescriptorFactory()
		: _typeHash(typeid(ObjectType).hash_code())
		, _dummy()
	{

	}

	template <typename MemberPointerType, typename ObjectType>
	TypeDescriptorFactory& Register(typename MemberPointerType typename ObjectType::* memberPointer, const std::string& name)
	{
		const Type::HashType typeHash = Type::GetTypeHash<MemberPointerType>();

		const byte offset = Type::ComputeOffset(_dummy, memberPointer);
		
		_members.push_back(TypeDescriptorMember{ typeHash, name, offset });

		return *this;
	}

	TypeDescriptor Build()
	{
		return TypeDescriptor(_typeHash, _members);
	}

private:
	ObjectType _dummy;
	TypeHashType _typeHash;
	std::vector<TypeDescriptorMember> _members;
};