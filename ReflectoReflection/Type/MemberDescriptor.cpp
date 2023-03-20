#include "MethodDescriptor.h"

#include "Type/TypeDescriptor.h"

#include "Common/Ensure.h"
#include "Utils/StringExt.h"

#include <string>

bool Reflecto::Reflection::MemberDescriptor::operator<(const MemberDescriptor& other) const
{
	return std::tie(_name, GetType()) < std::tie(other._name, GetType());
}

std::string Reflecto::Reflection::MemberDescriptor::ToString() const
{
	std::string typeStr = GetType() ? GetType()->ToString() : "unknown";
	return StringExt::Format<std::string>("{Type=%s,Name=%s,Offset=%u", typeStr.c_str(), _name.c_str(), _offset);
}
