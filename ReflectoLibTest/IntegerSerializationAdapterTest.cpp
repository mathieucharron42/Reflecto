#include "TestExtensions.h"

#include "Platform.h"
#include "Serialization/BinarySerialization.h"

#include "CppUnitTest.h"

#include <algorithm>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LibTest
{
	TEST_CLASS(SerializationTest)
	{
	public:
		TEST_METHOD(SerializeDeserializeSimpleByte)
		{
			std::vector<byte> bytes;

			const byte i = 42;
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = { 42 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			byte j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}
	};
}
