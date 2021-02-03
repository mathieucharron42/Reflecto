#pragma once

#include "ConstructorDescriptor.h"
#include "ConstructorDescriptorFactory.h"
#include "MemberDescriptor.h"
#include "MemberDescriptorFactory.h"
#include "Resolver/Resolver.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorType.h"

#include "Type/TypeLibrary.h"

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
			TypeDescriptorFactory& Register(typename member_t typename object_t::* memberPointer, const std::string& memberName)
			{
				MemberDescriptor member = MemberDescriptorFactory<object_t, member_t>(_typeLibrary, _sampleObj)
					.SetMember(memberPointer, memberName)
					.Build();

				_members.push_back(member);

				return *this;
			}

			TypeDescriptor Build()
			{
				const TypeDescriptorType* type = _typeLibrary.Get<object_t>();
				assert(type);
				return TypeDescriptor(*type, _parent, _constructor, _members);
			}

		private:
			Type::TypeLibrary _typeLibrary;
			object_t _sampleObj;
			const TypeDescriptor* _parent;
			ConstructorDescriptor _constructor;
			std::vector<MemberDescriptor> _members;
		};
	}
}