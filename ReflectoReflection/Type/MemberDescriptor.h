#pragma once

#include "Type.h"
#include "TypeExt.h"

#include "Common/Definitions.h"
#include "Utils/RelationalOperators.h"

#include <stdint.h>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class MemberDescriptor : public RelationalOperators<MemberDescriptor>
		{
		public:
			MemberDescriptor(const Type& type, const std::string& name, byte offset)
				: _type(type)
				, _name(name)
				, _offset(offset)
			{ }

			const Type& GetType() const
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

			bool operator<(const MemberDescriptor& other) const
			{
				return _name < other._name
					&& _type < other._type;
			}

			std::string ToString() const
			{
				return Utils::StringExt::Format<std::string>("{Type=%s,Name=%s,Offset=%u", _type.ToString().c_str(), _name.c_str(), _offset);
			}

		private:
			Type _type;
			std::string _name;
			uint32_t _offset;
		};
	}
}