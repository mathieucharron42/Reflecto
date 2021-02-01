
#include "jsoncpp/json/json.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonCppTest
{
	TEST_CLASS(JsonCppTest)
	{
	public:
		TEST_METHOD(WriteString)
		{
			Json::Value root;
			root = "Test";

			std::stringstream ss;
			std::unique_ptr<Json::StreamWriter> writer = BuildWriter();
			writer->write(root, &ss);

			const std::string expected = R"("Test")";
			const std::string actual = ss.str();
			Assert::AreEqual(expected, actual, L"Unexpected json");
		}

		TEST_METHOD(WriteObject)
		{
			Json::Value root;
			root["Name"] = "Mr. Potato Head";

			std::stringstream ss;
			std::unique_ptr<Json::StreamWriter> writer = BuildWriter();
			writer->write(root, &ss);

			const std::string expected = R"({"Name":"Mr. Potato Head"})";
			const std::string actual = ss.str();
			Assert::AreEqual(expected, actual, L"Unexpected json");
		}

		TEST_METHOD(WriteArray)
		{
			Json::Value root;
			root.append("Toy");
			root.append("Role model");
			root.append("Super hero");

			std::stringstream ss;
			std::unique_ptr<Json::StreamWriter> writer = BuildWriter();
			writer->write(root, &ss);

			const std::string expected = R"(["Toy","Role model","Super hero"])";
			const std::string actual = ss.str();
			Assert::AreEqual(expected, actual, L"Unexpected json");
		}

	private:
		std::unique_ptr<Json::StreamWriter> BuildWriter() const
		{
			Json::StreamWriterBuilder builder;
			builder["indentation"] = "";
			std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
			return writer;
		}
	};
}
