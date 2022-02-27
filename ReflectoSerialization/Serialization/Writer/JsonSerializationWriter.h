#pragma once

#include "Serialization/Writer/ISerializationWriter.h"

#include "Common/Definitions.h"
#include "jsoncpp/json.h"

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

			virtual bool WriteInteger32(int32_t value) override
			{
				return PushElement(JsonElement(value));
			}

			virtual bool WriteInteger64(int64_t value) override
			{
				return PushElement(JsonElement(value));
			}

			virtual bool WriteFloat(float value) override
			{
				return PushElement(JsonElement(value));
			}

			virtual bool WriteDouble(double value) override
			{
				return PushElement(JsonElement(value));
			}

			virtual bool WriteString(const std::string& value) override
			{
				return PushElement(JsonElement(value));
			}

			virtual bool WriteBoolean(bool value) override
			{
				return PushElement(JsonElement(value));
			}

			virtual bool WriteNull() override
			{
				return PushElement(JsonElement(Json::nullValue));
			}

			virtual bool WriteBeginObject() override
			{
				return PushElement(JsonElement(Json::objectValue));
			}

			virtual bool WriteEndObject() override
			{
				bool success = false;
				JsonElement* element;
				if (GetCurrentElement(element) && element->type() == Json::objectValue)
				{
					success = true;
				}
				return success;
			}

			virtual bool WriteBeginObjectProperty(const std::string& propertyName) override
			{
				bool success = false;
				JsonElement* element;
				if (GetCurrentElement(element) && element->type() == Json::objectValue)
				{
					success = PushPropertyName(propertyName);
				}
				return success;
			}

			virtual bool WriteEndObjectProperty() override
			{
				bool success = false;
				std::string propertyName;
				if (PopPropertyName(propertyName))
				{
					JsonElement element;
					if (PopElement(element))
					{
						JsonElement* parent;
						if (GetCurrentElement(parent) && parent->type() == Json::objectValue)
						{
							(*parent)[propertyName] = element;
							success = true;
						}
					}
				}
				return success;
			}

			virtual bool WriteBeginArray() override
			{
				return PushElement(JsonElement(Json::arrayValue));
			}

			virtual bool WriteEndArray() override
			{
				bool success = false;
				JsonElement* element;
				if (GetCurrentElement(element) && element->type() == Json::arrayValue)
				{
					success = true;
				}
				return success;
			}

			virtual bool WriteBeginArrayElement() override
			{
				bool success = false;
				JsonElement* element;
				if (GetCurrentElement(element) && element->type() == Json::arrayValue)
				{
					success = true;
				}
				return success;
			}

			virtual bool WriteEndArrayElement() override
			{
				bool success = false;
				JsonElement element;
				if (PopElement(element))
				{
					JsonElement* parent;
					if (GetCurrentElement(parent) && parent->type() == Json::arrayValue)
					{
						parent->append(element);
						success = true;
					}
					
				}
				return success;
			}

			bool Export(std::ostream& outputStream)
			{
				std::unique_ptr<Json::StreamWriter> writer = [&] {
					Json::StreamWriterBuilder builder;
					builder["indentation"] = "";
					return std::unique_ptr<Json::StreamWriter>(builder.newStreamWriter());
				}();

				JsonElement* element;
				if (GetCurrentElement(element))
				{
					writer->write(*element, &outputStream);
				}

				return true;
			}

		private:
			using JsonElement = Json::Value;

			bool PushElement(const JsonElement& element)
			{
				_stack.push(element);
				return true;
			}

			bool PopElement(JsonElement& element)
			{
				bool success = false;
				JsonElement* ptr;
				if (GetCurrentElement(ptr))
				{
					element = *ptr;
					_stack.pop();
					success = true;
				}
				return success;
			}

			bool GetCurrentElement(JsonElement*& element)
			{
				bool success = false;
				if (HasCurrentElement())
				{
					element = &_stack.top();
					success = true;
				}
				return success;
			}

			bool HasCurrentElement() const
			{
				return !_stack.empty();
			}

			bool PushPropertyName(const std::string &propertyName)
			{
				_propertyNames.push(propertyName);
				return true;
			}

			bool PopPropertyName(std::string& propertyName)
			{
				bool success = false;
				if (GetCurrentPropertyName(propertyName))
				{
					_propertyNames.pop();
					success = true;
				}
				return success;
			}

			bool GetCurrentPropertyName(std::string& propertyName)
			{
				bool success = false;
				if (HasCurrentPropertyName())
				{
					propertyName = _propertyNames.top();
					success = true;
				}
				return success;
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