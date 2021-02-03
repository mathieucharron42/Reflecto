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
				TEST_METHOD(GetByHash)
				{
					TypeLibrary testLibrary;
					TypeDescriptorType testUIntType = TypeDescriptorTypeFactory<uint32_t>().Build();
					testLibrary.Add(testUIntType);
					TypeDescriptorType testStringType = TypeDescriptorTypeFactory<std::string>().Build();
					testLibrary.Add(testStringType);
					TypeDescriptorType testFloatType = TypeDescriptorTypeFactory<float>().Build();
					testLibrary.Add(testFloatType);

					Assert::IsTrue(*testLibrary.Get<uint32_t>() == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.Get<float>() == testFloatType, L"Unexpected type");
				
					Assert::IsTrue(*testLibrary.GetByHash(testUIntType.Hash()) == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.GetByHash(testFloatType.Hash()) == testFloatType, L"Unexpected type");
				}

				TEST_METHOD(GetByName)
				{
					TypeLibrary testLibrary;
					TypeDescriptorType testUIntType = TypeDescriptorTypeFactory<uint32_t>().Build();
					testLibrary.Add(testUIntType);
					TypeDescriptorType testStringType = TypeDescriptorTypeFactory<std::string>().Build();
					testLibrary.Add(testStringType);
					TypeDescriptorType testFloatType = TypeDescriptorTypeFactory<float>().Build();
					testLibrary.Add(testFloatType);

					Assert::IsTrue(*testLibrary.GetByName("uint32") == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.GetByName("string") == testFloatType, L"Unexpected type");
				}

			};
		}
	}
}