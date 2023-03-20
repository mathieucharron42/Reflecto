#pragma once

#include "Type/TypeDescriptor.h"
#include "Type/TypeExt.h"

#include "Common/Definitions.h"
#include "Utils/AnyExt.h"
#include "Utils/NonCopyable.h"

#include <assert.h>
#include <memory>
#include <optional>
#include "Common/Ensure.h"

namespace Reflecto
{
	namespace Reflection
	{
		template<class object_t>
		class Resolver : private NonCopyable
		{
		public:
			template<typename return_t, typename ... args_t>
			using resolved_method_t = std::function<return_t(args_t...)>;
			
			template<typename ... args_t>
			using weak_resolved_method_t = std::function<std::any(args_t...)>;


			Resolver(const TypeDescriptorPtr& typeDescriptor)
				: _typeDescriptor(typeDescriptor)
			{
				ensure(Is());
			}

			std::unique_ptr<object_t> Instantiate()
			{
				std::unique_ptr<object_t> instance;
				if (ensure(_typeDescriptor))
				{
					const OptionalConstructorDescriptor& constructorDescriptor = _typeDescriptor->GetConstructor();
					if (constructorDescriptor.has_value())
					{
						instance = constructorDescriptor->GetConstructorMethod<object_t>()();
					}
				}
				return instance;
			}

			bool Is()
			{
				return _typeDescriptor && TypeExt::GetTypeHash<object_t>() == _typeDescriptor->GetHash();
			}

			template<typename member_t>
			member_t* ResolveMember(object_t& object, const std::string& memberName) const
			{
				member_t* memberPtr = nullptr;
				if (ensure(_typeDescriptor))
				{
					const MemberDescriptor* memberDescriptor = _typeDescriptor->GetMemberByNameRecursive(memberName);
					if (memberDescriptor)
					{
						memberPtr = ResolveMember<member_t>(object, *memberDescriptor);
					}
				}
				return memberPtr;
			}

			void* ResolveMember(object_t& object, const std::string& memberName) const
			{
				void* memberPtr = nullptr;
				if (ensure(_typeDescriptor))
				{
					const MemberDescriptor* memberDescriptor = _typeDescriptor->GetMemberByNameRecursive(memberName);
					if (ensure(memberDescriptor))
					{
						memberPtr = ResolveMember(object, *memberDescriptor);
					}
				}
				return memberPtr;
			}

			template<typename member_t>
			member_t* ResolveMember(object_t& object, const MemberDescriptor& memberDescriptor) const
			{
				member_t* memberPtr = nullptr;
				const TypeDescriptorPtr& memberType = memberDescriptor.GetType();
				if (memberType && memberType->GetHash() == TypeExt::GetTypeHash<member_t>())
				{
					memberPtr = static_cast<member_t*>(ResolveMember(object, memberDescriptor));
				}
				return memberPtr;
			}

			void* ResolveMember(object_t& object, const MemberDescriptor& memberDescriptor) const
			{
				return const_cast<void*>(ResolveMember(const_cast<const object_t&>(object), memberDescriptor));
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

			template<typename return_t = void, typename ... args_t>
			resolved_method_t<return_t, args_t...> ResolveMethod(const std::string& methodName, object_t& object) const
			{
				resolved_method_t<return_t, args_t...> method;
				if (ensure(_typeDescriptor))
				{
					const MethodDescriptor* methodDescriptor = _typeDescriptor->GetMethodByNameRecursive(methodName);
					if (methodDescriptor)
					{
						method = ResolveMethod<return_t, args_t...>(*methodDescriptor, object);
					}
				}
				return method;
			}

			template<typename return_t = void, typename ... args_t>
			resolved_method_t<return_t, args_t...> ResolveMethod(const MethodDescriptor& methodDescriptor, object_t& object) const
			{
				return ResolveMethod<return_t, args_t...>(methodDescriptor, &object);
			}

			template<typename return_t = void, typename ... args_t>
			resolved_method_t<return_t, args_t...> ResolveMethod(const MethodDescriptor& methodDescriptor, void* object) const
			{
				resolved_method_t<return_t, args_t...> resolvedMethod;
				MethodDescriptor::method_ptr_t<return_t, object_t, args_t...> method = methodDescriptor.GetMethod<return_t, object_t, args_t...>();
				if (method)
				{
					resolvedMethod = [=](args_t ... args) -> return_t {
						object_t& typedObject = *reinterpret_cast<object_t*>(object);
						return std::invoke(method, typedObject, args...);
					};
				}
				return resolvedMethod; 
			}

			template<typename return_t = void, typename ... args_t>
			weak_resolved_method_t<return_t, args_t...> WeakResolveMethod(const std::string& methodName, object_t& object) const
			{
				weak_resolved_method_t<return_t, args_t...> method;
				if (ensure(_typeDescriptor))
				{
					const MethodDescriptor* methodDescriptor = _typeDescriptor->GetMethodByNameRecursive(methodName);
					method = WeakResolveMethod<return_t, args_t...>(*methodDescriptor, object);
				}
				return method;
			}

			template<typename return_t = void, typename ... args_t>
			weak_resolved_method_t<return_t, args_t...> WeakResolveMethod(const MethodDescriptor& methodDescriptor, object_t& object) const
			{
				return WeakResolveMethod<return_t, args_t...>(methodDescriptor, &object);
			}

			template<typename ... args_t>
			weak_resolved_method_t<args_t...> WeakResolveMethod(const MethodDescriptor& methodDescriptor, void* object) const
			{
				weak_resolved_method_t<args_t...> weakResolvedMethod;
				MethodDescriptor::weak_method_ptr_t<object_t, args_t...> weakMethod = methodDescriptor.GetWeakMethod<object_t, args_t...>();
				if (weakMethod)
				{
					weakResolvedMethod = [=](args_t ... args) -> std::any {
						object_t& typedObject = *reinterpret_cast<object_t*>(object);
						return std::invoke(weakMethod, typedObject, args...);
					};
				}
				return weakResolvedMethod;
			}

		private:
			TypeDescriptorPtr _typeDescriptor;
		};
	}
}