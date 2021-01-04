#pragma once

#include "TypeExt.h"

class TypeDescriptorType
{
public:
	TypeDescriptorType(const std::string& name, TypeExt::HashType hash)
		: _name(name)
		, _hash(hash)
	{

	}

	const std::string Name() const
	{
		return _name;
	}

	TypeExt::HashType Hash() const
	{
		return _hash;
	}

	bool operator==(const TypeDescriptorType& type) const
	{
		return _name == type._name && _hash == type._hash;
	}

private:
	std::string _name;
	TypeExt::HashType _hash;
};