#pragma once

#include "TypeDescriptorType.h"
#include "TypeExt.h"

template<class type_t>
class TypeDescriptorTypeFactory
{
public:
	TypeDescriptorTypeFactory()
		: _name(type::GetClassName<type_t>())
		, _hash(type::GetTypeHash<type_t>())
	{

	}

	TypeDescriptorType Build()
	{
		return TypeDescriptorType{ _name, _hash };
	}

private:
	std::string _name;
	typehash_t _hash;
};