#pragma once

#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorTypeFactory.h"

#include "Common/Definitions.h"
#include "Utils/NonCopyable.h"

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
			template<typename ... args_t>
			using resolved_method_t = std::function<void(args_t...)>;

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

			template<typename ... args_t>
			resolved_method_t<args_t...> ResolveMethod(const std::string& methodName, object_t& object)
			{
				const MethodDescriptor* methodDescriptor = _typeDescriptor.GetMethodByNameRecursive(methodName);
				return methodDescriptor ? ResolveMethod<args_t...>(*methodDescriptor, object) : resolved_method_t<args_t...>();
			}

			template<typename ... args_t>
			resolved_method_t<args_t...> ResolveMethod(const MethodDescriptor& methodDescriptor, object_t& object)
			{
				return ResolveMethod<args_t...>(methodDescriptor, &object);
			}

			template<typename ... args_t>
			resolved_method_t<args_t...> ResolveMethod(const MethodDescriptor& methodDescriptor, void* object)
			{
				MethodDescriptor::method_ptr_t<object_t, args_t...> method = methodDescriptor.Method<object_t, args_t...>();
				return [=] (args_t ... args) { 
					object_t& typedObject = *reinterpret_cast<object_t*>(object);
					return method(typedObject, args...);
				};
			}


			//const std::function<void()> ResolveMethod(const void* object, const std::string methodName)
			//{
			//	const MethodDescriptor* methodDescriptor = _typeDescriptor.GetMethodByNameRecursive(methodName);
			//	return methodDescriptor ? ResolveMethod(object, *methodDescriptor) : std::function<void()>();
			//}

			//const std::function<void()> ResolveMethod(const void* object, const MethodDescriptor& methodDescriptor)
			//{
			//	std::function<void()> methodWrapper = [voidMethodWrapper = methodDescriptor.Method()] {
			//		return voidMethodWrapper.Method(object);
			//	}
			//	return methodWrapper;
			//}

		private:
			TypeDescriptor _typeDescriptor;
		};
	}
}