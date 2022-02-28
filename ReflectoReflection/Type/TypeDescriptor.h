#pragma once

#include "ConstructorDescriptor.h"
#include "MemberDescriptor.h"
#include "MethodDescriptor.h"
#include "ValueDescriptor.h"
#include "TypeExt.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeDescriptor
		{
		public:
			TypeDescriptor(const Type& type, const ConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members, const std::vector<MethodDescriptor>& methods, const std::vector<ValueDescriptor>& values)
				: TypeDescriptor(type, nullptr, constructor, members, methods, values)
			{ }

			TypeDescriptor(const Type& type, const TypeDescriptor* parent, const ConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members, const std::vector<MethodDescriptor>& methods, const std::vector<ValueDescriptor>& values)
				: _type(type)
				, _parent(parent)
				, _constructor(constructor)
				, _members(members)
				, _methods(methods)
				, _values(values)
			{ }

			const Type& GetType() const
			{
				return _type;
			}

			const ConstructorDescriptor GetConstructor() const
			{
				return _constructor;
			}

			const std::vector<MemberDescriptor>& GetMembers() const
			{
				return _members;
			}

			std::vector<MemberDescriptor> FetchMemberResursive() const
			{
				std::vector<MemberDescriptor> memberRecursive;

				if (_parent)
				{
					std::vector<MemberDescriptor> parentMembers = _parent->FetchMemberResursive();
					memberRecursive.insert(memberRecursive.end(), parentMembers.begin(), parentMembers.end());
				}

				memberRecursive.insert(memberRecursive.end(), _members.begin(), _members.end());

				return memberRecursive;
			}

			const MemberDescriptor* GetMemberByName(const std::string& name) const
			{
				auto found = std::find_if(_members.begin(), _members.end(), [&](const MemberDescriptor& member) {
					return member.GetName() == name;
				});

				return found != _members.end() ? &(*found) : nullptr;
			}

			const MemberDescriptor* GetMemberByNameRecursive(const std::string& name) const
			{
				const MemberDescriptor* member = nullptr;

				if (_parent)
				{
					member = _parent->GetMemberByNameRecursive(name);
				}

				if (!member)
				{
					member = GetMemberByName(name);
				}

				return member;
			}

			const std::vector<MethodDescriptor>& GetMethods() const
			{
				return _methods;
			}

			const MethodDescriptor* GetMethodByName(const std::string& name) const
			{
				auto found = std::find_if(_methods.begin(), _methods.end(), [&](const MethodDescriptor& method) {
					return method.GetName() == name;
				});

				return found != _methods.end() ? &(*found) : nullptr;
			}

			const MethodDescriptor* GetMethodByNameRecursive(const std::string& name) const
			{
				const MethodDescriptor* method = nullptr;

				if (_parent)
				{
					method = _parent->GetMethodByNameRecursive(name);
				}

				if (!method)
				{
					method = GetMethodByName(name);
				}

				return method;
			}

			const std::vector<ValueDescriptor>& GetValues() const
			{
				return _values;
			}

			const ValueDescriptor* GetValueByName(const std::string& name) const
			{
				auto found = std::find_if(_values.begin(), _values.end(), [&](const ValueDescriptor& value) {
					return value.GetName() == name;
				});

				return found != _values.end() ? &(*found) : nullptr;
			}

			template<typename enum_t>
			const ValueDescriptor* GetValueByValue(const enum_t& enumValue) const
			{
				auto found = std::find_if(_values.begin(), _values.end(), [&](const ValueDescriptor& value) {
					return value.GetValue<enum_t>() == enumValue;
				});

				return found != _values.end() ? &(*found) : nullptr;
			}

		private:
			Type _type;
			const TypeDescriptor* _parent;
			ConstructorDescriptor _constructor;
			std::vector<MemberDescriptor> _members;
			std::vector<MethodDescriptor> _methods;
			std::vector<ValueDescriptor> _values;
		};
	}
}