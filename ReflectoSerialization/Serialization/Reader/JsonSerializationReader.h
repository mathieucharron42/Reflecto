#pragma once

#include "Serialization/Reader/ISerializationReader.h"

#include "Common/Definitions.h"
#include "Common/Ensure.h"
#include "jsoncpp/json.h"

#include <cstdint>
#include <istream>
#include <functional>
#include <optional>
#include <stack>
#include <sstream>
#include <string>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		class JsonSerializationReader : public ISerializationReader
		{
		public:
			virtual bool ReadInteger32(int32_t& value) override
			{
				return ReadElementValue(Json::intValue, &Json::Value::asInt, value);
			}

			virtual bool ReadInteger64(int64_t& value) override
			{
				return ReadElementValue(Json::intValue, &Json::Value::asInt64, value);
			}

			virtual bool ReadFloat(float& value) override
			{
				return ReadElementValue(Json::realValue, &Json::Value::asFloat, value);
			}

			virtual bool ReadDouble(double& value) override
			{
				return ReadElementValue(Json::realValue, &Json::Value::asDouble, value);
			}

			virtual bool ReadString(std::string& value) override
			{
				return ReadElementValue(Json::stringValue, &Json::Value::asString, value);
			}

			virtual bool ReadBoolean(bool& value) override
			{
				return ReadElementValue(Json::booleanValue, &Json::Value::asBool, value);
			}

			virtual bool ReadNull(void* value) override
			{
				return ReadElementValue(Json::nullValue, [] (const JsonElement&) { return nullptr; }, value);
			}

			virtual bool ReadBeginArray() override
			{
				return ReadElement(Json::arrayValue, [&](const JsonElement& elem) {
					return PushCurrentArrayIndexes(elem.size());
				});
			}

			virtual bool ReadEndArray() override
			{
				return ReadElement(Json::arrayValue, [&](const JsonElement& elem) {
					return PopCurrentArrayIndexes() && PopElement();
				});
			}

			virtual bool HasArrayElementRemaining() override
			{
				return ReadElement(Json::arrayValue, [&](const JsonElement& elem) {
					std::stack<uint32_t>* indexes;
					return GetCurrentArrayIndexes(indexes) && !(*indexes).empty();
				});
			}

			virtual bool ReadBeginArrayElement(uint32_t& index) override
			{
				return ReadElement(Json::arrayValue, [&](const JsonElement& elem) {
					return GetCurrentArrayIndex(index) && index < elem.size() && PushElement(elem[index]);
				});
			}

			virtual bool ReadEndArrayElement() override
			{
				return ReadElement(Json::arrayValue, [&](const JsonElement& elem) {
					return PopCurrentArrayIndex();
				});
			}

			virtual bool ReadBeginObject() override
			{
				return ReadElement(Json::objectValue, [&](const JsonElement& elem) {
					return PushCurrentObjectProperties(elem.getMemberNames());
				});
			}

			virtual bool ReadEndObject() override
			{
				return ReadElement(Json::objectValue, [&](const JsonElement& elem) {
					return PopCurrentObjectProperties() && PopElement();
				});
			}

			virtual bool HasObjectPropertyRemaining() override
			{
				return ReadElement(Json::objectValue, [&](const JsonElement& elem) {
					std::stack<std::string>* properties;
					return GetCurrentObjectProperties(properties) && !(*properties).empty();
				});
			}

			virtual bool ReadBeginObjectProperty(std::string& propertyName) override
			{
				return ReadElement(Json::objectValue, [&](const JsonElement& elem) {
					return GetCurrentObjectProperty(propertyName) && elem.isMember(propertyName) && PushElement(elem[propertyName]);
				});
			}

			virtual bool ReadEndObjectProperty() override
			{
				return ReadElement(Json::objectValue, [&](const JsonElement& elem) {
					return PopCurrentObjectProperty();
				});
			}

			bool Import(std::istream& inputStream)
			{
				bool success = false;
				JsonElement element;

				const Json::CharReaderBuilder charReaderBuilder;
				JSONCPP_STRING error;
				
				if (Json::parseFromStream(charReaderBuilder, inputStream, &element, &error))
				{
					if (PushElement(element))
					{
						success = true;
					}
				}

				return success;
			}

			bool ImportFromFile(const std::string& fileName)
			{
				
			}

		private:
			using JsonElement = Json::Value;
			using JsonProperties = Json::Value::Members;

			template<typename member_function_ptr_type, typename type>
			bool ReadElementValue(Json::ValueType valueType, member_function_ptr_type memberFunctionPtr, type& value)
			{
				std::function<type(const JsonElement&)> func = memberFunctionPtr;
				return ReadElementValue(valueType, func, value);
			}

			template<typename type>
			bool ReadElementValue(Json::ValueType valueType, const std::function<type(const JsonElement&)>& func, type& value)
			{
				return ReadElement(valueType, [&](const JsonElement& element)
				{
					bool success = false;
					value = func(element);
					success = PopElement();
					return success;
				});
			}
			
			template<typename funct_t>
			bool ReadElement(Json::ValueType valueType, const funct_t& func)
			{
				bool success = false;
				JsonElement* currentElement;
				if (GetCurrentElement(currentElement) && currentElement->type() == valueType)
				{
					success = func(*currentElement);
				}
				return success;
			}

			bool PushElement(const JsonElement& element)
			{
				_stack.push(element);
				return true;
			}

			bool PopElement()
			{
				bool success = false;
				JsonElement* currentElement = nullptr;
				if (GetCurrentElement(currentElement))
				{	
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

			bool GetCurrentElementType(Json::ValueType& type)
			{
				bool success = false;
				JsonElement* currentElement = nullptr;
				if (GetCurrentElement(currentElement))
				{
					type = currentElement->type();
					success = true;
				}
				return success;
			}

			bool HasCurrentElement() const
			{
				return !_stack.empty();
			}

			bool PushCurrentArrayIndexes(uint32_t size)
			{
				std::stack<uint32_t> stack;
				for (uint32_t i = 0; i < size; ++i)
				{
					// Insert in reverse order
					stack.push(size - i - 1);
				}
				_arrayIndexes.push(stack);
				return true;
			}

			bool PopCurrentArrayIndexes()
			{
				bool success = false;
				if (!_arrayIndexes.empty())
				{
					_arrayIndexes.pop();
					success = true;
				}
				return success;
			}

			bool PopCurrentArrayIndex()
			{
				bool success = false;
				std::stack<uint32_t>* indexes;
				if (GetCurrentArrayIndexes(indexes))
				{
					(*indexes).pop();
					success = true;
				}
				return success;
			}

			bool GetCurrentArrayIndex(uint32_t& index)
			{
				bool success = false;
				std::stack<uint32_t>* indexes;
				if (GetCurrentArrayIndexes(indexes))
				{
					index = (*indexes).top();
					success = true;
				}
				return success;
			}

			bool GetCurrentArrayIndexes(std::stack<uint32_t>*& indexes)
			{
				bool success = false;
				if (!_arrayIndexes.empty())
				{
					indexes = &_arrayIndexes.top();
					success = true;
				}
				return success;
			}

			bool PushCurrentObjectProperties(const JsonProperties& properties)
			{
				std::stack<std::string> stack;
				for (uint32_t i = 0; i < properties.size(); ++i)
				{
					// Insert in reverse order
					stack.push(properties[properties.size() - i - 1]);
				}
				_objectProperties.push(stack);
				return true;
			}

			bool PopCurrentObjectProperties()
			{
				bool success = false;
				if (!_objectProperties.empty())
				{
					_objectProperties.pop();
					success = true;
				}
				return success;
			}

			bool PopCurrentObjectProperty()
			{
				bool success = false;
				std::stack<std::string>* properties;
				if (GetCurrentObjectProperties(properties))
				{
					(*properties).pop();
					success = true;
				}
				return success;
			}

			bool GetCurrentObjectProperty(std::string& property)
			{
				bool success = false;
				std::stack<std::string>* properties;
				if (GetCurrentObjectProperties(properties))
				{
					property = (*properties).top();
					success = true;
				}
				return success;
			}

			const bool GetCurrentObjectProperties(std::stack<std::string>*& properties)
			{
				bool success = false;
				if (!_objectProperties.empty())
				{
					properties = &_objectProperties.top();
					success = true;
				}
				return success;
			}

			std::stack<JsonElement> _stack;
			std::stack<std::stack<uint32_t>> _arrayIndexes;
			std::stack<std::stack<std::string>> _objectProperties;
		};
	}
}