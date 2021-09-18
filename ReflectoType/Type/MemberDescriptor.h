#pragma once

#include "TypeDescriptorType.h"
#include "TypeExt.h"

#include "Common/Definitions.h"

#include <stdint.h>
#include <string>

namespace Reflecto
{
	namespace Type
	{
		class MemberDescriptor
		{
		public:
			MemberDescriptor(const TypeDescriptorType& type, const std::string& name, byte offset)
				: _type(type)
				, _name(name)
				, _offset(offset)
			{ }

			const TypeDescriptorType& GetType() const
			{
				return _type;
			}

			const std::string& GetName() const
			{
				return _name;
			}

			const uint32_t GetOffset() const
			{
				return _offset;
			}

			bool operator==(const MemberDescriptor& other) const
			{
				return _type == other._type
					&& _name == other._name
					&& _offset == other._offset;
			}

		private:
			TypeDescriptorType _type;
			std::string _name;
			uint32_t _offset;
		};
	}
}