#include "Utils/NonCopyable.h"

#include <array>
#include <tuple>

#include "Utils/CollectionExt.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Utils
	{
		namespace Test
		{
			TEST_CLASS(CollectionExt)
			{
			public:
				TEST_METHOD(ArrayToTupleTest)
				{
					std::array<int32_t, 3> array = {42,5,29};
					std::tuple<int32_t, int32_t, int32_t> tuple = Reflecto::Utils::CollectionExt::ArrayToTuple(array);
					
					Assert::AreEqual(std::get<0>(array), std::get<0>(tuple), L"Element are unexpectedly different");
					Assert::AreEqual(std::get<1>(array), std::get<1>(tuple), L"Element are unexpectedly different");
					Assert::AreEqual(std::get<2>(array), std::get<2>(tuple), L"Element are unexpectedly different");
				}

				TEST_METHOD(ArrayToTupleConvertTest)
				{
					
					std::array<int32_t, 3> array = { 42,5,29 };
					std::tuple<std::string, std::string, std::string> tuple = Reflecto::Utils::CollectionExt::ArrayToTuple(array, [] (int32_t i) {
						return std::to_string(i+1);
					});

					Assert::AreEqual(std::string("43"), std::get<0>(tuple), L"Element are unexpectedly different");
					Assert::AreEqual(std::string("6"), std::get<1>(tuple), L"Element are unexpectedly different");
					Assert::AreEqual(std::string("30"), std::get<2>(tuple), L"Element are unexpectedly different");
				}
			};
		}
	}
}
