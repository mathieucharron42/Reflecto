#pragma once

#include "ConstructorDescriptor.h"
#include "ConstructorDescriptorFactory.h"
#include "MemberDescriptor.h"
#include "MemberDescriptorFactory.h"
#include "MethodDescriptor.h"
#include "MethodDescriptorFactory.h"
#include "Resolver/Resolver.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorType.h"

#include "Type/TypeLibrary.h"

#include <memory>

namespace Reflecto
{
	namespace Type
	{
		template <typename object_t>
		class TypeDescriptorFactory
		{
		public:
			TypeDescriptorFactory(const Type::TypeLibrary& typeLibrary)
				: TypeDescriptorFactory(typeLibrary, nullptr)
			{

			}

			TypeDescriptorFactory(const Type::TypeLibrary& typeLibrary, const TypeDescriptor* parentTypeDescriptor)
				: _typeLibrary(typeLibrary)
				, _sampleObj()
				, _parent(parentTypeDescriptor)
				, _constructor(ConstructorDescriptorFactory<object_t>().Build())
			{

			}

			template <typename member_t, typename object_t>
			TypeDescriptorFactory& RegisterMember(typename member_t typename object_t::* memberPointer, const std::string& memberName)
			{
				MemberDescriptor member = MemberDescriptorFactory<object_t, member_t>(_typeLibrary, _sampleObj, memberPointer, memberName).Build();

				_members.push_back(member);

				return *this;
			}

			template <typename object_t, typename ... args_t>
			TypeDescriptorFactory& RegisterMethod(void (object_t::* methodPointer)(args_t ...), const std::string& methodName)
			{
				MethodDescriptor method = MethodDescriptorFactory<object_t, args_t ...>(_typeLibrary, _sampleObj, methodPointer, methodName).Build();

				_methods.push_back(method);

				return *this;
			}

			TypeDescriptor Build()
			{
				const TypeDescriptorType& type = _typeLibrary.GetChecked<object_t>();
				return TypeDescriptor{type, _parent, _constructor, _members, _methods};
			}

		private:
			Type::TypeLibrary _typeLibrary;
			object_t _sampleObj;
			const TypeDescriptor* _parent;
			ConstructorDescriptor _constructor;
			std::vector<MemberDescriptor> _members;
			std::vector<MethodDescriptor> _methods;
		};
	}
}