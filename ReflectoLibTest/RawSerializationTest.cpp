#include "Serialization/RawSerialization.h"
#include "Platform.h"

#include "CppUnitTest.h"

#include <algorithm>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft 
{
	namespace VisualStudio 
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<std::vector<byte>>(const std::vector<byte>& vector)
			{
				std::wstring string;
				for (byte b : vector)
				{
					string += std::to_wstring(b);
				}
				return string;
			}
		}
	}
}

namespace LibTest
{
	TEST_CLASS(RawSerializationTest)
	{
	public:
		TEST_METHOD(SerializeDeserializeSimpleByte)
		{
			std::vector<byte> bytes;

			const byte i = 42;
			RawSerialization::Serialize(i, bytes);

			const std::size_t expectedByteSize = sizeof(i);
			Assert::AreEqual(expectedByteSize, bytes.size(), L"Serialization size is unexpected");

			const std::vector<byte> expectedBytes = { 42 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			byte j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeInt32)
		{
			std::vector<byte> bytes;
			
			const int32_t i = 259;
			RawSerialization::Serialize(i, bytes);
			
			const std::size_t expectedByteSize = sizeof(i);
			Assert::AreEqual(expectedByteSize, bytes.size(), L"Serialization size is unexpected");
			
			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 3, 1, 0, 0 } : std::vector<byte>{ 0, 0, 1, 3 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeInt32Negative)
		{
			std::vector<byte> bytes;

			const int32_t i = -259;
			RawSerialization::Serialize(i, bytes);

			const std::size_t expectedByteSize = sizeof(i);
			Assert::AreEqual(expectedByteSize, bytes.size(), L"Serialization size is unexpected");

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 253, 254, 255, 255 } : std::vector<byte>{ 255, 255, 254, 253 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeUnsignedInt32)
		{
			std::vector<byte> bytes;

			const  uint32_t i = 424242;
			RawSerialization::Serialize(i, bytes);

			const std::size_t expectedByteSize = sizeof(i);
			Assert::AreEqual(expectedByteSize, bytes.size(), L"Serialization size is unexpected");

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 50, 121, 6, 0 } : std::vector<byte>{ 0, 6, 121, 50 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			uint32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeInt64)
		{
			std::vector<byte> bytes;

			constexpr int64_t i = 4294967296;
			RawSerialization::Serialize(i, bytes);

			const std::size_t expectedByteSize = sizeof(i);
			Assert::AreEqual(expectedByteSize, bytes.size(), L"Serialization size is unexpected");

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 0, 0, 0, 0, 1, 0, 0, 0 } : std::vector<byte>{ 0, 0, 0, 0, 1, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int64_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeIntValue0)
		{
			std::vector<byte> bytes;
			
			const int32_t i = 0;
			RawSerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = std::vector<byte>{ 0, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeIntValueMax)
		{
			std::vector<byte> bytes;

			const int32_t i = std::numeric_limits<int32_t>::max();
			RawSerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 255, 255, 255, 127 } : std::vector<byte>{ 127, 255, 255, 255 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeIntValueMin)
		{
			std::vector<byte> bytes;

			const int32_t i = std::numeric_limits<int32_t>::min();
			RawSerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = Platform::IsLittleEndian() ? std::vector<byte>{ 0, 0, 0, 128 } : std::vector<byte>{ 128, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}


		TEST_METHOD(SerializeDeserializeUnsignedIntValueMax)
		{
			std::vector<byte> bytes;

			const uint32_t i = std::numeric_limits<uint32_t>::max();
			RawSerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = std::vector<byte>{ 255, 255, 255, 255 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			uint32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}

		TEST_METHOD(SerializeDeserializeUnsignedIntValueMin)
		{
			std::vector<byte> bytes;

			const uint32_t i = std::numeric_limits<uint32_t>::min();
			RawSerialization::Serialize(i, bytes);

			const std::vector<byte> expectedBytes = std::vector<byte>{ 0, 0, 0, 0 };
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			uint32_t j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did yield expected value");
		}
	};
}
