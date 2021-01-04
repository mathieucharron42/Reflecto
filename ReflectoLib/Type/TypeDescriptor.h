#pragma once

#include "TypeExt.h"
#include "TypeDescriptorMember.h"
#include "TypeDescriptorMethod.h"

#include <string>
#include <vector>

class TypeDescriptor
{
public:
	TypeDescriptor(const TypeDescriptorType& type, const std::vector<TypeDescriptorMember>& members)
		: _type(type)
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

	const TypeDescriptorMember* GetMemberByName(const std::string& name) const
	{
		auto found = std::find_if(_members.begin(), _members.end(), [&](const TypeDescriptorMember& member){
			return member.Name() == name;
		});

		return found != _members.end() ? &(*found) : nullptr;
	}

private:
	TypeDescriptorType _type;
	std::vector<TypeDescriptorMember> _members;
};
