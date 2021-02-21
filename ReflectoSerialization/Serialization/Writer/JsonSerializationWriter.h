#pragma once

#include "Serialization/TextSerialization.h"
#include "Serialization/Writer/ISerializationWriter.h"

#include "Common/Definitions.h"
#include "jsoncpp/json.h"

#include <assert.h>
#include <stack>
#include <sstream>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		class JsonSerializationWriter : public ISerializationWriter
		{
		public:
			JsonSerializationWriter()
			{
				
			}
			
			virtual void WriteInteger32(int32_t value) override
			{
				PushElement(JsonElement(value));
			}

			virtual void WriteInteger64(int64_t value) override
			{
				PushElement(JsonElement(value));
			}

			virtual void WriteFloat(float value) override
			{
				PushElement(JsonElement(value));
			}

			virtual void WriteDouble(double value) override
			{
				PushElement(JsonElement(value));
			}

			virtual void WriteString(const std::string& value) override
			{
				PushElement(JsonElement(value));
			}

			virtual void WriteBoolean(bool value) override
			{
				PushElement(JsonElement(value));
			}

			virtual void WriteNull() override
			{
				PushElement(JsonElement(Json::nullValue));
			}

			virtual void WriteBeginObject() override
			{
				PushElement(JsonElement(Json::objectValue));
			}

			virtual void WriteEndObject() override
			{

			}

			virtual void WriteBeginObjectProperty(const std::string& propertyName) override
			{
				PushPropertyName(propertyName);
			}

			virtual void WriteEndObjectProperty() override
			{
				const std::string propertyName = PopPropertyName();
				const JsonElement element = PopElement();
				GetCurrentElement()[propertyName] = element;
			}

			virtual void WriteBeginArray() override
			{
				PushElement(JsonElement(Json::arrayValue));
			}

			virtual void WriteEndArray() override
			{
				
			}

			virtual void WriteBeginArrayElement() override
			{
				
			}

			virtual void WriteEndArrayElement() override
			{
				const JsonElement element = PopElement();
				GetCurrentElement().append(element);
			}

			void Transpose(std::string& str)
			{
				std::unique_ptr<Json::StreamWriter> writer = [&] {
					Json::StreamWriterBuilder builder;
					builder["indentation"] = "";
					return std::unique_ptr<Json::StreamWriter>(builder.newStreamWriter());
				}();

				if (HasCurrentElement())
				{
					std::stringstream ss;
					writer->write(GetCurrentElement(), &ss);
					str = ss.str();
				}
			}

		private:
			using JsonElement = Json::Value;

			void PushElement(const JsonElement& element)
			{
				_stack.push(element);
			}

			JsonElement PopElement()
			{
				JsonElement element = GetCurrentElement();
				_stack.pop();
				return element;
			}

			JsonElement& GetCurrentElement()
			{
				assert(HasCurrentElement());
				return _stack.top();
			}

			const bool HasCurrentElement() const
			{
				return !_stack.empty();
			}

			void PushPropertyName(const std::string &propertyName)
			{
				_propertyNames.push(propertyName);
			}

			std::string PopPropertyName()
			{
				std::string propertyName = GetCurrentPropertyName();
				_propertyNames.pop();
				return propertyName;
			}

			std::string& GetCurrentPropertyName()
			{
				assert(HasCurrentPropertyName());
				return _propertyNames.top();
			}

			const bool HasCurrentPropertyName() const
			{
				return !_propertyNames.empty();
			}

			std::stack<JsonElement> _stack;
			std::stack<std::string> _propertyNames;
		};
	}
}