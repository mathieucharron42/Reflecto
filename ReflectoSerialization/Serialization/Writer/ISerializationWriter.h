#pragma once

#include <cstdint>
#include <string>

class ISerializationWriter
{
public:
	virtual void WriteInteger32(int32_t value) = 0;

	virtual void WriteInteger64(int64_t value) = 0;

	virtual void WriteFloat(float value) = 0;

	virtual void WriteDouble(double value) = 0;

	virtual void WriteString(const std::string& value) = 0;

	virtual void WriteBoolean(bool value) = 0;

	virtual void WriteNull() = 0;

	virtual void WriteBeginObject() = 0;

	virtual void WriteEndObject() = 0;

	virtual void WriteBeginObjectProperty(const std::string& propertyName) = 0;
	
	virtual void WriteEndObjectProperty() = 0;

	virtual void WriteBeginArray() = 0;

	virtual void WriteEndArray() = 0;

	virtual void WriteBeginArrayElement() = 0;

	virtual void WriteEndArrayElement() = 0;
};