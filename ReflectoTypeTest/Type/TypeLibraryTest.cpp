#include "Type/TypeDescriptorType.h"
#include "Type/TypeDescriptorTypeFactory.h"
#include "Type/TypeLibrary.h"

#include "CppUnitTest.h"
#include <tuple>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Type
	{
		namespace Test
		{
			TEST_CLASS(TypeLibraryTest)
			{
			public:
				TEST_METHOD(AddGet)
				{
					TypeLibrary testLibrary;
					TypeDescriptorType testUIntType = TypeDescriptorTypeFactory<uint32_t>().Build();
					testLibrary.Add(testUIntType);
					TypeDescriptorType testStringType = TypeDescriptorTypeFactory<std::string>().Build();
					testLibrary.Add(testStringType);

					Assert::IsTrue(*testLibrary.Get(testUIntType.Hash()) == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.Get<uint32_t>() == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.Get<std::string>() == testStringType, L"Unexpected type");
					Assert::IsTrue(testLibrary.Get<float>() == nullptr, L"Unexpected type");
				}
			};
		}
	}
}