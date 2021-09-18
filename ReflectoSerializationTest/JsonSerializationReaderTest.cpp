#include "Serialization/Reader/JsonSerializationReader.h"

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Serialization
	{
		namespace Test
		{
			TEST_CLASS(JsonSerializationReaderTest)
			{
			public:
				TEST_METHOD(ReadString)
				{
					JsonSerializationReader reader;
					reader.Import(R"("test")");
					
					std::string actual;
					reader.ReadString(actual);

					const std::string expected = R"(test)";
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadInt32)
				{
					JsonSerializationReader reader;
					reader.Import(R"(1)");
					
					std::int32_t actual;
					reader.ReadInteger32(actual);

					const int32_t expected = 1;
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadInt64)
				{
					JsonSerializationReader reader;
					reader.Import(R"(33445566778899)");

					std::int64_t actual;
					reader.ReadInteger64(actual);

					const std::int64_t expected = 33445566778899;
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadFloat)
				{
					JsonSerializationReader reader;
					reader.Import(R"(0.5)");

					float actual;
					reader.ReadFloat(actual);

					const float expected = 0.5f;
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadDouble)
				{
					JsonSerializationReader reader;
					reader.Import(R"(0.5)");

					double actual;
					reader.ReadDouble(actual);

					const double expected = 0.5;
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadBool)
				{
					{
						JsonSerializationReader reader;
						reader.Import(R"(true)");

						bool actual;
						reader.ReadBoolean(actual);

						const bool expected = true;
						Assert::AreEqual(expected, actual, L"Unexpected read value");
					}

					{
						JsonSerializationReader reader;
						reader.Import(R"(false)");

						bool actual;
						reader.ReadBoolean(actual);

						const bool expected = false;
						Assert::AreEqual(expected, actual, L"Unexpected read value");
					}
				}

				TEST_METHOD(ReadNull)
				{
					JsonSerializationReader reader;
					reader.Import(R"(null)");

					void* actual = nullptr;
					reader.ReadNull(actual);

					void* expected = nullptr;
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadArray)
				{
					JsonSerializationReader reader;
					reader.Import(R"([1,2,3])");

					reader.ReadBeginArray();
					{
						uint32_t actualIndex;
						reader.ReadBeginArrayElement(actualIndex);
						{
							const uint32_t expectedIndex = 0;
							Assert::AreEqual(expectedIndex, actualIndex, L"Unexpected index");

							int32_t actualValue;
							reader.ReadInteger32(actualValue);
							int32_t expectedValue = 1;
							Assert::AreEqual(expectedValue, actualValue, L"Unexpected read value");
						}
						reader.ReadEndArrayElement();

						reader.ReadBeginArrayElement(actualIndex);
						{
							const uint32_t expectedIndex = 1;
							Assert::AreEqual(expectedIndex, actualIndex, L"Unexpected index");

							int32_t actualValue;
							reader.ReadInteger32(actualValue);
							int32_t expectedValue = 2;
							Assert::AreEqual(expectedValue, actualValue, L"Unexpected read value");
						}
						reader.ReadEndArrayElement();

						reader.ReadBeginArrayElement(actualIndex);
						{
							const uint32_t expectedIndex = 2;
							Assert::AreEqual(expectedIndex, actualIndex, L"Unexpected index");

							int32_t actualValue;
							reader.ReadInteger32(actualValue);
							int32_t expectedValue = 3;
							Assert::AreEqual(expectedValue, actualValue, L"Unexpected read value");
						}
						reader.ReadEndArrayElement();
					}
					reader.ReadEndArray();
				}

				TEST_METHOD(ReadObject)
				{
					JsonSerializationReader reader;
					reader.Import(R"({"Age":1,"Friendliness":0.5,"Name":"Mr. Potato Head"})");

					reader.ReadBeginObject();
					{
						{
							std::string actualProperty;
							reader.ReadBeginObjectProperty(actualProperty);
							{
								const std::string expectedProperty = "Age";
								Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

								int32_t actualValue;
								reader.ReadInteger32(actualValue);
								const int32_t expectedValue = 1;
								Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
							}
							reader.ReadEndObjectProperty();
						}

						{
							std::string actualProperty;
							reader.ReadBeginObjectProperty(actualProperty);
							{
								const std::string expectedProperty = "Friendliness";
								Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

								float actualValue;
								reader.ReadFloat(actualValue);
								const float expectedValue = 0.5f;
								Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
							}
							reader.ReadEndObjectProperty();
						}

						{
							std::string actualProperty;
							reader.ReadBeginObjectProperty(actualProperty);
							{
								const std::string expectedProperty = "Name";
								Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

								std::string actualValue;
								reader.ReadString(actualValue);
								const std::string expectedValue = "Mr. Potato Head";
								Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
							}
							reader.ReadEndObjectProperty();
						}
					}
					reader.ReadEndObject();
				}

				TEST_METHOD(ReadComplexObject)
				{
					JsonSerializationReader reader;
					reader.Import(R"({"Eyes":{"Color":"black","Size":5},"Legs":{"PossibleColors":["blue","orange","white"]},"Mouth":null,"Name":"Mr. Potato Head"})");

					reader.ReadBeginObject();
					{
						{
							std::string actualProperty;
							reader.ReadBeginObjectProperty(actualProperty);
							{
								const std::string expectedProperty = "Eyes";
								Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

								reader.ReadBeginObject();
								{
									reader.ReadBeginObjectProperty(actualProperty);
									{
										const std::string expectedProperty = "Color";
										Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");
										
										std::string actualValue;
										reader.ReadString(actualValue);
										const std::string expectedValue = "black";
										Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
									}
									reader.ReadEndObjectProperty();

									reader.ReadBeginObjectProperty(actualProperty);
									{
										const std::string expectedProperty = "Size";
										Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

										int32_t actualValue;
										reader.ReadInteger32(actualValue);
										const int32_t expectedValue = 5;
										Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
									}
									reader.ReadEndObjectProperty();
								}
								reader.ReadEndObject();
							}
							reader.ReadEndObjectProperty();

							reader.ReadBeginObjectProperty(actualProperty);
							{
								const std::string expectedProperty = "Legs";
								Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

								reader.ReadBeginObject();
								{
									reader.ReadBeginObjectProperty(actualProperty);
									{
										const std::string expectedProperty = "PossibleColors";
										Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

										reader.ReadBeginArray();
										{
											uint32_t actualIndex;
											reader.ReadBeginArrayElement(actualIndex);
											{
												const uint32_t expectedIndex = 0;
												Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected index");

												std::string actualValue;
												reader.ReadString(actualValue);
												const std::string expectedValue = "blue";
												Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
											}
											reader.ReadEndArrayElement();

											reader.ReadBeginArrayElement(actualIndex);
											{
												const uint32_t expectedIndex = 1;
												Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected index");

												std::string actualValue;
												reader.ReadString(actualValue);
												const std::string expectedValue = "orange";
												Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
											}
											reader.ReadEndArrayElement();

											reader.ReadBeginArrayElement(actualIndex);
											{
												const uint32_t expectedIndex = 2;
												Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected index");

												std::string actualValue;
												reader.ReadString(actualValue);
												const std::string expectedValue = "white";
												Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
											}
											reader.ReadEndArrayElement();
										}
										reader.ReadEndArray();
									}
									reader.ReadEndObjectProperty();
								}
								reader.ReadEndObject();
							}
							reader.ReadEndObjectProperty();

							reader.ReadBeginObjectProperty(actualProperty);
							{
								const std::string expectedProperty = "Mouth";
								Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

								void* actualValue = nullptr;
								reader.ReadNull(actualValue);
								const void* expectedValue = nullptr;
								Assert::IsTrue(expectedValue == actualValue, L"Unexpected value");
							}
							reader.ReadEndObjectProperty();

							reader.ReadBeginObjectProperty(actualProperty);
							{
								const std::string expectedProperty = "Name";
								Assert::AreEqual(expectedProperty, actualProperty, L"Unexpected property");

								std::string actualValue;
								reader.ReadString(actualValue);
								const std::string expectedValue = "Mr. Potato Head";
								Assert::AreEqual(expectedValue, actualValue, L"Unexpected value");
							}
							reader.ReadEndObjectProperty();
						}
					}
					reader.ReadEndObject();
				}
			};
		}
	}
}