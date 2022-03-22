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
			virtual bool ReadInteger32(int32_t& value) = 0;

			virtual bool ReadUnsignedInteger32(uint32_t& value) = 0;

			virtual bool ReadInteger64(int64_t& value) = 0;

			virtual bool ReadFloat(float& value) = 0;

			virtual bool ReadDouble(double& value) = 0;

			virtual bool ReadString(std::string& value) = 0;

			virtual bool ReadBoolean(bool& value) = 0;

			virtual bool ReadNull(void* value) = 0;

			virtual bool ReadBeginObject() = 0;

			virtual bool ReadEndObject() = 0;

			virtual bool HasObjectPropertyRemaining() = 0;

			virtual bool ReadBeginObjectProperty(std::string& propertyName) = 0;

			virtual bool ReadEndObjectProperty() = 0;

			virtual bool ReadBeginArray() = 0;

			virtual bool ReadEndArray() = 0;

			virtual bool HasArrayElementRemaining() = 0;
	
			virtual bool ReadBeginArrayElement(uint32_t& index) = 0;

			virtual bool ReadEndArrayElement() = 0;
		};
	}
}