#pragma once

#include "Definitions.h"
#include "Type/TypeDescriptor.h"
#include "Type/MemberDescriptor.h"

#include <cassert>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

class Serializer
{
public:
	using TypeDescriptorMap = std::map<std::size_t, TypeDescriptor>;

	template <typename object_t>
	void Serialize(const object_t& obj, std::vector<byte>& bytes)
	{
		const TypeDescriptor& descriptor = GetDescriptor<object_t>();

		//descriptor.Serialize(obj, bytes);

		/*const byte* objAddr = reinterpret_cast<const byte*>(&obj);

		for (const TypeDescriptorMember& member : descriptor.Members())
		{
			MemberType type = member.Type();
			const byte* memberAddr = objAddr + member.Offset();
			Serialize(type, memberAddr, bytes);
		}*/
	}

	template <typename object_t>
	void Deserialize(const std::vector<byte>& bytes, object_t& obj)
	{
		/*std::vector<byte> copy = bytes;

		const TypeDescriptor& descriptor = GetDescriptor<ObjectType>();
		byte* objAddr = reinterpret_cast<byte*>(&obj);

		for (const TypeDescriptorMember& member : descriptor.Members())
		{
			MemberType type = member.Type();
			byte* memberAddr = objAddr + member.Offset();
			uint32_t byteRead = Deserialize(copy, type, memberAddr);
			copy.erase(copy.begin(), copy.begin() + byteRead);
		}*/
	}

	/*void Register(TypeHashType typeHash, TypeDescriptor descriptor)
	{
		assert(typeHash == descriptor.TypeHash());
		_descriptorMap.insert(std::make_pair(typeHash, descriptor));
	}*/

private:
	/*void Serialize(MemberType type, const byte* memberAddr, std::vector<byte>& bytes)
	{
		switch (type)
		{
		case MemberType::Int32:
			SerializationStrategy::template Serialize<int32_t>(memberAddr, bytes);
			break;
		case MemberType::Int64:
			SerializationStrategy::template Serialize<int64_t>(memberAddr, bytes);
			break;
		case MemberType::UInt32:
			SerializationStrategy::template Serialize<uint32_t>(memberAddr, bytes);
			break;
		case MemberType::UInt64:
			SerializationStrategy::template Serialize<uint64_t>(memberAddr, bytes);
			break;
		case MemberType::Float:
			SerializationStrategy::template Serialize<float>(memberAddr, bytes);
			break;
		case MemberType::String:
			SerializationStrategy::template Serialize<std::string>(memberAddr, bytes);
			break;
		default:
			break;
		}
	}

	uint32_t Deserialize(const std::vector<byte>& bytes, MemberType type, byte* ptr)
	{
		switch (type)
		{
		case MemberType::Int32:
			return SerializationStrategy::template Deserialize<int32_t>(bytes, ptr);
			break;
		case MemberType::Int64:
			return SerializationStrategy::template Deserialize<int64_t>(bytes, ptr);
			break;
		case MemberType::UInt32:
			return SerializationStrategy::template Deserialize<uint32_t>(bytes, ptr);
			break;
		case MemberType::UInt64:
			return SerializationStrategy::template Deserialize<uint64_t>(bytes, ptr);
			break;
		case MemberType::Float:
			return SerializationStrategy::template Deserialize<float>(bytes, ptr);
			break;
		case MemberType::String:
			return SerializationStrategy::template Deserialize<std::string>(bytes, ptr);
			break;
		default:
			return -1;
			break;
		}
	}*/

	template <typename object_t>
	const TypeDescriptor& GetDescriptor() const
	{
		const std::size_t typeHash = type::template GetTypeHash<object_t>();
		const TypeDescriptorMap::const_iterator found = _descriptorMap.find(typeHash);
		assert(found != _descriptorMap.end());
		return (*found).second;
	}

	TypeDescriptorMap _descriptorMap;
};