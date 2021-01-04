#pragma once

#include "TypeDescriptorType.h"

template<class type_t>
class TypeDescriptorTypeFactory
{
public:
	TypeDescriptorTypeFactory()
		: _name(TypeExt::GetClassName<type_t>())
		, _hash(TypeExt::GetTypeHash<type_t>())
	{

	}

	TypeDescriptorType Build()
	{
		return TypeDescriptorType{ _name, _hash };
	}

private:
	std::string _name;
	TypeExt::HashType _hash;
};