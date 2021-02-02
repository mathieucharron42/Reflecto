#pragma once

#include "Serialization/TextSerialization.h"

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
		class JsonSerializationWriter
		{
		public:
			JsonSerializationWriter()
			{
				PushNewElement();
			}
			
			void WriteInteger32(int32_t value)
			{
				GetCurrentElement().Json = value;
			}

			void WriteInteger64(int64_t value)
			{
				GetCurrentElement().Json = value;
			}

			void WriteFloat(float value)
			{
				GetCurrentElement().Json = value;
			}

			void WriteDouble(double value)
			{
				GetCurrentElement().Json = value;
			}

			void WriteString(const std::string& value)
			{
				GetCurrentElement().Json = value;
			}

			void WriteBoolean(bool value)
			{
				GetCurrentElement().Json = value;
			}

			void WriteWriter(const std::string& propertyName, const JsonSerializationWriter& value)
			{
				GetCurrentElement() = value.GetCurrentElement();
			}

			void WriteBeginObjectProperty(const std::string& propertyName)
			{
				Element elem;
				elem.PropertyName = propertyName;
				PushNewElement(elem);
			}

			void WriteEndObjectProperty()
			{
				Element elem = GetCurrentElement();
				PopElement();
				GetCurrentElement().Json[elem.PropertyName] = elem.Json;
			}

			void WriteBeginArrayElement()
			{
				PushNewElement();
			}

			void WriteEndArrayElement()
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

				std::stringstream ss;
				writer->write(GetCurrentElement().Json, &ss);
				str = ss.str();
			}

			void Transpose(std::vector<byte>& bytes)
			{
				std::string str;
				Transpose(str);

				TextSerialization::Serialize(str, bytes);
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