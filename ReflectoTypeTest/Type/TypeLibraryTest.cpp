#include "Type/Type.h"
#include "Type/TypeFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"

#include <CppUnitTest.h>
#include <tuple>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Reflection
	{
		namespace Test
		{
			TEST_CLASS(TypeLibraryTest)
			{
			public:
				TEST_METHOD(Get)
				{
					Type testUIntType = TypeFactory<uint32_t>("uint32").Build();
					Type testStringType = TypeFactory<std::string>("string").Build();
					Type testFloatType = TypeFactory<float>("float").Build();

					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add(testUIntType)
						.Add(testStringType)
						.Add(testFloatType)
						.Build();

					Assert::IsTrue(*testLibrary.Get<uint32_t>() == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.Get<float>() == testFloatType, L"Unexpected type");
				}
				TEST_METHOD(GetByHash)
				{
					Type testUIntType = TypeFactory<uint32_t>("uint32").Build();
					Type testStringType = TypeFactory<std::string>("string").Build();
					Type testFloatType = TypeFactory<float>("float").Build();

					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add(testUIntType)
						.Add(testStringType)
						.Add(testFloatType)
					.Build();
				
					Assert::IsTrue(*testLibrary.GetByHash(testUIntType.GetHash()) == testUIntType, L"Unexpected type");
					Assert::IsTrue(*testLibrary.GetByHash(testFloatType.GetHash()) == testFloatType, L"Unexpected type");
				}

				TEST_METHOD(GetByName)
				{
					Type testUIntType = TypeFactory<uint32_t>("uint32").Build();
					Type testStringType = TypeFactory<std::string>("string").Build();
					Type testFloatType = TypeFactory<float>("float").Build();

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