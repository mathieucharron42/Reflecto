#pragma once

#include "ConstructorDescriptor.h"
#include "ConstructorDescriptorFactory.h"
#include "MemberDescriptor.h"
#include "MemberDescriptorFactory.h"
#include "MethodDescriptor.h"
#include "MethodDescriptorFactory.h"
#include "ValueDescriptor.h"
#include "ValueDescriptorFactory.h"
#include "TypeDescriptor.h"
#include "TypeExt.h"

#include "Type/TypeLibrary.h"

#include <memory>


namespace Reflecto
{
	namespace Reflection
	{
		using TypeDescriptorUniquePtr = std::unique_ptr<TypeDescriptor>;

		template <typename object_t>
		class TypeDescriptorFactory
		{
		public:
			TypeDescriptorFactory(const TypeLibrary& typeLibrary, const std::string& name)
				: TypeDescriptorFactory(typeLibrary, name, TypeDescriptorPtr())
			{

			}

			TypeDescriptorFactory(const TypeLibrary& typeLibrary, const std::string& name, const TypeDescriptorPtr& parent)
				: _typeLibrary(typeLibrary)
				, _name(name)
				, _typeInfo(TypeExt::GetTypeInfo<object_t>())
				, _hash(TypeExt::GetTypeHash<object_t>())
				, _sampleObj()
				, _parent(parent)
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

			template <typename object_t, typename return_t, typename ... args_t>
			TypeDescriptorFactory& RegisterMethod(return_t(object_t::* methodPointer)(args_t ...), const std::string& methodName)
			{
				std::vector<std::string> parameterNames;
				return RegisterMethod(methodPointer, methodName, parameterNames);
			}

			template <typename object_t, typename return_t, typename ... args_t>
			TypeDescriptorFactory& RegisterMethod(return_t(object_t::* methodPointer)(args_t ...), const std::string& methodName, const std::vector<std::string>& parameterNames)
			{
				MethodDescriptor method = MethodDescriptorFactory<object_t, return_t, args_t ...>(_typeLibrary, _sampleObj, methodPointer, methodName, parameterNames).Build();

				_methods.push_back(method);

				return *this;
			}

			template <typename enum_t>
			TypeDescriptorFactory& RegisterValue(const enum_t& enumValue, const std::string& valueName)
			{
				ValueDescriptor value = ValueDescriptorFactory<enum_t>(_typeLibrary, enumValue, valueName).Build();

				_values.push_back(value);

				return *this;
			}

			TypeDescriptorUniquePtr Build()
			{
				return std::make_unique<TypeDescriptor>(_name, _typeInfo, _hash, _parent, _constructor, _members, _methods, _values);
			}

		private:
			TypeLibrary _typeLibrary;
			object_t _sampleObj;

			std::string _name;
			const std::type_info& _typeInfo;
			typehash_t _hash;
			
			TypeDescriptorPtr _parent;
			ConstructorDescriptor _constructor;
			std::vector<MemberDescriptor> _members;
			std::vector<MethodDescriptor> _methods;
			std::vector<ValueDescriptor> _values;
		};
	}
}