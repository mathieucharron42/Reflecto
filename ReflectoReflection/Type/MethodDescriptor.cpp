#include "MethodDescriptor.h"

#include "Type/TypeDescriptor.h"

#include "Common/Ensure.h"
#include "Utils/StringExt.h"

#include <string>

bool Reflecto::Reflection::MethodDescriptor::operator<(const MethodDescriptor& other) const
{
	return std::tie(_name, GetReturnType(), _parameters) < std::tie(other._name, other.GetReturnType(), other._parameters);
}

std::string Reflecto::Reflection::MethodDescriptor::ToString() const
{
	std::string returnTypeStr = GetReturnType() ? GetReturnType()->ToString() : "unknown";
	return StringExt::Format<std::string>("{ReturnType=%s,Name=%s,Parameters=%s}", returnTypeStr.c_str(), _name.c_str(), StringExt::Join<std::string>(_parameters, ",", &ParameterDescriptor::ToString).c_str());
}
