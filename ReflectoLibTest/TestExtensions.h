#pragma once

#include "Common/Definitions.h"
#include "Common/Utils/StringExt.h"

#include "CppUnitTest.h"

#include <vector>
#include <string>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<std::vector<Reflecto::byte>>(const std::vector<Reflecto::byte>& vector)
			{
				std::wstring elements = Reflecto::Utils::StringExt::Join<std::wstring>(vector, L",", [](Reflecto::byte b) { return std::to_wstring(b); });
				return Reflecto::Utils::StringExt::Format<std::wstring>(L"[%s]", elements.c_str());
			}
		}
	}
}