#pragma once

#include "ConstructorDescriptor.h"
#include "MemberDescriptor.h"
#include "MethodDescriptor.h"
#include "TypeExt.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Reflecto
{
	namespace Type
	{
		class TypeDescriptor
		{
		public:
			TypeDescriptor(const TypeDescriptorType& type, const ConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members, const std::vector<MethodDescriptor>& methods)
				: TypeDescriptor(type, nullptr, constructor, members, methods)
			{ }

			TypeDescriptor(const TypeDescriptorType& type, const TypeDescriptor* parent, const ConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members, const std::vector<MethodDescriptor>& methods)
				: _type(type)
				, _parent(parent)
				, _constructor(constructor)
				, _members(members)
				, _methods(methods)
			{ }

			const TypeDescriptorType& GetType() const
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

		private:
			TypeDescriptorType _type;
			const TypeDescriptor* _parent;
			ConstructorDescriptor _constructor;
			std::vector<MemberDescriptor> _members;
			std::vector<MethodDescriptor> _methods;
		};
	}
}