#include "TestCommon.h"

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
					/////////////
					// Arrange
					Type kExpectedUIntType = TypeFactory<uint32_t>("uint32").Build();
					Type kExpectedStringType = TypeFactory<std::string>("string").Build();
					Type kExpectedFloatType = TypeFactory<float>("float").Build();

					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add(kExpectedUIntType)
						.Add(kExpectedStringType)
						.Add(kExpectedFloatType)
					.Build();

					/////////////
					// Act
					const Type* actualUIntType = testLibrary.Get<uint32_t>();
					const Type* actualFloatType = testLibrary.Get<float>();
					const Type* actualStringType = testLibrary.Get<std::string>();
					const Type* actualMissingType = testLibrary.Get<bool>();

					/////////////
					// Assert
					Assert::IsNotNull(actualUIntType, L"Type is unexpectedly missing");
					Assert::AreEqual(kExpectedUIntType, *actualUIntType, L"Type is unexpectedly different");
				
					Assert::IsNotNull(actualStringType, L"Type is unexpectedly missing");
					Assert::AreEqual(kExpectedStringType, *actualStringType, L"Type is unexpectedly different");

					Assert::IsNotNull(actualFloatType, L"Type is unexpectedly missing");
					Assert::AreEqual(kExpectedFloatType, *actualFloatType, L"Type is unexpectedly different");
				
					Assert::IsNull(actualMissingType, L"Missing type is unexpectedly available");
				}

				TEST_METHOD(GetByHash)
				{
					/////////////
					// Arrange
					Type kExpectedUIntType = TypeFactory<uint32_t>("uint32").Build();
					Type kExpectedStringType = TypeFactory<std::string>("string").Build();
					
					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add(kExpectedUIntType)
						.Add(kExpectedStringType)
					.Build();

					/////////////
					// Act
					const Type* actualUIntType = testLibrary.GetByHash(TypeExt::GetTypeHash<uint32_t>());
					const Type* actualStringType = testLibrary.GetByHash(TypeExt::GetTypeHash<std::string>());
					const Type* actualMissingType = testLibrary.GetByHash(TypeExt::GetTypeHash<bool>());

					/////////////
					// Assert
					Assert::IsNotNull(actualUIntType, L"Type is unexpectedly missing");
					Assert::AreEqual(kExpectedUIntType, *actualUIntType, L"Type is unexpectedly different");

					Assert::IsNotNull(actualStringType, L"Type is unexpectedly missing");
					Assert::AreEqual(kExpectedStringType, *actualStringType, L"Type is unexpectedly different");

					Assert::IsNull(actualMissingType, L"Missing type is unexpectedly available");
				}

				TEST_METHOD(GetByName)
				{
					/////////////
					// Arrange
					Type kExpectedUIntType = TypeFactory<uint32_t>("uint32").Build();
					Type kExpectedStringType = TypeFactory<std::string>("string").Build();

					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add(kExpectedUIntType)
						.Add(kExpectedStringType)
					.Build();

					/////////////
					// Act
					const Type* actualUIntType = testLibrary.GetByName("uint32");
					const Type* actualStringType = testLibrary.GetByName("string");
					const Type* actualMissingType = testLibrary.GetByName("boolean");

					/////////////
					// Assert
					Assert::IsNotNull(actualUIntType, L"Type is unexpectedly missing");
					Assert::AreEqual(kExpectedUIntType, *actualUIntType, L"Type is unexpectedly different");

					Assert::IsNotNull(actualStringType, L"Type is unexpectedly missing");
					Assert::AreEqual(kExpectedStringType, *actualStringType, L"Type is unexpectedly different");

					Assert::IsNull(actualMissingType, L"Missing type is unexpectedly available");
				}
			};
		}
	}
}