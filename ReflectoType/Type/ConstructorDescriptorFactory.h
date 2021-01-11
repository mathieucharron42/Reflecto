#pragma once

#include "ConstructorDescriptor.h"

template <typename object_t>
class ConstructorDescriptorFactory
{
public:
	ConstructorDescriptorFactory()
	{
		_constructor = []() -> void*
		{
			return new object_t{};
		};
	}

	ConstructorDescriptor Build()
	{
		return ConstructorDescriptor{_constructor};
	}

private:
	ConstructorDescriptor::construction_func_t _constructor;
};

