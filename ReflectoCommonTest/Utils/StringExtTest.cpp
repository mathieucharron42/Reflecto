#include "Utils/StringExt.h"

#include "CppUnitTest.h"

#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Reflecto;

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
					const std::string actual = [] {
						const std::string separator = ",";
						const std::vector<std::string> elems = { "A", "B", "C", "D", "E" };
						return StringExt::Join(elems, separator);
					}();

					const std::string expected = "A,B,C,D,E";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(JoinProj)
				{
					const std::string actual = [] {
						const std::string separator = ",";
						const std::vector<int> elems = { 0, 1, 2, 3, 4 };
						auto proj = [](int i) { return std::to_string(i + 1); };
						return StringExt::Join(elems, separator, proj);
					}();

					const std::string expected = "1,2,3,4,5";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(JoinEmpty)
				{
					const std::string actual = [] {
						const std::string separator = ",";
						const std::vector<std::string> elems;
						return StringExt::Join(elems, separator);
					}();

					const std::string expected = "";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(Join1)
				{
					const std::string actual = [] {
						const std::string separator = ",";
						const std::vector<std::string> elems = { "Test" };
						return StringExt::Join(elems, separator);
					}();

					const std::string expected = "Test";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(JoinWString)
				{
					const std::wstring actual = [] {
						const std::wstring separator = L"|";
						const std::vector<std::wstring> elems = { L"Éléphant", L"Âne", L"Château" };
						return StringExt::Join(elems, separator);
					}();

					const std::wstring expected = L"Éléphant|Âne|Château";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(FormatString)
				{
					const std::string actual = [] {
						const std::string format = "This is a %s";
						const std::string arg1 = "POTATO";
						return StringExt::Format(format, arg1.c_str());
					}();

					const std::string expected = "This is a POTATO";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(FormatInteger)
				{
					const std::string actual = [] {
						const std::string format = "%i is the best integer";
						const int arg1 = 42;
						return StringExt::Format(format, arg1);
					}();

					const std::string expected = "42 is the best integer";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(FormatFloat)
				{
					const std::string actual = [] {
						const std::string format = "%.2f is the best float";
						const float arg1 = 99.99f;
						return StringExt::Format(format, arg1);
					}();

					const std::string expected = "99.99 is the best float";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(FormatMultiple)
				{
					const std::string actual = [] {
						const std::string format = "%i is a nice integer, %.5f is a good float and \"%s\" is the best string";
						const int arg1 = 42;
						const float arg2 = 3.14159f;
						const std::string arg3 = "potato";
						return StringExt::Format(format, arg1, arg2, arg3.c_str());
					}();

					const std::string expected = "42 is a nice integer, 3.14159 is a good float and \"potato\" is the best string";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}

				TEST_METHOD(FormatWString)
				{
					const std::wstring actual = [] {
						const std::wstring format = L"Here is %i éléphant %s";
						const int arg1 = 1;
						const std::wstring arg2 = L"âpre";
						return StringExt::Format(format, arg1, arg2.c_str());
					}();

					const std::wstring expected = L"Here is 1 éléphant âpre";
					Assert::AreEqual(expected, actual, L"Unexpected result");
				}
			};
		}
	}
}