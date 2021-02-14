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
				TEST_METHOD(NoWrite)
				{
					JsonSerializationWriter writer;
					
					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"()";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteString)
				{
					JsonSerializationWriter writer;
					writer.WriteString("test");

					std::string actual;
					writer.Transpose(actual);
					
					const std::string expected = R"("test")";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteInt32)
				{
					JsonSerializationWriter writer;
					writer.WriteInteger32(1);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(1)";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteInt64)
				{
					JsonSerializationWriter writer;
					writer.WriteInteger64(33445566778899);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(33445566778899)";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteFloat)
				{
					JsonSerializationWriter writer;
					writer.WriteFloat(0.5f);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(0.5)";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteDouble)
				{
					JsonSerializationWriter writer;
					writer.WriteDouble(0.5);

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(0.5)";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteBool)
				{
					{
						JsonSerializationWriter writer;
						writer.WriteBoolean(true);

						std::string actual;
						writer.Transpose(actual);

						const std::string expected = R"(true)";
						Assert::AreEqual(expected, actual, L"Unexpected written value");
					}

					{
						JsonSerializationWriter writer;
						writer.WriteBoolean(false);

						std::string actual;
						writer.Transpose(actual);

						const std::string expected = R"(false)";
						Assert::AreEqual(expected, actual, L"Unexpected written value");
					}
				}

				TEST_METHOD(WriteNull)
				{
					JsonSerializationWriter writer;

					writer.WriteNull();
					
					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"(null)";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteArray)
				{
					JsonSerializationWriter writer;

					writer.WriteBeginArray();

					writer.WriteBeginArrayElement();
					writer.WriteInteger32(1);
					writer.WriteEndArrayElement();

					writer.WriteBeginArrayElement();
					writer.WriteInteger32(2);
					writer.WriteEndArrayElement();

					writer.WriteBeginArrayElement();
					writer.WriteInteger32(3);
					writer.WriteEndArrayElement();

					writer.WriteEndArray();

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"([1,2,3])";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}
				
				TEST_METHOD(WriteObject)
				{
					JsonSerializationWriter writer;

					writer.WriteBeginObject();

					writer.WriteBeginObjectProperty("Name");
					writer.WriteString("Mr. Potato Head");
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("Age");
					writer.WriteInteger32(1);
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("Friendliness");
					writer.WriteFloat(0.5f);
					writer.WriteEndObjectProperty();

					writer.WriteEndObject();

					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"({"Age":1,"Friendliness":0.5,"Name":"Mr. Potato Head"})";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}

				TEST_METHOD(WriteComplexObject)
				{
					JsonSerializationWriter writer;

					writer.WriteBeginObject();
					{
						writer.WriteBeginObjectProperty("Name");
						{
							writer.WriteString("Mr. Potato Head");
						}
						writer.WriteEndObjectProperty();

						writer.WriteBeginObjectProperty("Eyes");
						{
							writer.WriteBeginObject();
							{
								writer.WriteBeginObjectProperty("Size");
								{
									writer.WriteInteger32(5);
								}
								writer.WriteEndObjectProperty();
								writer.WriteBeginObjectProperty("Color");
								{
									writer.WriteString("black");
								}
								writer.WriteEndObjectProperty();
							}
							writer.WriteEndObject();
						}
						writer.WriteEndObjectProperty();

						writer.WriteBeginObjectProperty("Mouth");
						writer.WriteNull();
						writer.WriteEndObjectProperty();

						writer.WriteBeginObjectProperty("Legs");
						{
							writer.WriteBeginObject();
							{
								writer.WriteBeginObjectProperty("PossibleColors");
								{
									writer.WriteBeginArray();
									{
										writer.WriteBeginArrayElement();
										writer.WriteString("blue");
										writer.WriteEndArrayElement();
										writer.WriteBeginArrayElement();
										writer.WriteString("orange");
										writer.WriteEndArrayElement();
										writer.WriteBeginArrayElement();
										writer.WriteString("white");
										writer.WriteEndArrayElement(); 
									}
									writer.WriteEndArray();
								}
								writer.WriteEndObjectProperty();
							}
							writer.WriteEndObject();

						}
						writer.WriteEndObjectProperty();
					}
					writer.WriteEndObject();
					
					std::string actual;
					writer.Transpose(actual);

					const std::string expected = R"({"Eyes":{"Color":"black","Size":5},"Legs":{"PossibleColors":["blue","orange","white"]},"Mouth":null,"Name":"Mr. Potato Head"})";
					Assert::AreEqual(expected, actual, L"Unexpected written value");
				}
			};
		}
	}
}