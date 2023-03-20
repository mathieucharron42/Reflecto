#pragma once

#include "MemberDescriptor.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"
#include "TypeLibrary.h"

#include <assert.h>

namespace Reflecto
{
	namespace Reflection
	{
		template <typename object_t, typename member_t>
		class MemberDescriptorFactory
		{
		public:
			using member_pointer_t = typename member_t object_t::*;

			MemberDescriptorFactory(const TypeLibrary& typeLibrary, object_t& sampleObj, member_pointer_t memberPointer, const std::string& memberName)
				: _typeLibrary(typeLibrary)
				, _sampleObj(sampleObj)
				, _memberPointer(memberPointer)
				, _name(memberName)
			{ }

			MemberDescriptor Build()
			{
				byte offset = TypeExt::ComputeMemberOffset(_sampleObj, _memberPointer);
				const TypeDescriptorPtr type = _typeLibrary.GetDescriptor<member_t>();
				return MemberDescriptor{ type, _name, offset };
			}

		private:
			TypeLibrary _typeLibrary;
			object_t _sampleObj;
			member_pointer_t _memberPointer;
			std::string _name;
		};
	}
}