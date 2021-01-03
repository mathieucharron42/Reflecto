#pragma once

#include <string>

class TypeDescriptorMethod
{
public:
	TypeDescriptorMethod(const std::string& name)
		: _name(name)
	{ }

	const std::string Name() const
	{
		return _name;
	}

	bool operator==(const TypeDescriptorMethod& other) const
	{
		return _name == other._name;
	}
private:
	std::string _name;
};