#include "Common/Utils/StringExt.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"
#include "Serialization/SerializationFormat.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerializationMetaType.h"
#include "Serialization/Strategy/JsonSerializationStrategy.h"
#include "Serialization/TextSerialization.h"

#include "CppUnitTest.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Serialization
	{
		namespace Test
		{
			TEST_CLASS(SerializerTest)
			{
			public:
				TEST_METHOD(SerializeInt)
				{
					const Type::TypeDescriptor intDescriptor = Type::TypeDescriptorFactory<int32_t>().Build();

					Serializer<JsonSerializationWriter> serializer;
					serializer.RegisterType(intDescriptor, JsonSerializationStrategy::SerializeInt32);

					int32_t testValue = 42;

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualStr;
					writer.Transpose(actualStr);

					std::string expectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%i})"), intDescriptor.Type().Name().c_str(), testValue);
					Assert::AreEqual(expectedStr, actualStr, L"Serialized bytes are unexpected!");
				}

				TEST_METHOD(SerializeString)
				{
					const Type::TypeDescriptor strDescriptor = Type::TypeDescriptorFactory<std::string>().Build();

					Serializer<JsonSerializationWriter> serializer;
					serializer.RegisterType(strDescriptor, JsonSerializationStrategy::SerializeString);

					std::string testValue = "test";

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualStr;
					writer.Transpose(actualStr);

					std::string expectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), strDescriptor.Type().Name().c_str(), testValue.c_str());
					Assert::AreEqual(expectedStr, actualStr, L"Serialized bytes are unexpected!");
				}


				TEST_METHOD(SerializeObject)
				{
					struct PersonTestObject
					{
						std::string Name;
						int32_t Age = 0;
					};

					const Type::TypeDescriptor strDescriptor = Type::TypeDescriptorFactory<std::string>().Build();
					const Type::TypeDescriptor int32Descriptor = Type::TypeDescriptorFactory<int32_t>().Build();
					const Type::TypeDescriptor objDescriptor = Type::TypeDescriptorFactory<PersonTestObject>()
						.Register(&PersonTestObject::Name, "Name")
						.Register(&PersonTestObject::Age, "Age")
					.Build();

					Serializer<JsonSerializationWriter> serializer;
					serializer.RegisterType(int32Descriptor, JsonSerializationStrategy::SerializeInt32);
					serializer.RegisterType(strDescriptor, JsonSerializationStrategy::SerializeString);
					serializer.RegisterType(objDescriptor, JsonSerializationStrategy::SerializeObject<PersonTestObject>);

				
					PersonTestObject testValue;
					const std::string testValueName = "Mr. Potato Head";
					testValue.Name = testValueName;
					const int32_t testValueAge = 1;
					testValue.Age = testValueAge;
					
					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);
					
					std::string actualStr;
					writer.Transpose(actualStr);

					std::string expectedNameStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), strDescriptor.Type().Name().c_str(), testValueName.c_str());
					std::string expectedAgeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), int32Descriptor.Type().Name().c_str(), testValueAge);
					std::string expectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Age":%s,"Name":%s}})"), objDescriptor.Type().Name().c_str(), expectedAgeStr.c_str(), expectedNameStr.c_str());
					Assert::AreEqual(expectedStr, actualStr, L"Serialized bytes are unexpected!");
				}
			};
		}
	}
}