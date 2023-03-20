#include "MethodDescriptor.h"

#include "Type/TypeDescriptor.h"

#include "Common/Ensure.h"
#include "Utils/StringExt.h"

#include <string>

std::string Reflecto::Reflection::MemberDescriptor::ToString() const
{
	return StringExt::Format<std::string>("{Type=%s,Name=%s,Offset=%u", GetType().ToString().c_str(), _name.c_str(), _offset);
}
