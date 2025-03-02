#pragma once

#include <any>
#include <tuple>

namespace Reflecto
{
	namespace TypeExt
	{
		template <std::size_t index, typename... types_t>
		using nth_type = typename std::tuple_element<index, std::tuple<types_t...>>::type;
	}
}