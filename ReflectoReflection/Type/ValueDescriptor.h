#pragma once

#include "TypeExt.h"

#include "Common/Definitions.h"
#include "Utils/RelationalOperators.h"

#include <memory>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeDescriptor;
		using TypeDescriptorPtr = std::shared_ptr<TypeDescriptor>;

		class ValueDescriptor : public RelationalOperators<ValueDescriptor>
		{
		public:
			using underlying_vale_type = uint64_t;

			ValueDescriptor(const TypeDescriptorPtr& type, const std::string& name, underlying_vale_type value)
				: _type(type)
				, _name(name)
				, _underlyingValue(value)
			{

			}

			const TypeDescriptorPtr& GetType() const
			{
				return _type;
			}

			const std::string& GetName() const
			{
				return _name;
			}

			template <typename enum_t>
			const enum_t GetValue() const;

			const underlying_vale_type GetUnderlyingValue() const
			{
				return _underlyingValue;
			}

			bool operator<(const ValueDescriptor& other) const;

			std::string ToString() const;

		private:
			TypeDescriptorPtr _type;
			std::string _name;
			underlying_vale_type _underlyingValue;
		};

		template <typename enum_t>
		const enum_t Reflecto::Reflection::ValueDescriptor::GetValue() const
		{
			return static_cast<enum_t>(GetUnderlyingValue());
		}

	}
}