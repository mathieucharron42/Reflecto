#pragma once

#include "TypeExt.h"
#include "ConstructorDescriptor.h"
#include "MemberDescriptor.h"
#include "MethodDescriptor.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

class TypeDescriptor
{
public:
	TypeDescriptor(const TypeDescriptorType& type, const ConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members)
		: TypeDescriptor(type, nullptr, constructor, members)
	{ }

	TypeDescriptor(const TypeDescriptorType& type, const TypeDescriptor* parent, const ConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members)
		: _type(type)
		, _parent(parent)
		, _constructor(constructor)
		, _members(members)
	{ }

	const TypeDescriptorType& Type() const
	{
		return _type;
	}

	const ConstructorDescriptor Constructor() const
	{
		return _constructor;
	}

	const std::vector<MemberDescriptor>& Members() const
	{
		return _members;
	}

	std::vector<MemberDescriptor> MemberResursive() const
	{
		std::vector<MemberDescriptor> memberRecursive;

		if (_parent)
		{
			std::vector<MemberDescriptor> parentMembers = _parent->MemberResursive();
			memberRecursive.insert(memberRecursive.end(), parentMembers.begin(), parentMembers.end());
		}

		memberRecursive.insert(memberRecursive.end(), _members.begin(), _members.end());
		
		return memberRecursive;
	}

	const MemberDescriptor* GetMemberByName(const std::string& name) const
	{
		auto found = std::find_if(_members.begin(), _members.end(), [&](const MemberDescriptor& member){
			return member.Name() == name;
		});

		return found != _members.end() ? &(*found) : nullptr;
	}

	const MemberDescriptor* GetMemberByNameRecursive(const std::string& name) const
	{
		const MemberDescriptor* member = nullptr;

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
	ConstructorDescriptor _constructor;
	std::vector<MemberDescriptor> _members;
};
