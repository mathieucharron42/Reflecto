#pragma once

#include "TypeExt.h"

#include "Common/Ensure.h"
#include "Common/Definitions.h"
#include "Utils/RelationalOperators.h"
#include "Utils/StringExt.h"

#include <stdint.h>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeDescriptor;
		using TypeDescriptorPtr = std::shared_ptr<TypeDescriptor>;

		class MemberDescriptor : public RelationalOperators<MemberDescriptor>
		{
		public:
			MemberDescriptor(const TypeDescriptorPtr& type, const std::string& name, byte offset)
				: _type(type)
				, _name(name)
				, _offset(offset)
			{ }

			const TypeDescriptorPtr& GetType() const
			{
				return _type;
			}

			const std::string& GetName() const
			{
				return _name;
			}

			const uint32_t GetOffset() const
			{
				return _offset;
			}

			template<typename object_t, typename member_t>
			member_t* ResolveMember(object_t& object) const
			{
				return const_cast<member_t*>(ResolveMember<object_t, member_t>(const_cast<const object_t&>(object)));
			}

			template<typename object_t, typename member_t>
			const member_t* ResolveMember(const object_t& object) const
			{
				void* memberPtr = ResolveMember<object_t>(object);
				const member_t* typedMemberPtr = static_cast<const member_t*>(memberPtr);
				return typedMemberPtr;
			}

			template<typename object_t>
			void* ResolveMember(object_t& object) const
			{
				return const_cast<void*>(ResolveMember<object_t>(const_cast<const object_t&>(object)));
			}

			template<typename object_t>
			const void* ResolveMember(const object_t& object) const
			{
				const void* objectPtr = &object;
				return ResolveMember(objectPtr);
			}

			void* ResolveMember(void* object) const
			{
				return const_cast<void*>(ResolveMember(const_cast<const void*>(object)));
			}

			const void* ResolveMember(const void* object) const
			{
				const byte* objRawAddr = reinterpret_cast<const byte*>(object);
				const byte* memberRawAddr = objRawAddr + GetOffset();
				return memberRawAddr;
			}

			bool operator<(const MemberDescriptor& other) const;

			std::string ToString() const;

		private:
			TypeDescriptorPtr _type;
			std::string _name;
			uint32_t _offset;
		};
	}
}