#pragma once

#include "Definitions.h"
#include "Type.h"

#include <stdint.h>
#include <string>

class TypeDescriptorMember
{
public:
	TypeDescriptorMember(Type::HashType type, const std::string& name, byte offset)
		: _type(type)
		, _name(name)
		, _offset(offset)
	{ }

	const Type::HashType Type() const
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

	bool operator==(const TypeDescriptorMember& other) const
	{
		return _type == other._type 
			&& _name == other._name 
			&& _offset == other._offset;
	}

private:
	Type::HashType _type;
	std::string _name;
	uint32_t _offset;
};