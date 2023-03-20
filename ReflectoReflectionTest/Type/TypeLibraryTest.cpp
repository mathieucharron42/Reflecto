#include "TestCommon.h"

#include "Type/TypeDescriptor.h"
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
					
					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add<uint32_t>("uint32")
						.Add<std::string>("string")
						.Add<float>("float")					
					.Build();

					/////////////
					// Act
					const TypeDescriptorPtr actualUIntType = testLibrary.GetDescriptor<uint32_t>();
					const TypeDescriptorPtr actualFloatType = testLibrary.GetDescriptor<float>();
					const TypeDescriptorPtr actualStringType = testLibrary.GetDescriptor<std::string>();
					const TypeDescriptorPtr actualMissingType = testLibrary.GetDescriptor<bool>();

					/////////////
					// Assert
					Assert::IsTrue(actualUIntType != nullptr, L"Type is unexpectedly missing");
					Assert::IsTrue(actualStringType != nullptr, L"Type is unexpectedly missing");
					Assert::IsTrue(actualFloatType != nullptr, L"Type is unexpectedly missing");
					Assert::IsTrue(actualMissingType == nullptr, L"Missing type is unexpectedly available");
				}

				TEST_METHOD(GetByHash)
				{
					/////////////
					// Arrange
					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add<uint32_t>("uint32")
						.Add<std::string>("string")
						.Build();

					/////////////
					// Act
					const TypeDescriptorPtr actualUIntType = testLibrary.GetDescriptorByHash(TypeExt::GetTypeHash<uint32_t>());
					const TypeDescriptorPtr actualStringType = testLibrary.GetDescriptorByHash(TypeExt::GetTypeHash<std::string>());
					const TypeDescriptorPtr actualMissingType = testLibrary.GetDescriptorByHash(TypeExt::GetTypeHash<bool>());

					/////////////
					// Assert
					Assert::IsTrue(actualUIntType != nullptr, L"Type is unexpectedly missing");
					Assert::IsTrue(actualStringType != nullptr, L"Type is unexpectedly missing");
					Assert::IsTrue(actualMissingType == nullptr, L"Missing type is unexpectedly available");
				}

				TEST_METHOD(GetByName)
				{
					/////////////
					// Arrange
					TypeLibrary testLibrary = TypeLibraryFactory()
						.Add<uint32_t>("uint32")
						.Add<std::string>("string")
					.Build();

					/////////////
					// Act
					const TypeDescriptorPtr actualUIntType = testLibrary.GetDescriptorByName("uint32");
					const TypeDescriptorPtr actualStringType = testLibrary.GetDescriptorByName("string");
					const TypeDescriptorPtr actualMissingType = testLibrary.GetDescriptorByName("boolean");

					/////////////
					// Assert
					Assert::IsTrue(actualUIntType != nullptr, L"Type is unexpectedly missing");
					Assert::IsTrue(actualStringType != nullptr, L"Type is unexpectedly missing");
					Assert::IsTrue(actualMissingType == nullptr, L"Missing type is unexpectedly available");
				}
			};
		}
	}
}