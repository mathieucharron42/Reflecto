#pragma once

#include "TypeExt.h"
#include "TypeDescriptorMember.h"
#include "TypeDescriptorMethod.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

class TypeDescriptor
{
public:
	TypeDescriptor(const TypeDescriptorType& type, const std::vector<TypeDescriptorMember>& members)
		: TypeDescriptor(type, nullptr, members)
	{ }

	TypeDescriptor(const TypeDescriptorType& type, const TypeDescriptor* parent, const std::vector<TypeDescriptorMember>& members)
		: _type(type)
		, _parent(parent)
		, _members(members)
	{ }

	const TypeDescriptorType& Type() const
	{
		return _type;
	}

	const std::vector<TypeDescriptorMember>& Members() const
	{
		return _members;
	}

	std::vector<TypeDescriptorMember> MemberResursive() const
	{
		std::vector<TypeDescriptorMember> memberRecursive;

		if (_parent)
		{
			std::vector<TypeDescriptorMember> parentMembers = _parent->MemberResursive();
			memberRecursive.insert(memberRecursive.end(), parentMembers.begin(), parentMembers.end());
		}

		memberRecursive.insert(memberRecursive.end(), _members.begin(), _members.end());
		
		return memberRecursive;
	}

	const TypeDescriptorMember* GetMemberByName(const std::string& name) const
	{
		auto found = std::find_if(_members.begin(), _members.end(), [&](const TypeDescriptorMember& member){
			return member.Name() == name;
		});

		return found != _members.end() ? &(*found) : nullptr;
	}

	const TypeDescriptorMember* GetMemberByNameRecursive(const std::string& name) const
	{
		const TypeDescriptorMember* member = nullptr;

		if (_parent)
		{
			member = _parent->GetMemberByNameRecursive(name);
		}

		if (!member)
		{
			member = GetMemberByName(name);
		}

		return member;
	}
private:
	TypeDescriptorType _type;
	const TypeDescriptor* _parent;
	std::vector<TypeDescriptorMember> _members;
};
