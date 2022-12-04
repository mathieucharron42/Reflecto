#pragma once

#include "Utils/CollectionExt.h"
#include "Utils/StringExt.h"

#include <CppUnitTest.h>

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<std::string>>(const std::vector<std::string>& vector)
{
	return Reflecto::StringExt::Format<std::wstring>(L"{%s}", Reflecto::StringExt::Join<std::wstring>(vector, L",", [](const std::string& str) {
		return Reflecto::StringExt::Format<std::wstring>(L"\"%s\"", Reflecto::StringExt::ToWstring(str).c_str());
	}).c_str());
}
