#include "Common/Definitions.h"
#include "Utils/AreTrait.h"

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Utils
	{
		namespace Test
		{
			TEST_CLASS(AreTraitTest)
			{
			public:
				TEST_METHOD(SinglePodType)
				{
					Assert::IsTrue(AreTrait<std::is_pod, float>::value, L"AreTrait is unexpectedly false!");
				}

				TEST_METHOD(MultiplePodType)
				{
					Assert::IsTrue(AreTrait<std::is_pod, int32_t, float>::value, L"AreTrait is unexpectedly false!");
				}

				TEST_METHOD(SingleNonPodType)
				{
					Assert::IsFalse(AreTrait<std::is_pod, std::string>::value, L"AreTrait is unexpectedly true!");
				}

				TEST_METHOD(MultipleNonPodType)
				{
					class Obj { std::string str; };
					Assert::IsFalse(AreTrait<std::is_pod, std::string, Obj>::value, L"AreTrait is unexpectedly true!");
				}

				TEST_METHOD(MultipleMixedPodType)
				{
					Assert::IsFalse(AreTrait<std::is_pod, float, std::string>::value, L"AreTrait is unexpectedly true!");
				}

				TEST_METHOD(Value)
				{
					Assert::IsTrue(AreTrait_V<std::is_pod, float>, L"AreTrait_v is unexpectedly false!");
				}
			};
		}
	}
}
