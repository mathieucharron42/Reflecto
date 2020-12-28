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

			const std::vector<byte> expectedBytes = [&] {
				std::vector<byte> expectedBytes(expectedByteSize, 0);
				if (Platform::IsLittleEndian())
				{
					expectedBytes[0] = 42;
				}
				else
				{
					expectedBytes[expectedBytes.size() - 1] = 42;
				}
				return expectedBytes;
			}();
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			byte j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did not work as expected");
		}

		TEST_METHOD(SerializeDeserializeSimpleInt)
		{
			std::vector<byte> bytes;
			
			const int i = 259;
			RawSerialization::Serialize(i, bytes);
			
			const std::size_t expectedByteSize = sizeof(i);
			Assert::AreEqual(expectedByteSize, bytes.size(), L"Serialization size is unexpected");
			
			const std::vector<byte> expectedBytes = [&] {
				std::vector<byte> expectedBytes;
				if (Platform::IsLittleEndian())
				{
					expectedBytes = { 3, 1, 0, 0 };
				}
				else
				{
					expectedBytes = { 0, 0, 1, 3 };
				}
				return expectedBytes;
			}();
			Assert::AreEqual(expectedBytes, bytes, L"Serialized bytes are unexpected");

			int j;
			RawSerialization::Deserialize(bytes, j);
			Assert::AreEqual(i, j, L"Deserialization did not work as expected");
		}
	};
}
