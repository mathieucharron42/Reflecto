#include "Type/TypeDescriptorType.h"
#include "Type/TypeDescriptorTypeFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"

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
					TypeDescriptorType testUIntType = TypeDescriptorTypeFactory<uint32_t>("uint32").Build();
					TypeDescriptorType testStringType = TypeDescriptorTypeFactory<std::string>("string").Build();
					TypeDescriptorType testFloatType = TypeDescriptorTypeFactory<float>("float").Build();

					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add(testUIntType)
						.Add(testStringType)
						.Add(testFloatType)
					.Build();
					
					Assert::IsTrue(*testLibrary.Get<uint32_t>() == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.Get<float>() == testFloatType, L"Unexpected type");
				
					Assert::IsTrue(*testLibrary.GetByHash(testUIntType.Hash()) == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.GetByHash(testFloatType.Hash()) == testFloatType, L"Unexpected type");
				}

				TEST_METHOD(GetByName)
				{
					TypeDescriptorType testUIntType = TypeDescriptorTypeFactory<uint32_t>("uint32").Build();
					TypeDescriptorType testStringType = TypeDescriptorTypeFactory<std::string>("string").Build();
					TypeDescriptorType testFloatType = TypeDescriptorTypeFactory<float>("float").Build();

					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add(testUIntType)
						.Add(testStringType)
						.Add(testFloatType)
					.Build();

					Assert::IsTrue(*testLibrary.GetByName("uint32") == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.GetByName("string") == testStringType, L"Unexpected type");
				}

			};
		}
	}
}