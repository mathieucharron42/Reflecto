#pragma once

#include <utility>

namespace Reflecto
{
	namespace Utils
	{
		struct Identity
		{
			template< class T>
			constexpr T&& operator()(T&& t) const noexcept
			{
				return std::forward<T>(t);
			}
		};
	}
}