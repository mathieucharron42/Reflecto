#include "ValueDescriptor.h"

#include "Type/TypeDescriptor.h"
#include "Utils/StringExt.h"

bool Reflecto::Reflection::ValueDescriptor::operator<(const ValueDescriptor& other) const
{
	return GetType() < other.GetType()
		&& _name < other._name
		&& _underlyingValue < other._underlyingValue;
}

std::string Reflecto::Reflection::ValueDescriptor::ToString() const
{
	std::string typeStr = GetType() ? GetType()->ToString() : "unknown";
	return StringExt::Format<std::string>("{Type=%s,Name=%s,Value=%llu", typeStr.c_str(), _name.c_str(), _underlyingValue);
}
