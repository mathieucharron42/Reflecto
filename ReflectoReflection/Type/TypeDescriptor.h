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
		class TypeDescriptor;
		using TypeDescriptorPtr = std::shared_ptr<TypeDescriptor>;
		using TypeDescriptorUniquePtr = std::unique_ptr<TypeDescriptor>;

		using OptionalConstructorDescriptor = std::optional<ConstructorDescriptor>;

		class TypeDescriptor
		{
		public:
			TypeDescriptor(const std::string& name, const std::type_info& typeInfo, typehash_t hash)
				: TypeDescriptor(name, typeInfo, hash, {}, {}, {}, {})
			{

			}

			TypeDescriptor(const std::string& name, const std::type_info& typeInfo, typehash_t hash, const OptionalConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members, const std::vector<MethodDescriptor>& methods, const std::vector<ValueDescriptor>& values)
				: TypeDescriptor(name, typeInfo, hash, TypeDescriptorPtr(), constructor, members, methods, values)
			{ }

			TypeDescriptor(const std::string& name, const std::type_info& typeInfo, typehash_t hash, const TypeDescriptorPtr& parent, const OptionalConstructorDescriptor& constructor, const std::vector<MemberDescriptor>& members, const std::vector<MethodDescriptor>& methods, const std::vector<ValueDescriptor>& values)
				: _name(name)
				, _typeInfo(typeInfo)
				, _hash(hash)
				, _parent(parent)
				, _constructor(constructor)
				, _members(members)
				, _methods(methods)
				, _values(values)
			{ }

			const std::type_info& GetInfo() const
			{
				return _typeInfo;
			}

			const std::string GetName() const
			{
				return _name;
			}

			typehash_t GetHash() const
			{
				return _hash;
			}

			const OptionalConstructorDescriptor& GetConstructor() const
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

			bool operator<(const TypeDescriptor& other) const
			{
				return std::tie(_hash, _name) < std::tie(other._hash, other._name);
			}

			std::string ToString() const
			{
				return StringExt::Format<std::string>("Type{Name=%s}", _name.c_str());
			}

		private:
			std::string _name;
			const std::type_info& _typeInfo;
			typehash_t _hash;

			TypeDescriptorPtr _parent;
			OptionalConstructorDescriptor _constructor;
			std::vector<MemberDescriptor> _members;
			std::vector<MethodDescriptor> _methods;
			std::vector<ValueDescriptor> _values;
		};
	}
}