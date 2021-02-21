#pragma once

#include "Serialization/Reader/ISerializationReader.h"

#include "Common/Definitions.h"
#include "jsoncpp/json.h"

#include <assert.h>
#include <optional>
#include <stack>
#include <sstream>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		class JsonSerializationReader : public ISerializationReader
		{
		public:
			JsonSerializationReader()
			{

			}

			virtual void ReadInteger32(int32_t& value) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::intValue);
				value = PopElement().asInt();
			}

			virtual void ReadInteger64(int64_t& value) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::intValue);
				value = PopElement().asInt64();
			}

			virtual void ReadFloat(float& value) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::realValue);
				value = PopElement().asFloat();
			}

			virtual void ReadDouble(double& value) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::realValue);
				value = PopElement().asDouble();
			}

			virtual void ReadString(std::string& value) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::stringValue);
				value = PopElement().asString();
			}

			virtual void ReadBoolean(bool& value) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::booleanValue);
				value = PopElement().asBool();
			}

			virtual void ReadNull(void* value) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::nullValue);
				PopElement();
				value = nullptr;
			}

			virtual void ReadBeginArray() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::arrayValue);
				PushArrayIndex(0);
			}

			virtual void ReadEndArray() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::arrayValue);
				PopArrayIndex();
				PopElement();
			}

			virtual bool HasArrayElementRemaining() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::arrayValue);
				const uint32_t index = GetCurrentArrayIndex();
				return index < GetCurrentElement().size();
			}

			virtual void ReadBeginArrayElement(uint32_t& index) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::arrayValue);
				index = GetCurrentArrayIndex();
				PushElement(GetCurrentElement()[index]);
			}

			virtual void ReadEndArrayElement() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::arrayValue);
				uint32_t& index = GetCurrentArrayIndex();
				++index;
			}

			virtual void ReadBeginObject() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::objectValue);
				PushObjectProperties(GetCurrentElement().getMemberNames());
			}

			virtual void ReadEndObject() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::objectValue);
				PopObjectProperties();
				PopElement();
			}

			virtual bool HasObjectPropertyRemaining() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::objectValue);
				JsonProperties& properties = GetCurrentObjectProperties();
				return !properties.empty();
			}

			virtual void ReadBeginObjectProperty(std::string& propertyName) override
			{
				assert(GetCurrentElement().type() == Json::ValueType::objectValue);
				JsonProperties& properties = GetCurrentObjectProperties();
				propertyName = properties.front();
				PushElement(GetCurrentElement()[propertyName]);
			}

			virtual void ReadEndObjectProperty() override
			{
				assert(GetCurrentElement().type() == Json::ValueType::objectValue);
				JsonProperties& properties = GetCurrentObjectProperties();
				properties.erase(properties.begin());
			}

			void Import(const std::string& str)
			{
				JsonElement element;

				const Json::CharReaderBuilder charReaderBuilder;
				JSONCPP_STRING error;
				std::stringstream ss(str);
				if (Json::parseFromStream(charReaderBuilder, ss, &element, &error))
				{
					PushElement(element);
				}
				else
				{
					assert(false);
				}
			}

		private:
			using JsonElement = Json::Value;
			using JsonProperties = Json::Value::Members;

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

			bool HasCurrentElement() const
			{
				return !_stack.empty();
			}

			void PushArrayIndex(uint32_t index)
			{
				_arrayIndexes.push(index);
			}

			uint32_t PopArrayIndex()
			{
				uint32_t index = GetCurrentArrayIndex();
				_arrayIndexes.pop();
				return index;
			}

			uint32_t& GetCurrentArrayIndex()
			{
				assert(HasCurrentArrayIndex());
				return _arrayIndexes.top();
			}

			const bool HasCurrentArrayIndex() const
			{
				return !_arrayIndexes.empty();
			}

			void PushObjectProperties(const JsonProperties& properties)
			{
				_objectProperties.push(properties);
			}

			JsonProperties PopObjectProperties()
			{
				JsonProperties properties = GetCurrentObjectProperties();
				_objectProperties.pop();
				return properties;
			}

			JsonProperties& GetCurrentObjectProperties()
			{
				assert(HasCurrentObjectProperties());
				return _objectProperties.top();
			}

			const bool HasCurrentObjectProperties() const
			{
				return !_objectProperties.empty();
			}
			std::stack<JsonElement> _stack;
			std::stack<uint32_t> _arrayIndexes;
			std::stack<JsonProperties> _objectProperties;
		};
	}
}