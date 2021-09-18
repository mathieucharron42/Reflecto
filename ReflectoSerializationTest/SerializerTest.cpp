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

#include <CppUnitTest.h>

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

				TEST_METHOD(SerializeFloat)
				{
					Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<float>("float")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<float, FloatSerializationStrategy>()
					.Build();

					float testValue = 0.5f;

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualSerializedStr;
					writer.Transpose(actualSerializedStr);

					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%.1f})"), "float", testValue);
					Assert::AreEqual(expectedSerialized, actualSerializedStr, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerializedStr);

					float actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::AreEqual(testValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeDouble)
				{
					Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<double>("double")
						.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<double, DoubleSerializationStrategy>()
					.Build();

					double testValue = 0.5;

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualSerializedStr;
					writer.Transpose(actualSerializedStr);

					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%.1f})"), "double", testValue);
					Assert::AreEqual(expectedSerialized, actualSerializedStr, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerializedStr);

					double actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::AreEqual(testValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeBoolean)
				{
					Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<bool>("boolean")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<bool, BooleanSerializationStrategy>()
					.Build();

					bool testValue = true;

					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);

					std::string actualSerializedStr;
					writer.Transpose(actualSerializedStr);

					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%s})"), "boolean", testValue ? "true" : "false");
					Assert::AreEqual(expectedSerialized, actualSerializedStr, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerializedStr);

					bool actualDeserializedValue;
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
						.RegisterMember(&TestPerson::Name, "Name")
						.RegisterMember(&TestPerson::Age, "Age")
					.Build();

					const Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<int32_t, Int32SerializationStrategy>()
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<TestPerson, ObjectSerializationStrategy<TestPerson>>(testPersonDescriptor)
					.Build();
	
					TestPerson testValue;
					const std::string testValueName = "George";
					testValue.Name = testValueName;
					const int32_t testValueAge = 1;
					testValue.Age = testValueAge;
					
					JsonSerializationWriter writer;
					serializer.Serialize(testValue, writer);
					
					std::string actualSerialized;
					writer.Transpose(actualSerialized);

					std::string expectedNameStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testValueName.c_str());
					std::string expectedAgeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int32", testValueAge);
					std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Age":%s,"Name":%s}})"), testPersonDescriptor.GetType().GetName().c_str(), expectedAgeStr.c_str(), expectedNameStr.c_str());
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

				TEST_METHOD(SerializeObjectObject)
				{
					struct TestPotatoHead
					{
						struct Eyes
						{
							std::string Color;
							int32_t Size = 0;

							bool operator==(const Eyes& other)
							{
								return Color == other.Color && Size == other.Size;
							}
						};

						struct Nose
						{
							std::string Type;

							bool operator==(const Nose& other)
							{
								return Type == other.Type;
							}
						};

						struct Mouth
						{
							bool IsSmiling = false;

							bool operator==(const Mouth& other)
							{
								return IsSmiling == other.IsSmiling;
							}
						};

						std::string Name;
						Eyes CurrentEyes;
						Nose CurrentNose;
						Mouth CurrentMouth;
						
						bool operator==(const TestPotatoHead& other)
						{
							return Name == other.Name && CurrentEyes == other.CurrentEyes && CurrentNose == other.CurrentNose && CurrentMouth == other.CurrentMouth;
						}
					};

					const Type::TypeLibrary testTypeLibrary = Type::TypeLibraryFactory()
						.Add<TestPotatoHead>("TestPotatoHead")
						.Add<TestPotatoHead::Eyes>("TestPotatoHead::Eyes")
						.Add<TestPotatoHead::Nose>("TestPotatoHead::Nose")
						.Add<TestPotatoHead::Mouth>("TestPotatoHead::Mouth")
						.Add<std::string>("string")
						.Add<int32_t>("int32")
						.Add<bool>("boolean")
					.Build();

					const Type::TypeDescriptor testPotatoHeadDescriptor = Type::TypeDescriptorFactory<TestPotatoHead>(testTypeLibrary)
						.RegisterMember(&TestPotatoHead::Name, "Name")
						.RegisterMember(&TestPotatoHead::CurrentEyes, "Eyes")
						.RegisterMember(&TestPotatoHead::CurrentNose, "Nose")
						.RegisterMember(&TestPotatoHead::CurrentMouth, "Mouth")
					.Build();

					const Type::TypeDescriptor testPotatoHeadEyesDescriptor = Type::TypeDescriptorFactory<TestPotatoHead::Eyes>(testTypeLibrary)
						.RegisterMember(&TestPotatoHead::Eyes::Color, "Color")
						.RegisterMember(&TestPotatoHead::Eyes::Size, "Size")
					.Build();

					const Type::TypeDescriptor testPotatoHeadNoseDescriptor = Type::TypeDescriptorFactory<TestPotatoHead::Nose>(testTypeLibrary)
						.RegisterMember(&TestPotatoHead::Nose::Type, "Type")
					.Build();
					
					const Type::TypeDescriptor testPotatoHeadMouthDescriptor = Type::TypeDescriptorFactory<TestPotatoHead::Mouth>(testTypeLibrary)
						.RegisterMember(&TestPotatoHead::Mouth::IsSmiling, "IsSmiling")
					.Build();

					const Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<int32_t, Int32SerializationStrategy>()
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<bool, BooleanSerializationStrategy>()
						.LearnType<TestPotatoHead, ObjectSerializationStrategy<TestPotatoHead>>(testPotatoHeadDescriptor)
						.LearnType<TestPotatoHead::Eyes, ObjectSerializationStrategy<TestPotatoHead::Eyes>>(testPotatoHeadEyesDescriptor)
						.LearnType<TestPotatoHead::Mouth, ObjectSerializationStrategy<TestPotatoHead::Mouth>>(testPotatoHeadMouthDescriptor)
						.LearnType<TestPotatoHead::Nose, ObjectSerializationStrategy<TestPotatoHead::Nose>>(testPotatoHeadNoseDescriptor)
					.Build();

					TestPotatoHead testPotatoHead;
					const std::string testValueName = "Mr. Potato Head";
					testPotatoHead.Name = testValueName;
					
					TestPotatoHead::Eyes testEyes;
					const std::string testEyesColor = "Blue";
					testEyes.Color = testEyesColor;
					const int32_t testEyesSize = 3;
					testEyes.Size = testEyesSize;
					testPotatoHead.CurrentEyes = testEyes;

					TestPotatoHead::Nose testNose;
					const std::string testNoseType = "Normal";
					testNose.Type = testNoseType;
					testPotatoHead.CurrentNose = testNose;

					TestPotatoHead::Mouth testMouth;
					const bool testMouthIsSmiling = true;
					testMouth.IsSmiling = testMouthIsSmiling;
					testPotatoHead.CurrentMouth = testMouth;

					JsonSerializationWriter writer;
					serializer.Serialize(testPotatoHead, writer);

					std::string actualSerialized;
					writer.Transpose(actualSerialized);

					const std::string expectedNameStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testValueName.c_str());
					
					const std::string expectedEyesColorStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testEyesColor.c_str());
					const std::string expectedEyesSizeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int32", testEyesSize);
					const std::string expectedEyesStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Color":%s,"Size":%s}})"), testPotatoHeadEyesDescriptor.GetType().GetName().c_str(), expectedEyesColorStr.c_str(), expectedEyesSizeStr.c_str());
					
					const std::string expectedNoseTypeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testNoseType.c_str()); 
					const std::string expectedNoseStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Type":%s}})"), testPotatoHeadNoseDescriptor.GetType().GetName().c_str(), expectedNoseTypeStr.c_str());

					const std::string expectedMouthIsSmilingStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%s})"), "boolean", testMouthIsSmiling ? "true" : "false");
					const std::string expectedMouthStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"IsSmiling":%s}})"), testPotatoHeadMouthDescriptor.GetType().GetName().c_str(), expectedMouthIsSmilingStr.c_str());

					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Eyes":%s,"Mouth":%s,"Name":%s,"Nose":%s}})"), testPotatoHeadDescriptor.GetType().GetName().c_str(), expectedEyesStr.c_str(), expectedMouthStr.c_str(), expectedNameStr.c_str(), expectedNoseStr.c_str());
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");

					JsonSerializationReader reader;
					reader.Import(actualSerialized);

					TestPotatoHead actualDeserializedValue;
					serializer.Deserialize(actualDeserializedValue, reader);

					Assert::IsTrue(testPotatoHead == actualDeserializedValue, L"Deserialized value is unexpected");
				}
			};
		}
	}
}