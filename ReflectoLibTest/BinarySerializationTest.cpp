//#include "TestExtensions.h"

#include "TestExtensions.h"

#include "Platform.h"
#include "Serialization/BinarySerialization.h"

#include "CppUnitTest.h"

#include <algorithm>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Reflecto;

namespace LibTest
{
	TEST_CLASS(BinarySerializationTest)
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

		TEST_METHOD(SerializeDeserializeInt32)
		{
			std::vector<byte> bytes;
			
			const int32_t i = 259;
			BinarySerialization::Serialize(i, bytes);
			
			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 3, 1, 0, 0 } : std::vector<byte>{ 0, 0, 1, 3 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeInt32Negative)
		{
			std::vector<byte> bytes;

			const int32_t i = -259;
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 253, 254, 255, 255 } : std::vector<byte>{ 255, 255, 254, 253 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeUnsignedInt32)
		{
			std::vector<byte> bytes;

			const  uint32_t i = 424242;
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 50, 121, 6, 0 } : std::vector<byte>{ 0, 6, 121, 50 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			uint32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeInt64)
		{
			std::vector<byte> bytes;

			constexpr int64_t i = 4294967296;
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 0, 0, 0, 0, 1, 0, 0, 0 } : std::vector<byte>{ 0, 0, 0, 0, 1, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int64_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeIntValue0)
		{
			std::vector<byte> bytes;
			
			const int32_t i = 0;
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = std::vector<byte>{ 0, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeIntValueMax)
		{
			std::vector<byte> bytes;

			constexpr int32_t i = std::numeric_limits<int32_t>::max();
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 255, 255, 255, 127 } : std::vector<byte>{ 127, 255, 255, 255 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeIntValueMin)
		{
			std::vector<byte> bytes;

			constexpr int32_t i = std::numeric_limits<int32_t>::min();
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 0, 0, 0, 128 } : std::vector<byte>{ 128, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeUnsignedIntValueMax)
		{
			std::vector<byte> bytes;

			constexpr uint32_t i = std::numeric_limits<uint32_t>::max();
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = std::vector<byte>{ 255, 255, 255, 255 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			uint32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeUnsignedIntValueMin)
		{
			std::vector<byte> bytes;

			constexpr uint32_t i = std::numeric_limits<uint32_t>::min();
			BinarySerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = std::vector<byte>{ 0, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			uint32_t j;
			BinarySerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeFloat)
		{
			std::vector<byte> bytes;

			constexpr float f = 3.14169f;
			BinarySerialization::Serialize(f, bytes);

			const std::size_t expectedByteSize = sizeof(f);
			Assert::AreEqual(expectedByteSize, bytes.size(), L"Serialization size is unexpected");

			// https://en.wikipedia.org/wiki/Floating-point_arithmetic
			// const std::vector<byte> expectedBytes = ?
			// Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			float h;
			BinarySerialization::Deserialize(bytes, h);
			Assert::AreEqual(f, h, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeString)
		{
			std::vector<byte> bytes;

			std::string s = "This is a simple test";
			BinarySerialization::Serialize(s, bytes);

			const std::vector<byte> expectedBytes = { 84, 104, 105, 115, 32, 105, 115, 32, 97, 32, 115, 105, 109, 112, 108, 101, 32, 116, 101, 115, 116, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			std::string t;
			BinarySerialization::Deserialize(bytes, t);
			Assert::AreEqual(s, t, L"Deserialization did yield expected value");
		}
	};
}
