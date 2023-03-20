#include "ParameterDescriptor.h"

#include "Type/TypeDescriptor.h"

#include "Common/Ensure.h"
#include "Utils/StringExt.h"

Reflecto::Reflection::ParameterDescriptor::ParameterDescriptor(const TypeDescriptorPtr& type, const std::string& name) : _type(type)
, _name(name)
{

}

const Reflecto::Reflection::TypeDescriptor& Reflecto::Reflection::ParameterDescriptor::GetType() const
{
	ensure(_type);
	return *(_type);
}

const std::string Reflecto::Reflection::ParameterDescriptor::GetName() const
{
	return _name;
}

bool Reflecto::Reflection::ParameterDescriptor::operator<(const ParameterDescriptor& other) const
{
	return std::tie(GetType(), _name) < std::tie(other.GetType(), other._name);
}

std::string Reflecto::Reflection::ParameterDescriptor::ToString() const
{
	return StringExt::Format<std::string>("{Type=%s,Name=%s}", GetType().ToString().c_str(), _name.c_str());
}
