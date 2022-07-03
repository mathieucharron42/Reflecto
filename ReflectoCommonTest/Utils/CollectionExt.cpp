#include "Utils/CollectionExt.h"
#include "Utils/StringExt.h"
#include "Utils/NonCopyable.h"

#include <CppUnitTest.h>

#include <array>
#include <tuple>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<std::string>>(const std::vector<std::string>& vector)
{
	return Reflecto::StringExt::Join<std::wstring>(vector, L",", [] (const std::string& str) {
		return Reflecto::StringExt::ToWstring(str);
	});
}

namespace Reflecto
{
	namespace Utils
	{
		namespace Test
		{
			TEST_CLASS(CollectionExtTest)
			{
			public:
				TEST_METHOD(ArrayToTuple)
				{
					std::array<int32_t, 3> array = {42,5,29};
					std::tuple<int32_t, int32_t, int32_t> tuple = CollectionExt::ArrayToTuple(array);
					
					Assert::AreEqual(std::get<0>(array), std::get<0>(tuple), L"Element are unexpectedly different");
					Assert::AreEqual(std::get<1>(array), std::get<1>(tuple), L"Element are unexpectedly different");
					Assert::AreEqual(std::get<2>(array), std::get<2>(tuple), L"Element are unexpectedly different");
				}

				TEST_METHOD(ArrayToTupleProjection)
				{
					std::array<int32_t, 3> array = { 42, 5, 29 };
					std::tuple<std::string, std::string, std::string> tuple = CollectionExt::ArrayToTupleProjection(array, [] (int32_t i) {
						return std::to_string(i+1);
					});

					Assert::AreEqual(std::string("43"), std::get<0>(tuple), L"Element 0 is unexpected");
					Assert::AreEqual(std::string("6"), std::get<1>(tuple), L"Element 1 is unexpected");
					Assert::AreEqual(std::string("30"), std::get<2>(tuple), L"Element 2 is unexpected");
				}

				TEST_METHOD(ArrayToTupleMultipleProjection)
				{
					std::array<int32_t, 3> array = { 42, 5, 29 };
					auto projections = std::make_tuple(
						[] (int32_t i) { return std::to_string(i);},
						[] (int32_t i) { return i;},
						[] (int32_t i) { return static_cast<float>(i);}
					);
					std::tuple<std::string, int32_t, float> tuple = CollectionExt::ArrayToTupleProjections(array, projections);

					Assert::AreEqual(std::string("42"), std::get<0>(tuple), L"Element 0 is unexpected");
					Assert::AreEqual(5, std::get<1>(tuple), L"Element 1 is unexpected");
					Assert::AreEqual(29.f, std::get<2>(tuple), L"Element 2 is unexpected");
				}


				TEST_METHOD(Concatenate)
				{
					/////////////
					// Arrange
					std::vector<std::string> vector1 = { "Here", "are", "some", "strings" };
					std::vector<std::string> vector2 = { "A", "few", "more", "elements" };
					std::vector<std::string> vector3 = { "And", "a", "little", "more" };
					std::vector<std::string> vector4 = { };

					std::vector<std::string> kExpected1And2 = { "Here", "are", "some", "strings", "A", "few", "more", "elements" };
					std::vector<std::string> kExpected3And1 = { "And", "a", "little", "more", "Here", "are", "some", "strings" };
					std::vector<std::string> kExpected4 = { };
					std::vector<std::string> kExpected1And2And3And4 = { "Here", "are", "some", "strings", "A", "few", "more", "elements", "And", "a", "little", "more"};

					/////////////
					// Act
					std::vector<std::string> actual1And2 = CollectionExt::Concatenate<std::string>(vector1, vector2);
					std::vector<std::string> actual3And1 = CollectionExt::Concatenate<std::string>(vector3, vector1);
					std::vector<std::string> actual4 = CollectionExt::Concatenate(vector4);
					std::vector<std::string> actual1And2And3And4 = CollectionExt::Concatenate(vector1, vector2, vector3, vector4);

					/////////////
					// Assert
					Assert::AreEqual(kExpected1And2, actual1And2, L"Unexpected concatenation");
					Assert::AreEqual(kExpected3And1, actual3And1, L"Unexpected concatenation");
					Assert::AreEqual(kExpected4, actual4, L"Unexpected concatenation");
					Assert::AreEqual(kExpected4, actual4, L"Unexpected concatenation");
					Assert::AreEqual(kExpected1And2And3And4, actual1And2And3And4, L"Unexpected concatenation");

				}
			};
		}
	}
}
