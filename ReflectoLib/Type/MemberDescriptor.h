#pragma once

#include "Definitions.h"
#include "TypeDescriptorType.h"
#include "TypeExt.h"

#include <stdint.h>
#include <string>

class MemberDescriptor
{
public:
	MemberDescriptor(const TypeDescriptorType& type, const std::string& name, byte offset)
		: _type(type)
		, _name(name)
		, _offset(offset)
	{ }

	const TypeDescriptorType& Type() const
	{
		return _type;
	}

	const std::string& Name() const
	{
		return _name;
	}

	const uint32_t Offset() const
	{
		return _offset;
	}

	bool operator==(const MemberDescriptor& other) const
	{
		return _type == other._type 
			&& _name == other._name 
			&& _offset == other._offset;
	}

private:
	TypeDescriptorType _type;
	std::string _name;
	uint32_t _offset;
};