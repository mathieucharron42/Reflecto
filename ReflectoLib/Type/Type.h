#pragma once

#include "Definitions.h"

#include <string>

namespace Type
{
	template <typename T>
	std::string GetClassName()
	{
		std::string fullName = typeid(T).name();
		std::string clean = fullName.substr(fullName.find(" ")+1);
		return clean;
	}

	template <typename ObjectType>
	std::size_t GetTypeHash()
	{
		return typeid(ObjectType).hash_code();
	}
}