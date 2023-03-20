#include "MethodDescriptor.h"

#include "Type/TypeDescriptor.h"

#include "Common/Ensure.h"
#include "Utils/StringExt.h"

#include <string>

const std::string& Reflecto::Reflection::MethodDescriptor::GetName() const
{
	return _name;
}

const Reflecto::Reflection::TypeDescriptor& Reflecto::Reflection::MethodDescriptor::GetReturnType() const
{
	ensure(_returnType);
	return *(_returnType);
}

const std::vector<Reflecto::Reflection::ParameterDescriptor>& Reflecto::Reflection::MethodDescriptor::GetParameters() const
{
	return _parameters;
}

bool Reflecto::Reflection::MethodDescriptor::operator<(const MethodDescriptor& other) const
{
	return std::tie(_name, GetReturnType(), _parameters) < std::tie(other._name, other.GetReturnType(), other._parameters);
}

std::string Reflecto::Reflection::MethodDescriptor::ToString() const
{
	return StringExt::Format<std::string>("{ReturnType=%s,Name=%s,Parameters=%s}", GetReturnType().ToString().c_str(), _name.c_str(), StringExt::Join<std::string>(_parameters, ",", &ParameterDescriptor::ToString).c_str());
}
