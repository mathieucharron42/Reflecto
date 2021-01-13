#include "Common/Utils/NonCopyable.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Utils
	{
		namespace Test
		{
			TEST_CLASS(NonCopyable)
			{
			public:
				TEST_METHOD(IsNonCopyable)
				{
					class TestClass : public NonCopyable
					{ };

					Assert::IsFalse(std::is_trivially_copyable_v<TestClass>, L"NonCopyable is unexpectedly copyable!");
				}
			};
		}
	}
}
