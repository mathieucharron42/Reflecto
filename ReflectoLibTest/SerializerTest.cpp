#include "TestExtensions.h"

#include "Serialization/Serializer.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Reflecto;

namespace LibTest
{
	TEST_CLASS(SerializerTest)
	{
	public:
		TEST_METHOD(SerializeTest)
		{
			//Serializer serializer;

			std::vector<byte> bytes;

			//int i;
			//serializer.Serialize(i, bytes);
		}
	};
}
