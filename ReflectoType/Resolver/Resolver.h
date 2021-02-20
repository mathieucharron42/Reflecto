#pragma once

#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorTypeFactory.h"

#include "Common/Definitions.h"
#include "Common/Utils/NonCopyable.h"

#include <assert.h>
#include <optional>

namespace Reflecto
{
	namespace Type
	{
		template<class object_t>
		class Resolver : private Utils::NonCopyable
		{
		public:
			Resolver(const TypeDescriptor& typeDescriptor)
				: _typeDescriptor(typeDescriptor)
			{
				assert(TypeExt::GetTypeHash<object_t>() == typeDescriptor.Type().Hash());
			}

			object_t* Instantiate()
			{
				const ConstructorDescriptor& constructorDescriptor = _typeDescriptor.Constructor();
				void* instance = (constructorDescriptor.Function())();
				return static_cast<object_t*>(instance);
			}

			template<typename member_t>
			member_t* ResolveMember(object_t& object, const std::string& memberName)
			{
				return static_cast<member_t*>(ResolveMember(object, memberName));
			}

			void* ResolveMember(object_t& object, const std::string& memberName)
			{
				const MemberDescriptor* memberDescriptor = _typeDescriptor.GetMemberByNameRecursive(memberName);
				return memberDescriptor ? ResolveMember(object, *memberDescriptor) : nullptr;
			}

			template<typename member_t>
			member_t* ResolveMember(object_t& object, const MemberDescriptor& memberDescriptor)
			{
				return static_cast<member_t*>(ResolveMember(object, memberDescriptor));
			}

			void* ResolveMember(object_t& object, const MemberDescriptor& memberDescriptor)
			{
				return ResolveMember(&object, memberDescriptor);
			}

			const void* ResolveMember(const object_t& object, const MemberDescriptor& memberDescriptor)
			{
				return ResolveMember(&object, memberDescriptor);
			}

			void* ResolveMember(void* object, const MemberDescriptor& memberDescriptor)
			{
				return const_cast<void*>(ResolveMember(const_cast<const void*>(object), memberDescriptor));
			}

			const void* ResolveMember(const void* object, const MemberDescriptor& memberDescriptor)
			{
				const byte* objRawAddr = reinterpret_cast<const byte*>(object);
				const byte* memberRawAddr = objRawAddr + memberDescriptor.Offset();
				return memberRawAddr;
			}

		private:
			TypeDescriptor _typeDescriptor;
		};
	}
}