#include "Serialization/Writer/JsonSerializationWriter.h"

#include <CppUnitTest.h>

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
				TEST_METHOD(NoWrite)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"()";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteString)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"("test")";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteString("test");
					success &= writer.Transpose(actual);
					
					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteInt32)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"(1)";
					
					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteInteger32(1);
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteInt64)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"(33445566778899)";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteInteger64(33445566778899);
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteFloat)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"(0.5)";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteFloat(0.5f);
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteDouble)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"(0.5)";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteDouble(0.5);
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteBool)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"(false)";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteBoolean(false);
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteNull)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"(null)";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteNull();
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteArray)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"([1,2,3])";

					/////////////
					// Act
					bool success = true;
					std::string actual;

					success &= writer.WriteBeginArray();

					success &= writer.WriteBeginArrayElement();
					success &= writer.WriteInteger32(1);
					success &= writer.WriteEndArrayElement();

					success &= writer.WriteBeginArrayElement();
					success &= writer.WriteInteger32(2);
					success &= writer.WriteEndArrayElement();

					success &= writer.WriteBeginArrayElement();
					success &= writer.WriteInteger32(3);
					success &= writer.WriteEndArrayElement();

					success &= writer.WriteEndArray();
					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}
				
				TEST_METHOD(WriteObject)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"({"FieldFloat":0.5,"FieldInt32":1,"FieldString":"ABC123321CBA"})";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteBeginObject();

					success &= writer.WriteBeginObjectProperty("FieldString");
					success &= writer.WriteString("ABC123321CBA");
					success &= writer.WriteEndObjectProperty();

					success &= writer.WriteBeginObjectProperty("FieldInt32");
					success &= writer.WriteInteger32(1);
					success &= writer.WriteEndObjectProperty();

					success &= writer.WriteBeginObjectProperty("FieldFloat");
					success &= writer.WriteFloat(0.5f);
					success &= writer.WriteEndObjectProperty();

					success &= writer.WriteEndObject();

					success &= writer.Transpose(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteComplexObject)
				{
					/////////////
					// Arrange
					JsonSerializationWriter writer;
					const std::string expected = R"({"Eyes":{"Color":"black","Size":5},"Legs":{"PossibleColors":["blue","orange","white"]},"Mouth":null,"Name":"Mr. Potato Head"})";

					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= writer.WriteBeginObject();
					{
						success &= writer.WriteBeginObjectProperty("Name");
						{
							success &= writer.WriteString("Mr. Potato Head");
						}
						success &= writer.WriteEndObjectProperty();

						success &= writer.WriteBeginObjectProperty("Eyes");
						{
							success &= writer.WriteBeginObject();
							{
								success &= writer.WriteBeginObjectProperty("Size");
								{
									success &= writer.WriteInteger32(5);
								}
								success &= writer.WriteEndObjectProperty();
								success &= writer.WriteBeginObjectProperty("Color");
								{
									success &= writer.WriteString("black");
								}
								success &= writer.WriteEndObjectProperty();
							}
							success &= writer.WriteEndObject();
						}
						success &= writer.WriteEndObjectProperty();

						success &= writer.WriteBeginObjectProperty("Mouth");
						success &= writer.WriteNull();
						success &= writer.WriteEndObjectProperty();

						success &= writer.WriteBeginObjectProperty("Legs");
						{
							success &= writer.WriteBeginObject();
							{
								success &= writer.WriteBeginObjectProperty("PossibleColors");
								{
									success &= writer.WriteBeginArray();
									{
										success &= writer.WriteBeginArrayElement();
										success &= writer.WriteString("blue");
										success &= writer.WriteEndArrayElement();
										success &= writer.WriteBeginArrayElement();
										success &= writer.WriteString("orange");
										success &= writer.WriteEndArrayElement();
										success &= writer.WriteBeginArrayElement();
										success &= writer.WriteString("white");
										success &= writer.WriteEndArrayElement();
									}
									success &= writer.WriteEndArray();
								}
								success &= writer.WriteEndObjectProperty();
							}
							success &= writer.WriteEndObject();
						}
						success &= writer.WriteEndObjectProperty();
					}
					success &= writer.WriteEndObject();
					success &= writer.Transpose(actual);


					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}
			};
		}
	}
}