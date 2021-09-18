#include "Utils/IdentityTransform.h"

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Utils
	{
		namespace Test
		{
			TEST_CLASS(IdentityTransformTest)
			{
			public:
				TEST_METHOD(OperationTest)
				{
					const IdentityTransform transform;

					std::string value = "Hello!";
					std::string actual = transform(value);
					std::string expected = value;
					
					Assert::AreEqual(actual, expected, L"Identity is unexpectedly not yielding same value");
				}
			};
		}
	}
}
