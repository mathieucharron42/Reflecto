#pragma once

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
				GetCurrentElement() = value;
			}

			void WriteInteger64(int64_t value)
			{
				GetCurrentElement() = value;
			}

			void WriteFloat(float value)
			{
				GetCurrentElement() = value;
			}

			void WriteDouble(double value)
			{
				GetCurrentElement() = value;
			}

			void WriteString(const std::string& value)
			{
				GetCurrentElement() = value;
			}

			void WriteBoolean(bool value)
			{
				GetCurrentElement() = value;
			}

			void WriteWriter(const std::string& propertyName, const JsonSerializationWriter& value)
			{
				GetCurrentElement() = value.GetCurrentElement();
			}

			void WriteBeginObjectProperty(const std::string& propertyName)
			{
				_currentPropertyName = propertyName;
				PushNewElement();
			}

			void WriteEndObjectProperty()
			{
				Json::Value elem = GetCurrentElement();
				PopElement();
				GetCurrentElement()[_currentPropertyName] = elem;
				_currentPropertyName.clear();
			}

			void WriteBeginArrayElement()
			{
				PushNewElement();
			}

			void WriteEndArrayElement()
			{
				Json::Value elem = GetCurrentElement();
				PopElement();
				GetCurrentElement().append(elem);
			}

			void Transpose(std::string& str)
			{
				std::unique_ptr<Json::StreamWriter> writer = [&] {
					Json::StreamWriterBuilder builder;
					builder["indentation"] = "";
					return std::unique_ptr<Json::StreamWriter>(builder.newStreamWriter());
				}();

				std::stringstream ss;
				writer->write(GetCurrentElement(), &ss);
				str = ss.str();
			}

		private:
			void PushNewElement(const Json::Value& obj = Json::Value())
			{
				_stack.push(obj);
			}

			void PopElement()
			{
				return _stack.pop();
			}

			Json::Value& GetCurrentElement()
			{
				assert(!_stack.empty());
				return _stack.top();
			}

			const Json::Value& GetCurrentElement() const
			{
				assert(!_stack.empty());
				return _stack.top();
			}

			std::stack<Json::Value> _stack;
			std::string _currentPropertyName;
		};
	}
}