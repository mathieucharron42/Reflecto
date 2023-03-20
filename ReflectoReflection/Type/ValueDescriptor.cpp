#include "ValueDescriptor.h"

#include "Type/TypeDescriptor.h"
#include "Utils/StringExt.h"

Reflecto::Reflection::ValueDescriptor::ValueDescriptor(const TypeDescriptorPtr& type, const std::string& name, underlying_vale_type value)
	: _type(type)
	, _name(name)
	, _underlyingValue(value)
{

}

const Reflecto::Reflection::TypeDescriptor& Reflecto::Reflection::ValueDescriptor::GetType() const
{
	ensure(_type);
	return *(_type);
}

const std::string& Reflecto::Reflection::ValueDescriptor::GetName() const
{
	return _name;
}

const Reflecto::Reflection::ValueDescriptor::underlying_vale_type Reflecto::Reflection::ValueDescriptor::GetUnderlyingValue() const
{
	return _underlyingValue;
}

bool Reflecto::Reflection::ValueDescriptor::operator<(const ValueDescriptor& other) const
{
	return GetType() < other.GetType()
		&& _name < other._name
		&& _underlyingValue < other._underlyingValue;
}

std::string Reflecto::Reflection::ValueDescriptor::ToString() const
{
	return StringExt::Format<std::string>("{Type=%s,Name=%s,Value=%llu", GetType().ToString().c_str(), _name.c_str(), _underlyingValue);
}
