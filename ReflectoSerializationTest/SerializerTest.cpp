#include "Type/TypeDescriptor.h"
#include "Serialization/Reader/JsonSerializationReader.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerializerFactory.h"
#include "Serialization/Strategy/SerializationStrategy.h"
#include "Serialization/Writer/JsonSerializationWriter.h"
#include "Type/TypeDescriptorFactory.h"
#include "Type/TypeFactory.h"
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
			struct TestPerson
			{
				std::string Name;
				int32_t Age = 0;

				bool operator==(const TestPerson& other) const
				{
					return Name == other.Name && Age == other.Age;
				}
			};
		}
	}
}

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<std::string>>(const std::vector<std::string>& vector)
{
	return Reflecto::Utils::StringExt::ToWstring(Reflecto::Utils::StringExt::Join<std::string>(vector, ","));
}

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<Reflecto::Serialization::Test::TestPerson>(const Reflecto::Serialization::Test::TestPerson& obj)
{
	return Reflecto::Utils::StringExt::Format<std::wstring>(L"{Name=%s,Age=%i}", obj.Name.c_str(), obj.Age);
}

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
					/////////////
					// Arrange
					Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<int32_t>("int32")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<int32_t, Int32SerializationStrategy>()
					.Build();

					int32_t expectedValue = 42;
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%i})"), "int32", expectedValue);

					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					int32_t actualDeserializedValue;
					std::string actualSerializedStr;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					actualSerializedStr = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerializedStr, L"Serialized value is unexpected!");
					Assert::AreEqual(expectedValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeString)
				{
					/////////////
					// Arrange
					Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<std::string>("string")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<std::string, StringSerializationStrategy>()
					.Build();

					std::string expectedValue = "test";
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", expectedValue.c_str());

					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					std::string actualSerialized;
					std::string actualDeserializedValue;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					actualSerialized = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");
					Assert::AreEqual(expectedValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeFloat)
				{
					/////////////
					// Arrange
					Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<float>("float")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<float, FloatSerializationStrategy>()
					.Build();

					const float expectedValue = 0.5f;
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%.1f})"), "float", expectedValue);

					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					std::string actualSerializedStr;
					float actualDeserializedValue;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					actualSerializedStr = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerializedStr, L"Serialized value is unexpected!");
					Assert::AreEqual(expectedValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeDouble)
				{
					/////////////
					// Arrange
					Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<double>("double")
						.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<double, DoubleSerializationStrategy>()
					.Build();

					const double expectedValue = 0.5;
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%.1f})"), "double", expectedValue);

					/////////////
					// Act
					bool success = true;

					std::stringstream stream;
					std::string actualSerializedStr;
					double actualDeserializedValue;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					actualSerializedStr = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerializedStr, L"Serialized value is unexpected!");
					Assert::AreEqual(expectedValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeBoolean)
				{
					/////////////
					// Arrange
					Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<bool>("boolean")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<bool, BooleanSerializationStrategy>()
					.Build();
					
					bool expectedValue = true;
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%s})"), "boolean", expectedValue ? "true" : "false");

					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					bool actualDeserializedValue;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					std::string actualSerialized = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");
					Assert::AreEqual(expectedValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeVector)
				{
					/////////////
					// Arrange
					Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<std::string>("string")
						.Add<std::vector<std::string>>("vector<string>")
					.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<std::vector<std::string>, VectorSerializationStrategy<std::vector<std::string>>>()
					.Build();

					const std::vector<std::string> expectedValue = { "uno", "dos", "tres" };
					const std::string value1ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "uno");
					const std::string value2ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "dos");
					const std::string value3ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "tres");
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":[%s,%s,%s]})"), "vector<string>", value1ExpectedStr.c_str(), value2ExpectedStr.c_str(), value3ExpectedStr.c_str());

					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					std::string actualSerialized;
					std::vector<std::string> actualDeserializedValue;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					actualSerialized = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");
					Assert::AreEqual(expectedValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeObject)
				{
					/////////////
					// Arrange
					const Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<TestPerson>("TestPerson")
						.Add<std::string>("string")
						.Add<int32_t>("int32")
					.Build();

					const Reflection::TypeDescriptor testPersonDescriptor = Reflection::TypeDescriptorFactory<TestPerson>(testTypeLibrary)
						.RegisterMember(&TestPerson::Name, "Name")
						.RegisterMember(&TestPerson::Age, "Age")
					.Build();

					const Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<int32_t, Int32SerializationStrategy>()
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<TestPerson, ObjectSerializationStrategy<TestPerson>>(testPersonDescriptor)
					.Build();

					const std::string expectedNameValue = "George";
					const int32_t expectedAgeValue = 1;
					const std::string expectedNameStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", expectedNameValue.c_str());
					const std::string expectedAgeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int32", expectedAgeValue);
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Age":%s,"Name":%s}})"), testPersonDescriptor.GetType().GetName().c_str(), expectedAgeStr.c_str(), expectedNameStr.c_str());

					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					std::string actualSerialized;
					TestPerson actualDeserializedValue;

					TestPerson expectedValue;
					expectedValue.Name = expectedNameValue;
					expectedValue.Age = expectedAgeValue;
					
					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					actualSerialized = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");
					Assert::AreEqual(expectedValue, actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeMap)
				{
					/////////////
					// Arrange
					Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<std::string>("string")
						.Add<int32_t>("int")
						.Add<std::map<int32_t, std::string>>("map<int,string>")
						.Build();

					Serializer serializer = SerializerFactory(testTypeLibrary)
						.LearnType<std::string, StringSerializationStrategy>()
						.LearnType<int32_t, Int32SerializationStrategy>()
						.LearnType<std::map<int32_t, std::string>, MapSerializationStrategy<std::map<int32_t, std::string>>>()
					.Build();

					const std::map<int32_t, std::string> expectedValue = { {1, "uno"}, { 2, "dos"}, { 3, "tres" } };
					
					const std::string key1ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int", 1);
					const std::string key2ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int", 2);
					const std::string key3ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int", 3);
					const std::string value1ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "uno");
					const std::string value2ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "dos");
					const std::string value3ExpectedStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", "tres");
					const std::string entry1ExpectedStr = Utils::StringExt::Format(std::string(R"({"key":%s,"value":%s})"), key1ExpectedStr.c_str(), value1ExpectedStr.c_str());
					const std::string entry2ExpectedStr = Utils::StringExt::Format(std::string(R"({"key":%s,"value":%s})"), key2ExpectedStr.c_str(), value2ExpectedStr.c_str());
					const std::string entry3ExpectedStr = Utils::StringExt::Format(std::string(R"({"key":%s,"value":%s})"), key3ExpectedStr.c_str(), value3ExpectedStr.c_str());
					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":[%s,%s,%s]})"), "map<int,string>", entry1ExpectedStr.c_str(), entry2ExpectedStr.c_str(), entry3ExpectedStr.c_str());

					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					std::string actualSerialized;
					std::map<int32_t, std::string> actualDeserializedValue;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(expectedValue, writer);
					success &= writer.Export(stream);

					actualSerialized = stream.str();

					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");
					Assert::IsTrue(expectedValue == actualDeserializedValue, L"Deserialized value is unexpected");
				}

				TEST_METHOD(SerializeObjectObject)
				{
					/////////////
					// Arrange
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

					const Reflection::TypeLibrary testTypeLibrary = Reflection::TypeLibraryFactory()
						.Add<TestPotatoHead>("TestPotatoHead")
						.Add<TestPotatoHead::Eyes>("TestPotatoHead::Eyes")
						.Add<TestPotatoHead::Nose>("TestPotatoHead::Nose")
						.Add<TestPotatoHead::Mouth>("TestPotatoHead::Mouth")
						.Add<std::string>("string")
						.Add<int32_t>("int32")
						.Add<bool>("boolean")
					.Build();

					const Reflection::TypeDescriptor testPotatoHeadDescriptor = Reflection::TypeDescriptorFactory<TestPotatoHead>(testTypeLibrary)
						.RegisterMember(&TestPotatoHead::Name, "Name")
						.RegisterMember(&TestPotatoHead::CurrentEyes, "Eyes")
						.RegisterMember(&TestPotatoHead::CurrentNose, "Nose")
						.RegisterMember(&TestPotatoHead::CurrentMouth, "Mouth")
					.Build();

					const Reflection::TypeDescriptor testPotatoHeadEyesDescriptor = Reflection::TypeDescriptorFactory<TestPotatoHead::Eyes>(testTypeLibrary)
						.RegisterMember(&TestPotatoHead::Eyes::Color, "Color")
						.RegisterMember(&TestPotatoHead::Eyes::Size, "Size")
					.Build();

					const Reflection::TypeDescriptor testPotatoHeadNoseDescriptor = Reflection::TypeDescriptorFactory<TestPotatoHead::Nose>(testTypeLibrary)
						.RegisterMember(&TestPotatoHead::Nose::Type, "Type")
					.Build();
					
					const Reflection::TypeDescriptor testPotatoHeadMouthDescriptor = Reflection::TypeDescriptorFactory<TestPotatoHead::Mouth>(testTypeLibrary)
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

					const std::string testValueName = "Mr. Potato Head";
					const std::string testEyesColor = "Blue";
					const std::string testNoseType = "Normal";
					const int32_t testEyesSize = 3;
					const bool testMouthIsSmiling = true;

					const std::string expectedNameStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testValueName.c_str());

					const std::string expectedEyesColorStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testEyesColor.c_str());
					const std::string expectedEyesSizeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%d})"), "int32", testEyesSize);
					const std::string expectedEyesStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Color":%s,"Size":%s}})"), testPotatoHeadEyesDescriptor.GetType().GetName().c_str(), expectedEyesColorStr.c_str(), expectedEyesSizeStr.c_str());

					const std::string expectedNoseTypeStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":"%s"})"), "string", testNoseType.c_str());
					const std::string expectedNoseStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Type":%s}})"), testPotatoHeadNoseDescriptor.GetType().GetName().c_str(), expectedNoseTypeStr.c_str());

					const std::string expectedMouthIsSmilingStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":%s})"), "boolean", testMouthIsSmiling ? "true" : "false");
					const std::string expectedMouthStr = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"IsSmiling":%s}})"), testPotatoHeadMouthDescriptor.GetType().GetName().c_str(), expectedMouthIsSmilingStr.c_str());

					const std::string expectedSerialized = Utils::StringExt::Format(std::string(R"({"type":"%s","value":{"Eyes":%s,"Mouth":%s,"Name":%s,"Nose":%s}})"), testPotatoHeadDescriptor.GetType().GetName().c_str(), expectedEyesStr.c_str(), expectedMouthStr.c_str(), expectedNameStr.c_str(), expectedNoseStr.c_str());
					
					/////////////
					// Act
					bool success = true;
					std::stringstream stream;
					std::string actualSerialized;
					TestPotatoHead actualDeserializedValue;

					TestPotatoHead testPotatoHead;
					
					testPotatoHead.Name = testValueName;
					
					TestPotatoHead::Eyes testEyes;
					testEyes.Color = testEyesColor;
					testEyes.Size = testEyesSize;
					testPotatoHead.CurrentEyes = testEyes;

					TestPotatoHead::Nose testNose;
					testNose.Type = testNoseType;
					testPotatoHead.CurrentNose = testNose;

					TestPotatoHead::Mouth testMouth;
					testMouth.IsSmiling = testMouthIsSmiling;
					testPotatoHead.CurrentMouth = testMouth;

					JsonSerializationWriter writer;
					success &= serializer.Serialize(testPotatoHead, writer);
					success &= writer.Export(stream);

					actualSerialized = stream.str();
					
					JsonSerializationReader reader;
					success &= reader.Import(stream);
					success &= serializer.Deserialize(actualDeserializedValue, reader);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Failure is unexpected!");
					Assert::IsTrue(testPotatoHead == actualDeserializedValue, L"Deserialized value is unexpected");
					Assert::AreEqual(expectedSerialized, actualSerialized, L"Serialized value is unexpected!");
				}
			};
		}
	}
}