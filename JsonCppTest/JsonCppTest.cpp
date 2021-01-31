
#include "jsoncpp/json/json.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonCppTest
{
	TEST_CLASS(JsonCppTest)
	{
	public:
		
		TEST_METHOD(WriteJson)
		{
			Json::StreamWriterBuilder builder;
			builder["indentation"] = "";
			std::unique_ptr<Json::StreamWriter> writer{ builder.newStreamWriter() };


			Json::Value root;
			root["Property1"] = 4;

			std::stringstream ss;
			writer->write(root, &ss);

			const std::string expected = R"({"Property1":4)";
			const std::string actual = ss.str();
			Assert::AreEqual(expected, actual, L"Unexpected json");
		}
	};
}
