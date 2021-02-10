#pragma once

#include "Serialization/TextSerialization.h"
#include "Serialization/Writer/ISerializationWriter.h"

#include "Common/Definitions.h"
#include "jsoncpp/json/json.h"

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
				PushNewElement();
			}
			
			virtual void WriteInteger32(int32_t value) override
			{
				GetCurrentElement().Json = value;
			}

			virtual void WriteInteger64(int64_t value) override
			{
				GetCurrentElement().Json = value;
			}

			virtual void WriteFloat(float value) override
			{
				GetCurrentElement().Json = value;
			}

			virtual void WriteDouble(double value) override
			{
				GetCurrentElement().Json = value;
			}

			virtual void WriteString(const std::string& value) override
			{
				GetCurrentElement().Json = value;
			}

			virtual void WriteBoolean(bool value) override
			{
				GetCurrentElement().Json = value;
			}

			virtual void WriteBeginObjectProperty(const std::string& propertyName) override
			{
				Element elem;
				elem.PropertyName = propertyName;
				PushNewElement(elem);
			}

			virtual void WriteEndObjectProperty() override
			{
				Element elem = GetCurrentElement();
				PopElement();
				GetCurrentElement().Json[elem.PropertyName] = elem.Json;
			}

			virtual void WriteBeginArrayElement() override
			{
				PushNewElement();
			}

			virtual void WriteEndArrayElement() override
			{
				Element elem = GetCurrentElement();
				PopElement();
				GetCurrentElement().Json.append(elem.Json);
			}

			void Transpose(std::string& str)
			{
				std::unique_ptr<Json::StreamWriter> writer = [&] {
					Json::StreamWriterBuilder builder;
					builder["indentation"] = "";
					return std::unique_ptr<Json::StreamWriter>(builder.newStreamWriter());
				}();

				{
					std::stringstream ss;
					writer->write(GetCurrentElement().Json, &ss);
					str = ss.str();
				}
			}

		private:
			struct Element
			{
				std::string PropertyName;
				Json::Value Json;
			};

			void PushNewElement(const Element& element = Element())
			{
				_stack.push(element);
			}

			void PopElement()
			{
				return _stack.pop();
			}

			Element& GetCurrentElement()
			{
				assert(!_stack.empty());
				return _stack.top();
			}

			const Element& GetCurrentElement() const
			{
				assert(!_stack.empty());
				return _stack.top();
			}

			std::stack<Element> _stack;
			std::stack<std::string> _propertyNames;
		};
	}
}