#pragma once

#include "Type/TypeDescriptor.h"
#include "Type/TypeFactory.h"
#include "Type/TypeExt.h"

#include "Common/Definitions.h"
#include "Utils/NonCopyable.h"

#include <assert.h>
#include <memory>
#include <optional>

namespace Reflecto
{
	namespace Reflection
	{
		template<class object_t>
		class Resolver : private Utils::NonCopyable
		{
		public:
			template<typename ... args_t>
			using resolved_method_t = std::function<void(args_t...)>;

			Resolver(const TypeDescriptor& typeDescriptor)
				: _typeDescriptor(typeDescriptor)
			{
				assert(TypeExt::GetTypeHash<object_t>() == typeDescriptor.GetType().GetHash());
			}

			std::unique_ptr<object_t> Instantiate()
			{
				const ConstructorDescriptor& constructorDescriptor = _typeDescriptor.GetConstructor();
				auto& constructor = constructorDescriptor.GetConstructorMethod<object_t>();
				std::unique_ptr<object_t> instance = constructor();
				return instance;
			}

			template<typename member_t>
			member_t* ResolveMember(object_t& object, const std::string& memberName) const
			{
				member_t* memberPtr = nullptr;
				const MemberDescriptor* memberDescriptor = _typeDescriptor.GetMemberByNameRecursive(memberName);	
				if (memberDescriptor)
				{
					memberPtr = ResolveMember<member_t>(object, *memberDescriptor);
				}
				return memberPtr;
			}

			void* ResolveMember(object_t& object, const std::string& memberName) const
			{
				const MemberDescriptor* memberDescriptor = _typeDescriptor.GetMemberByNameRecursive(memberName);
				return memberDescriptor ? ResolveMember(object, *memberDescriptor) : nullptr;
			}

			template<typename member_t>
			member_t* ResolveMember(object_t& object, const MemberDescriptor& memberDescriptor) const
			{
				member_t* memberPtr = nullptr;
				if (memberDescriptor.GetType().GetHash() == TypeExt::GetTypeHash<member_t>())
				{
					memberPtr = static_cast<member_t*>(ResolveMember(object, memberDescriptor));
				}
				return memberPtr;
			}

			void* ResolveMember(object_t& object, const MemberDescriptor& memberDescriptor) const
			{
				return ResolveMember(&object, memberDescriptor);
			}

			const void* ResolveMember(const object_t& object, const MemberDescriptor& memberDescriptor) const
			{
				return ResolveMember(&object, memberDescriptor);
			}

			void* ResolveMember(void* object, const MemberDescriptor& memberDescriptor) const
			{
				return const_cast<void*>(ResolveMember(const_cast<const void*>(object), memberDescriptor));
			}

			const void* ResolveMember(const void* object, const MemberDescriptor& memberDescriptor) const
			{
				const byte* objRawAddr = reinterpret_cast<const byte*>(object);
				const byte* memberRawAddr = objRawAddr + memberDescriptor.GetOffset();
				return memberRawAddr;
			}

			template<typename ... args_t>
			resolved_method_t<args_t...> ResolveMethod(const std::string& methodName, object_t& object) const
			{
				const MethodDescriptor* methodDescriptor = _typeDescriptor.GetMethodByNameRecursive(methodName);
				return methodDescriptor ? ResolveMethod<args_t...>(*methodDescriptor, object) : resolved_method_t<args_t...>();
			}

			template<typename ... args_t>
			resolved_method_t<args_t...> ResolveMethod(const MethodDescriptor& methodDescriptor, object_t& object) const
			{
				return ResolveMethod<args_t...>(methodDescriptor, &object);
			}

			template<typename ... args_t>
			resolved_method_t<args_t...> ResolveMethod(const MethodDescriptor& methodDescriptor, void* object) const
			{
				resolved_method_t<args_t...> resolvedMethod;
				MethodDescriptor::method_ptr_t<object_t, args_t...> rawMethod = methodDescriptor.GetMethod<object_t, args_t...>();
				if (rawMethod)
				{
					resolvedMethod = [=](args_t ... args) {
						object_t& typedObject = *reinterpret_cast<object_t*>(object);
						return rawMethod(typedObject, args...);
					};
				}
				return resolvedMethod; 
			}

		private:
			TypeDescriptor _typeDescriptor;
		};
	}
}