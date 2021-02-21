#include "Type/TypeDescriptor.h"
#include "Serialization/Reader/JsonSerializationReader.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerializerFactory.h"
#include "Serialization/Strategy/SerializationStrategy.h"
#include "Serialization/TextSerialization.h"
#include "Serialization/Writer/JsonSerializationWriter.h"
#include "Type/TypeDescriptorFactory.h"
#include "Type/TypeDescriptorTypeFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"
#include "Utils/StringExt.h"

#include "CppUnitTest.h"

#include <functional>
#include <map>
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
					Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<int32_t>("int32")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<int32_t, Int32SerializationStrategy>()
					.Build();

					int32_t testValue = 42;

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualSerializedStr;
					writer.Transpose(actualSerializedStr);

					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%i})"), "int32", testValue);
					Assert::AreEqual(expectedSerialized, actualSerializedStr, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerializedStr);

					int32_t actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::AreEqual(testValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeString)
				{
					Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<std::string>("string")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<std::string, StringSerializationStrategy>()
					.Build();

					std::string testValue = "test";

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualSerialized;
					writer.Transpose(actualSerialized);

					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testValue.c_str());
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerialized);

					std::string actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::AreEqual(testValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeObject)
				{
					struct TestPerson
					{
						std::string Name;
						int32_t Age = 0;

						bool operator==(const TestPerson& other)
						{
							return Name == other.Name && Age == other.Age;
						}
					};

					const Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<TestPerson>("TestPerson")
						.Add<std::string>("string")
						.Add<int32_t>("int32")
					.Build();

					const Type::TypeDescriptor testPersonDescriptor = Type::TypeDescriptorFactory<TestPerson>(testTypeLibrary)
						.Register(&TestPerson::Name, "Name")
						.Register(&TestPerson::Age, "Age")
					.Build();

					const Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<int32_t, Int32SerializationStrategy>()
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<TestPerson, ObjectSerializationStrategy<TestPerson>>(testPersonDescriptor)
					.Build();
	
					TestPerson testValue;
					const std::string testValueName = "Mr. Potato Head";
					testValue.Name = testValueName;
					const int32_t testValueAge = 1;
					testValue.Age = testValueAge;
					
					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);
					
					std::string actualSerialized;
					writer.Transpose(actualSerialized);

					std::string expectedNameStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testValueName.c_str());
					std::string expectedAgeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int32", testValueAge);
					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Age":%s,"Name":%s}})"), testPersonDescriptor.Type().Name().c_str(), expectedAgeStr.c_str(), expectedNameStr.c_str());
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerialized);

					TestPerson actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::IsTrue(testValue == actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeVector)
				{
					Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<std::string>("string")
						.Add<std::vector<std::string>>("vector<string>")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<std::vector<std::string>, VectorSerializationStrategy<std::vector<std::string>>>()
					.Build();

					std::vector<std::string> testValue = { "uno", "dos", "tres" };

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualSerialized;
					writer.Transpose(actualSerialized);

					std::string value1ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "uno");
					std::string value2ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "dos");
					std::string value3ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "tres");
					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":[%s,%s,%s]})"), "vector<string>", value1ExpectedStr.c_str(), value2ExpectedStr.c_str(), value3ExpectedStr.c_str());
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerialized);

					std::vector<std::string> actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::IsTrue(testValue == actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeMap)
				{
					Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<std::string>("string")
						.Add<int32_t>("int")
						.Add<std::map<int32_t, std::string>>("map<int,string>")
						.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<int32_t, Int32SerializationStrategy>()
						.LearnType<std::map<int32_t, std::string>, MapSerializationStrategy<std::map<int32_t, std::string>>>()
					.Build();

					std::map<int32_t, std::string> testValue = { {1, "uno"}, { 2, "dos"}, { 3, "tres" } };

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualSerialized;
					writer.Transpose(actualSerialized);

					std::string key1ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int", 1);
					std::string key2ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int", 2);
					std::string key3ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int", 3);
					std::string value1ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "uno");
					std::string value2ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "dos");
					std::string value3ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "tres");

					std::string entry1ExpectedStr = Utils::StringExt::Format(std::string(R"({"key":%s,"value":%s})"), key1ExpectedStr.c_str(), value1ExpectedStr.c_str());
					std::string entry2ExpectedStr = Utils::StringExt::Format(std::string(R"({"key":%s,"value":%s})"), key2ExpectedStr.c_str(), value2ExpectedStr.c_str());
					std::string entry3ExpectedStr = Utils::StringExt::Format(std::string(R"({"key":%s,"value":%s})"), key3ExpectedStr.c_str(), value3ExpectedStr.c_str());
					
					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":[%s,%s,%s]})"), "map<int,string>", entry1ExpectedStr.c_str(), entry2ExpectedStr.c_str(), entry3ExpectedStr.c_str());
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");
				
					JsonSerializationReader reader;
					reader.Import(actualSerialized);

					std::map<int32_t, std::string> actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::IsTrue(testValue == actualDeserializedValue, L"Deserialized value is unexpected");
				}
			};
		}
	}
}