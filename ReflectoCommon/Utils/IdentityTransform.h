#pragma once

#include <utility>

namespace Reflecto
{
	namespace Utils
	{
		struct IdentityTransform
		{
			template<class type>
			constexpr type&& operator()(type&& t) const noexcept
			{
				return std::forward<type>(t);
			}
		};
	}
}