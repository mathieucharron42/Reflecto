#include "ParameterDescriptor.h"

#include "Type/TypeDescriptor.h"

#include "Common/Ensure.h"
#include "Utils/StringExt.h"

bool Reflecto::Reflection::ParameterDescriptor::operator<(const ParameterDescriptor& other) const
{
	return std::tie(GetType(), _name) < std::tie(other.GetType(), other._name);
}

std::string Reflecto::Reflection::ParameterDescriptor::ToString() const
{
	std::string typeStr = GetType() ? GetType()->ToString() : "unknown";
	return StringExt::Format<std::string>("{Type=%s,Name=%s}", typeStr.c_str(), _name.c_str());
}
