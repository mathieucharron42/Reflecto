#pragma once

#include "Type/MemberDescriptor.h"
#include "Type/MethodDescriptor.h"
#include "Type/Type.h"

#include <CppUnitTest.h>

#include <array>
#include <vector>

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<Reflecto::Reflection::Type>(const Reflecto::Reflection::Type& type)
{
	return Reflecto::Utils::StringExt::ToWstring(type.ToString());
}

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<Reflecto::Reflection::MemberDescriptor>>(const std::vector<Reflecto::Reflection::MemberDescriptor>& members)
{
	return Reflecto::Utils::StringExt::StringifyCollection<std::wstring>(members, [](Reflecto::Reflection::MemberDescriptor d) {
		return Reflecto::Utils::StringExt::ToWstring(d.ToString());
	});
}

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<Reflecto::Reflection::MethodDescriptor>>(const std::vector<Reflecto::Reflection::MethodDescriptor>& methods)
{
	return Reflecto::Utils::StringExt::StringifyCollection<std::wstring>(methods, [](Reflecto::Reflection::MethodDescriptor d) {
		return Reflecto::Utils::StringExt::ToWstring(d.ToString());
	});
}

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::array<uint32_t, 10>>(const std::array<uint32_t, 10>& a)
{

}