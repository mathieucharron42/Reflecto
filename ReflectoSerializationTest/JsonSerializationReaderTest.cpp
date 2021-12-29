#include "Serialization/Reader/JsonSerializationReader.h"

#include "Utils/StringExt.h"

#include <CppUnitTest.h>
#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<uint32_t>>(const std::vector<uint32_t>& vector)
{
	return Reflecto::Utils::StringExt::Join<std::wstring>(vector, L",");
}

template<>
inline std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<std::string>>(const std::vector<std::string>& vector)
{
	return Reflecto::Utils::StringExt::ToWstring(Reflecto::Utils::StringExt::Join<std::string>(vector, ","));
}

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
					/////////////
					// Arrange
					const std::string expected = R"(test)";
					JsonSerializationReader reader;
					reader.Import(R"("test")");
				
					/////////////
					// Act
					bool success = true;
					std::string actual;
					success &= reader.ReadString(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadInt32)
				{
					/////////////
					// Arrange
					const int32_t expected = 1;
					JsonSerializationReader reader;
					reader.Import(R"(1)");
					
					/////////////
					// Act
					bool success = true;
					std::int32_t actual;
					success &= reader.ReadInteger32(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadInt64)
				{
					/////////////
					// Arrange
					const std::int64_t expected = 33445566778899;
					JsonSerializationReader reader;
					reader.Import(R"(33445566778899)");

					/////////////
					// Act
					bool success = true;
					std::int64_t actual;
					success &= reader.ReadInteger64(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadFloat)
				{
					/////////////
					// Arrange
					const float expected = 0.5f;
					JsonSerializationReader reader;
					reader.Import(R"(0.5)");

					/////////////
					// Act
					bool success = true;
					float actual;
					success &= reader.ReadFloat(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadDouble)
				{
					/////////////
					// Arrange
					const double expected = 0.5;
					JsonSerializationReader reader;
					reader.Import(R"(0.5)");

					/////////////
					// Act
					bool success = true;
					double actual;
					success &= reader.ReadDouble(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadBool)
				{

					/////////////
					// Arrange
					const bool expected = true;
					JsonSerializationReader reader;
					reader.Import(R"(true)");

					/////////////
					// Act
					bool success = true;
					bool actual;
					reader.ReadBoolean(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadNull)
				{
					/////////////
					// Arrange
					void* expected = nullptr;
					JsonSerializationReader reader;
					reader.Import(R"(null)");

					/////////////
					// Act
					bool success = true;
					void* actual = nullptr;
					success &= reader.ReadNull(actual);

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expected, actual, L"Unexpected read value");
				}

				TEST_METHOD(ReadArray)
				{
					/////////////
					// Arrange
					JsonSerializationReader reader;
					reader.Import(R"([1,2,3])");
					const uint32_t expectedFirstIndex = 0;
					const int32_t expectedFirstValue = 1;
					const uint32_t expectedSecondIndex = 1;
					const int32_t expectedSecondValue = 2;
					const uint32_t expectedThirdIndex = 2;
					const int32_t expectedThirdValue = 3;

					/////////////
					// Act
					bool success = true;
					int32_t actualFirstValue;
					int32_t actualSecondValue;
					int32_t actualThirdValue;

					uint32_t actualFirstIndex;
					uint32_t actualSecondIndex;
					uint32_t actualThirdIndex;
					success &= reader.ReadBeginArray();
					{
						success &= reader.ReadBeginArrayElement(actualFirstIndex);
						{
							success &= reader.ReadInteger32(actualFirstValue);
						}
						success &= reader.ReadEndArrayElement();

						success &= reader.ReadBeginArrayElement(actualSecondIndex);
						{
							success &= reader.ReadInteger32(actualSecondValue);
						}
						success &= reader.ReadEndArrayElement();

						success &= reader.ReadBeginArrayElement(actualThirdIndex);
						{
							success &= reader.ReadInteger32(actualThirdValue);
						}
						success &= reader.ReadEndArrayElement();
					}
					success &= reader.ReadEndArray();

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expectedFirstIndex, actualFirstIndex, L"Unexpected first index");
					Assert::AreEqual(expectedFirstValue, actualFirstValue, L"Unexpected first read value");
					Assert::AreEqual(expectedSecondIndex, actualSecondIndex, L"Unexpected second index");
					Assert::AreEqual(expectedSecondValue, actualSecondValue, L"Unexpected second read value");
					Assert::AreEqual(expectedThirdIndex, actualThirdIndex, L"Unexpected third index");
					Assert::AreEqual(expectedThirdValue, actualThirdValue, L"Unexpected third read value");
				}

				TEST_METHOD(ReadObject)
				{
					/////////////
					// Arrange
					JsonSerializationReader reader;
					reader.Import(R"({"Age":1,"Friendliness":0.5,"Name":"Mr. Potato Head"})");
					
					const std::string expectedFirstProperty = "Age";
					const int32_t expectedFirstValue = 1;
					const std::string expectedSecondProperty = "Friendliness";
					const float expectedSecondValue = 0.5f;
					const std::string expectedThirdProperty = "Name";
					const std::string expectedThirdValue = "Mr. Potato Head";

					/////////////
					// Act
					bool success = true;
					std::string actualFirstProperty;
					int32_t actualFirstValue;
					std::string actualSecondProperty;
					float actualSecondValue;
					std::string actualThirdProperty;
					std::string actualThirdValue;

					success &= reader.ReadBeginObject();
					{
						success &= reader.ReadBeginObjectProperty(actualFirstProperty);
						{
							success &= reader.ReadInteger32(actualFirstValue);
						}
						success &= reader.ReadEndObjectProperty();
											
						success &= reader.ReadBeginObjectProperty(actualSecondProperty);
						{
							success &= reader.ReadFloat(actualSecondValue);
						}		
						success &= reader.ReadEndObjectProperty();
						
						success &= reader.ReadBeginObjectProperty(actualThirdProperty);
						{
							success &= reader.ReadString(actualThirdValue);
						}
						success &= reader.ReadEndObjectProperty();
					}
					success &= reader.ReadEndObject();

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					Assert::AreEqual(expectedFirstProperty, actualFirstProperty, L"Unexpected property");
					Assert::AreEqual(expectedFirstValue, actualFirstValue, L"Unexpected value");
					Assert::AreEqual(expectedSecondProperty, actualSecondProperty, L"Unexpected property");
					Assert::AreEqual(expectedSecondValue, actualSecondValue, L"Unexpected value");		
					Assert::AreEqual(expectedThirdProperty, actualThirdProperty, L"Unexpected property");
					Assert::AreEqual(expectedThirdValue, actualThirdValue, L"Unexpected value");
				}

				TEST_METHOD(ReadNestedArray)
				{
					/////////////
					// Arrange
					JsonSerializationReader reader;
					reader.Import(R"([["hey","you", "wow"],["potato","test"],["hello","tomorrow","yesterday","Aluminum"]])");
					const std::vector<std::vector<uint32_t>> expectedIndexes = { {0,1,2}, {0,1}, {0,1,2,3} };
					const std::vector<std::vector<std::string>> expectedValues = { {"hey","you", "wow"}, {"potato","test"}, {"hello","tomorrow","yesterday","Aluminum"} };
					
					/////////////
					// Act
					bool success = true;
					std::vector<uint32_t> actualTopLevelIndexes = { 0, 0, 0 };
					std::vector<std::vector<uint32_t>> actualIndexes = { {0, 0, 0}, {0, 0}, {0, 0, 0, 0} };
					std::vector<std::vector<std::string>> actualValues = { {"", "", ""}, {"", ""}, {"", "", "", ""}};
					
					success &= reader.ReadBeginArray();
					{
						success &= reader.ReadBeginArrayElement(actualTopLevelIndexes[0]);
						{
							success &= reader.ReadBeginArray();
							{
								success &= reader.ReadBeginArrayElement(actualIndexes[0][0]);
								{
									success &= reader.ReadString(actualValues[0][0]);
								}
								success &= reader.ReadEndArrayElement();
								success &= reader.ReadBeginArrayElement(actualIndexes[0][1]);
								{
									success &= reader.ReadString(actualValues[0][1]);
								}
								success &= reader.ReadEndArrayElement();
								success &= reader.ReadBeginArrayElement(actualIndexes[0][2]);
								{
									success &= reader.ReadString(actualValues[0][2]);
								}
								success &= reader.ReadEndArrayElement();
							}
							success &= reader.ReadEndArray();
						}
						success &= reader.ReadEndArrayElement();

						success &= reader.ReadBeginArrayElement(actualTopLevelIndexes[1]);
						{
							success &= reader.ReadBeginArray();
							{
								success &= reader.ReadBeginArrayElement(actualIndexes[1][0]);
								{
									success &= reader.ReadString(actualValues[1][0]);
								}
								success &= reader.ReadEndArrayElement();
								success &= reader.ReadBeginArrayElement(actualIndexes[1][1]);
								{
									success &= reader.ReadString(actualValues[1][1]);
								}
								success &= reader.ReadEndArrayElement();
							}
							success &= reader.ReadEndArray();
						}
						success &= reader.ReadEndArrayElement();
						
						success &= reader.ReadBeginArrayElement(actualTopLevelIndexes[2]);
						{
							success &= reader.ReadBeginArray();
							{
								success &= reader.ReadBeginArrayElement(actualIndexes[2][0]);
								{
									success &= reader.ReadString(actualValues[2][0]);
								}
								success &= reader.ReadEndArrayElement();
								success &= reader.ReadBeginArrayElement(actualIndexes[2][1]);
								{
									success &= reader.ReadString(actualValues[2][1]);
								}
								success &= reader.ReadEndArrayElement();
								success &= reader.ReadBeginArrayElement(actualIndexes[2][2]);
								{
									success &= reader.ReadString(actualValues[2][2]);
								}
								success &= reader.ReadEndArrayElement();
								success &= reader.ReadBeginArrayElement(actualIndexes[2][3]);
								{
									success &= reader.ReadString(actualValues[2][3]);
								}
								success &= reader.ReadEndArrayElement();
							}
							success &= reader.ReadEndArray();
						}
						success &= reader.ReadEndArrayElement();
						
					}
					success &= reader.ReadEndArray();

					/////////////
					// Assert
					Assert::IsTrue(success, L"Unexpected operation failure");
					for (uint32_t i = 0; i < expectedIndexes.size(); ++i)
					{
						Assert::AreEqual(expectedIndexes[i], actualIndexes[i], L"Unexpected indexes");
					}
					for (uint32_t i = 0; i < expectedValues.size(); ++i)
					{
						Assert::AreEqual(expectedValues[i], actualValues[i], L"Unexpected values");
					}
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