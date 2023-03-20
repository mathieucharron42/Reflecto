#pragma once

#include "TypeExt.h"

#include "Common/Ensure.h"
#include "Common/Definitions.h"
#include "Utils/RelationalOperators.h"
#include "Utils/StringExt.h"

#include <stdint.h>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class MemberDescriptor;
		using MemberDescriptorPtr = std::shared_ptr<MemberDescriptor>;
		using MemberDescriptorUniquePtr = std::unique_ptr<MemberDescriptor>;
		using MemberDescriptorWeakPtr = std::weak_ptr<MemberDescriptor>;

		class TypeDescriptor;
		using TypeDescriptorPtr = std::shared_ptr<TypeDescriptor>;
		using TypeDescriptorUniquePtr = std::unique_ptr<TypeDescriptor>;

		class MemberDescriptor : public RelationalOperators<MemberDescriptor>
		{
		public:
			MemberDescriptor(const TypeDescriptorPtr& type, const std::string& name, byte offset)
				: _type(type)
				, _name(name)
				, _offset(offset)
			{ }

			const TypeDescriptorPtr& GetType() const
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

			bool operator<(const MemberDescriptor& other) const;

			std::string ToString() const;

		private:
			TypeDescriptorPtr _type;
			std::string _name;
			uint32_t _offset;
		};
	}
}