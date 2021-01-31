#include "Serialization/TextSerialization.h"
#include "Common/Utils/StringExt.h"

#include "CppUnitTest.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<std::vector<Reflecto::byte>>(const std::vector<Reflecto::byte>& vector)
			{
				std::wstring elements = Reflecto::Utils::StringExt::Join<std::wstring>(vector, L",", [](Reflecto::byte b) { return std::to_wstring(b); });
				return Reflecto::Utils::StringExt::Format<std::wstring>(L"[%s]", elements.c_str());
			}
		}
	}
}

namespace Reflecto
{
	namespace Serialization
	{
		namespace Test
		{
			TEST_CLASS(TextSerializationTest)
			{
			public:
				TEST_METHOD(SerializeString)
				{
					std::vector<byte> actual;
					TextSerialization::Serialize(std::string("Here is a test"), actual);

					std::vector<byte> expected = { 72, 101, 114, 101, 32, 105, 115, 32, 97, 32, 116, 101, 115, 116, 0 };
					Assert::AreEqual(expected, actual, L"Serialize bytes are not expected");
				}

				TEST_METHOD(DeserializeString)
				{
					std::string actual;
					std::vector<byte> bytes= { 72, 101, 114, 101, 32, 105, 115, 32, 97, 32, 116, 101, 115, 116, 0 };
					TextSerialization::Deserialize(bytes, actual);

					std::string expected = "Here is a test";
					Assert::AreEqual(expected, actual, L"Deserialize string is not expected");
				}

				TEST_METHOD(SerializeDeserializeString)
				{
					std::vector<byte> bytes;

					const std::string expected = "Here is a test";
					std::string actual;
					TextSerialization::Serialize(expected, bytes);
					TextSerialization::Deserialize(bytes, actual);

					Assert::AreEqual(expected, actual, L"Serialize+Deserialize did not yield expected symmetric result");
				}
			};
		}
	}
}