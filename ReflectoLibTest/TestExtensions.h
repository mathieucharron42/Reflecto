#pragma once

#include "Definitions.h"
#include "Utils/StringExt.h"

#include "CppUnitTest.h"

#include <vector>
#include <string>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<std::vector<byte>>(const std::vector<byte>& vector)
			{
				std::wstring elements = StringExt::Join<std::wstring>(vector, L",", [](byte b) { return std::to_wstring(b); });
				return StringExt::Format<std::wstring>(L"[%s]", elements.c_str());
			}
		}
	}
}