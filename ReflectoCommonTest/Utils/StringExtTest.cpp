#include "Utils/StringExt.h"

#include "TestCommon.h"

#include <CppUnitTest.h>

#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Utils
	{
		namespace Test
		{
			TEST_CLASS(StringExtTest)
			{
			public:
				TEST_METHOD(Join)
				{
					/////////////
					// Arrange
					static const std::string kSeparator = ",";
					
					static const std::vector<std::string> kElems0 = { };
					static const std::string kExpected0 = "";
					
					static const std::vector<std::string> kElems1 = { "Test" };
					static const std::string kExpected1 = "Test";

					static const std::vector<std::string> kElems4 = { "A", "B", "C", "D", "E" };
					static const std::string kExpected4 = "A,B,C,D,E";

					static const std::wstring kSeparatorWStr = L"|";
					static const std::wstring kExpectedWStr = L"Éléphant|Noël|Château";
					static const std::vector<std::wstring> kElemsWStr = { L"Éléphant", L"Noël", L"Château" };

					static const std::vector<int> kElemsProj = { 0, 1, 2, 3, 4 };
					static const auto kProj = [](int i) { return std::to_string(i * i); };
					const std::string kExpectedProj = "0,1,4,9,16";

					/////////////
					// Act
					const std::string actual0 = StringExt::Join(kElems0, kSeparator);
					const std::string actual1 = StringExt::Join(kElems1, kSeparator);
					const std::string actual4 = StringExt::Join(kElems4, kSeparator);
					const std::wstring actualWStr = StringExt::Join(kElemsWStr, kSeparatorWStr);
					const std::string actualProj = StringExt::Join(kElemsProj, kSeparator, kProj);

					/////////////
					// Assert
					Assert::AreEqual(kExpected0, actual0, L"Unexpected joined string");
					Assert::AreEqual(kExpected1, actual1, L"Unexpected joined string");
					Assert::AreEqual(kExpected4, actual4, L"Unexpected joined string");
					Assert::AreEqual(kExpectedWStr, actualWStr, L"Unexpected joined wstring");
					Assert::AreEqual(kExpectedProj, actualProj, L"Unexpected joined projected string");
				}

				TEST_METHOD(FormatString)
				{
					/////////////
					// Arrange
					const std::string kFormatStr = "\"%s\" is the best string";
					const std::string kParamStr = "potato";
					const std::string kExpectedStr = "\"potato\" is the best string";

					const std::string kFormatInteger = "%d is the best integer";
					const int32_t kParamInteger = 42;
					const std::string kExpectedInteger = "42 is the best integer";

					const std::string kFormatFloat = "%.0f is the best float";
					const float kParamFloat = 99.99999f;
					const std::string kExpectedFloat = "100 is the best float";

					const std::string kFormatMultiple = "%i is a nice integer, %.5f is a good float and \"%s\" is the best string";
					const std::string kExpectedMultiple = "42 is a nice integer, 99.99999 is a good float and \"potato\" is the best string";

					const std::wstring kFormatWString = L"Voici %i %ss %ss";
					const std::wstring kParamWString1 = L"éléphant";
					const std::wstring kParamWString2 = L"opiniâtre";
					const std::wstring kExpectedWString = L"Voici 42 éléphants opiniâtres";

					/////////////
					// Act
					const std::string actualStr = StringExt::Format(kFormatStr, kParamStr.c_str());
					const std::string actualInteger = StringExt::Format(kFormatInteger, kParamInteger);
					const std::string actualFloat = StringExt::Format(kFormatFloat, kParamFloat);
					const std::string actualMultiple = StringExt::Format(kFormatMultiple, kParamInteger, kParamFloat, kParamStr.c_str());
					const std::wstring actualWString = StringExt::Format(kFormatWString, kParamInteger, kParamWString1.c_str(), kParamWString2.c_str());

					/////////////
					// Assert
					Assert::AreEqual(kExpectedStr, actualStr, L"Unexpected formatted string");
					Assert::AreEqual(kExpectedInteger, actualInteger, L"Unexpected formatted string");
					Assert::AreEqual(kExpectedFloat, actualFloat, L"Unexpected formatted string");
					Assert::AreEqual(kExpectedMultiple, actualMultiple, L"Unexpected formatted string");
					Assert::AreEqual(kExpectedWString, actualWString, L"Unexpected formatted wstring");
				}

				TEST_METHOD(ToWstring)
				{
					/////////////
					// Arrange
					const std::wstring expectedWstr = L"Hello";
					const std::string inputStr = "Hello";
					
					/////////////
					// Act
					const std::wstring actualWStr = StringExt::ToWstring(inputStr);
					
					/////////////
					// Assert
					Assert::AreEqual(expectedWstr, actualWStr, L"Unexpected wstring");
				}

				TEST_METHOD(Tokenize)
				{
					/////////////
					// Arrange
					const std::string string1 = "a,b,c";
					const std::string separator1 = ",";
					const std::vector<std::string> expected1 = {"a", "b", "c"};

					const std::string string2 = "";
					const std::vector<std::string> expected2 = {};

					const std::string string3 = "a||ab||||abc||abcd";
					const std::string separator3 = "||";
					const std::vector<std::string> expected3 = {"a", "ab", "abc", "abcd"};

					const std::string string4 = "a,b,c,d";
					const std::string separator4 = ";";
					const std::vector<std::string> expected4 = { "a,b,c,d" };

					/////////////
					// Act
					const std::vector<std::string> actual1 = StringExt::Tokenize(string1, separator1);
					const std::vector<std::string> actual2 = StringExt::Tokenize(string2, separator1);
					const std::vector<std::string> actual3 = StringExt::Tokenize(string3, separator3);
					const std::vector<std::string> actual4 = StringExt::Tokenize(string4, separator4);

					/////////////
					// Assert
					Assert::AreEqual(expected1, actual1, L"Unexpected tokens");
					Assert::AreEqual(expected2, actual2, L"Unexpected tokens");
					Assert::AreEqual(expected3, actual3, L"Unexpected tokens");
					Assert::AreEqual(expected4, actual4, L"Unexpected tokens");
				}
			};
		}
	}
}