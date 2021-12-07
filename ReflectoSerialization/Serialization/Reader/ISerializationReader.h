#pragma once

#include <cstdint>
#include <string>

namespace Reflecto
{
	namespace Serialization
	{
		class ISerializationReader
		{
		public:
			virtual void ReadInteger32(int32_t& value) = 0;

			virtual void ReadInteger64(int64_t& value) = 0;

			virtual void ReadFloat(float& value) = 0;

			virtual void ReadDouble(double& value) = 0;

			virtual void ReadString(std::string& value) = 0;

			virtual void ReadBoolean(bool& value) = 0;

			virtual void ReadNull(void* value) = 0;

			virtual void ReadBeginObject() = 0;

			virtual void ReadEndObject() = 0;

			virtual bool HasObjectPropertyRemaining() = 0;

			virtual void ReadBeginObjectProperty(std::string& propertyName) = 0;

			virtual void ReadEndObjectProperty() = 0;

			virtual void ReadBeginArray() = 0;

			virtual void ReadEndArray() = 0;

			virtual bool HasArrayElementRemaining() = 0;
	
			virtual void ReadBeginArrayElement(uint32_t& index) = 0;

			virtual void ReadEndArrayElement() = 0;
		};
	}
}