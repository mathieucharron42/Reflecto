#include "Serialization/Writer/JsonSerializationWriter.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Serialization
	{
		namespace Test
		{
			TEST_CLASS(JsonSerializationWriterTest)
			{
			public:
				TEST_METHOD(SerializeString)
				{
					JsonSerializationWriter writer;
					writer.WriteString("test");

					std::string actual;
					writer.Transpose(actual);
					
					const std::string expected = R"("test")";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(SerializeInt32)
				{
					JsonSerializationWriter writer;
					writer.WriteInteger32(1);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(1)";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(SerializeInt64)
				{
					JsonSerializationWriter writer;
					writer.WriteInteger64(33445566778899);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(33445566778899)";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(SerializeFloat)
				{
					JsonSerializationWriter writer;
					writer.WriteFloat(0.5f);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(0.5)";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(SerializeDouble)
				{
					JsonSerializationWriter writer;
					writer.WriteDouble(0.5);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(0.5)";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(SerializeBool)
				{
					{
						JsonSerializationWriter writer;
						writer.WriteBoolean(true);

						std::string actual;
						writer.Transpose(actual);

						const std::string expected = R"(true)";
						Assert::AreEqual(expected, actual, L"Unexpected serialization");
					}

					{
						JsonSerializationWriter writer;
						writer.WriteBoolean(false);

						std::string actual;
						writer.Transpose(actual);

						const std::string expected = R"(false)";
						Assert::AreEqual(expected, actual, L"Unexpected serialization");
					}
				}

				TEST_METHOD(SerializeNull)
				{
					JsonSerializationWriter writer;
					
					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(null)";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(SerializeArray)
				{
					JsonSerializationWriter writer;

					writer.WriteBeginArrayElement();
					writer.WriteInteger32(1);
					writer.WriteEndArrayElement();

					writer.WriteBeginArrayElement();
					writer.WriteInteger32(2);
					writer.WriteEndArrayElement();

					writer.WriteBeginArrayElement();
					writer.WriteInteger32(3);
					writer.WriteEndArrayElement();

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"([1,2,3])";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}
				
				TEST_METHOD(SerializeObject)
				{
					JsonSerializationWriter writer;

					writer.WriteBeginObjectProperty("Name");
					writer.WriteString("Mr. Potato Head");
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("Age");
					writer.WriteInteger32(1);
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("Friendliness");
					writer.WriteFloat(0.5f);
					writer.WriteEndObjectProperty();

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"({"Age":1,"Friendliness":0.5,"Name":"Mr. Potato Head"})";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(SerializeComplexObject)
				{
					JsonSerializationWriter writer;

					writer.WriteBeginObjectProperty("Name");
					writer.WriteString("Mr. Potato Head");
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("Eyes");
					{
						writer.WriteBeginObjectProperty("Size");
						writer.WriteInteger32(5);
						writer.WriteEndObjectProperty();
						writer.WriteBeginObjectProperty("Color");
						writer.WriteString("black");
						writer.WriteEndObjectProperty();
					}
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("Mouth");
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("Legs");
					{
						writer.WriteBeginObjectProperty("PossibleColors");
						writer.WriteBeginArrayElement();
						writer.WriteString("blue");
						writer.WriteEndArrayElement();
						writer.WriteBeginArrayElement();
						writer.WriteString("orange");
						writer.WriteEndArrayElement();
						writer.WriteBeginArrayElement();
						writer.WriteString("white");
						writer.WriteEndArrayElement();
						writer.WriteEndObjectProperty();

					}
					writer.WriteEndObjectProperty();
					
					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"({"Eyes":{"Color":"black","Size":5},"Legs":{"PossibleColors":["blue","orange","white"]},"Mouth":null,"Name":"Mr. Potato Head"})";
					Assert::AreEqual(expected, actual, L"Unexpected serialization");
				}

				TEST_METHOD(Tranpose)
				{
					JsonSerializationWriter writer;
					writer.WriteString("test");

					std::string actualStr;
					writer.Transpose(actualStr);

					const std::string expectedStr = R"("test")";
					Assert::AreEqual(expectedStr, actualStr, L"Unexpected serialization");

					std::vector<byte> actualBytes;
					writer.Transpose(actualBytes);

					std::vector<byte> expectedBytes = { 34, 116, 101, 115, 116, 34, 0 };
					Assert::IsTrue(expectedBytes == actualBytes, L"Unexpected serialization");
				}
			};
		}
	}
}