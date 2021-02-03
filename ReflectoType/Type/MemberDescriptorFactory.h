#pragma once

#include "MemberDescriptor.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorFactory.h"
#include "TypeLibrary.h"

#include <assert.h>

namespace Reflecto
{
	namespace Type
	{
		template <typename object_t, typename member_t>
		class MemberDescriptorFactory
		{
		public:
			MemberDescriptorFactory(const TypeLibrary& typeLibrary, object_t& sampleObj)
				: _typeLibrary(typeLibrary)
				, _sampleObj(sampleObj)
				, _name()
				, _offset(0)
			{ }

			MemberDescriptorFactory& SetMember(typename member_t object_t::* memberPointer, const std::string& memberName)
			{
				byte offset = TypeExt::ComputeOffset(_sampleObj, memberPointer);
				return SetMember(offset, memberName);
			}

			MemberDescriptorFactory& SetMember(byte offset, const std::string& memberName)
			{
				_name = memberName;
				_offset = offset;
				return *this;
			}

			MemberDescriptor Build()
			{
				const TypeDescriptorType* type = _typeLibrary.Get<member_t>();
				assert(type);
				return MemberDescriptor{ *type, _name, _offset };
			}

		private:
			TypeLibrary _typeLibrary;
			object_t _sampleObj;
			std::string _name;
			byte _offset;
		};
	}
}