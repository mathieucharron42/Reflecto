#pragma once

#include "TypeDescriptorMember.h"
#include "TypeDescriptorMethod.h"

#include <string>
#include <vector>

using TypeHashType = std::size_t;

class TypeDescriptor
{
public:
	TypeDescriptor(TypeHashType typeHash, const std::vector<TypeDescriptorMember>& members)
		: _typeHash(typeHash)
		, _members(members)
	{ }

	TypeHashType TypeHash() const
	{
		return _typeHash;
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
	TypeHashType _typeHash;
	std::vector<TypeDescriptorMember> _members;
};
