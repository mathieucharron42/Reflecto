#pragma once

#include <cstdint>
#include <string>

class ISerializationWriter
{
public:
	virtual bool WriteInteger32(int32_t value) = 0;

	virtual bool WriteInteger64(int64_t value) = 0;

	virtual bool WriteFloat(float value) = 0;

	virtual bool WriteDouble(double value) = 0;

	virtual bool WriteString(const std::string& value) = 0;

	virtual bool WriteBoolean(bool value) = 0;

	virtual bool WriteNull() = 0;

	virtual bool WriteBeginObject() = 0;

	virtual bool WriteEndObject() = 0;

	virtual bool WriteBeginObjectProperty(const std::string& propertyName) = 0;
	
	virtual bool WriteEndObjectProperty() = 0;

	virtual bool WriteBeginArray() = 0;

	virtual bool WriteEndArray() = 0;

	virtual bool WriteBeginArrayElement() = 0;

	virtual bool WriteEndArrayElement() = 0;
};