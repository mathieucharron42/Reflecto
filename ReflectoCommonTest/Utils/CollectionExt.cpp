#include "Utils/CollectionExt.h"
#include "Utils/NonCopyable.h"

#include <CppUnitTest.h>

#include <array>
#include <tuple>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
			};
		}
	}
}
