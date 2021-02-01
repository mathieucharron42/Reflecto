#include "Common/Utils/StringExt.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"
#include "Serialization/SerializationFormat.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerializationMetaType.h"
#include "Serialization/Strategy/JsonSerializationStrategy.h"
#include "Serialization/TextSerialization.h"

#include "CppUnitTest.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Serialization
	{
		namespace Test
		{
			TEST_CLASS(SerializerTest)
			{
			public:
				TEST_METHOD(SerializeInt)
				{
				/*	const Type::TypeDescriptor intDescriptor = Type::TypeDescriptorFactory<int>().Build();

					Serializer<JsonSerializationFormat> serializer;
					serializer.RegisterType(intDescriptor, JsonSerializationStrategy::SerializeInt);

					int i = 42;

					std::vector<byte> bytes;
					serializer.Serialize(i, bytes);

					std::string expectedStr = Utils::StringExt::Format(std::string(R"({"type":"int","value":%i})"), i);
					std::string actual;
					TextSerialization::Deserialize(bytes, actual);
					Assert::AreEqual(expectedStr, actual, L"Serialized bytes are unexpected!");

					std::vector<byte> expectedBytes;
					TextSerialization::Serialize(expectedStr, expectedBytes);
					Assert::IsTrue(bytes == expectedBytes, L"Serialized bytes are unexpected!");*/
				}
			};
		}
	}
}