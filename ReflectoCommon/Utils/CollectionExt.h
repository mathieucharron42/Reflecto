#pragma once

#include "Identity.h"

#include <array>
#include <tuple>
#include <utility>

namespace Reflecto
{
	namespace Utils
	{
		namespace CollectionExt
		{
			/*template<
				typename element_t,
				std::size_t array_size,
				std::size_t... i
			>
				constexpr auto ArrayToTuple(const std::array<element_t, array_size>& array, std::index_sequence<i...>)
			{
				return std::make_tuple(array[i]...);
			}

			template<
				typename element_t,
				std::size_t array_size
			>
			constexpr auto ArrayToTuple(const std::array<element_t, array_size>& array)
			{
				return ArrayToTuple(array, std::make_index_sequence<array_size>{});
			}*/

			template<
				typename element_t,
				std::size_t array_size,
				typename projection_t = Reflecto::Utils::Identity,
				std::size_t... i
			>
			constexpr auto ArrayToTuple(const std::array<element_t, array_size>& array, projection_t projection, std::index_sequence<i...>)
			{
				return std::make_tuple(projection(array[i])...);
			}

			template<
				typename element_t,
				std::size_t array_size,
				typename projection_t = Reflecto::Utils::Identity
			>
			constexpr auto ArrayToTuple(const std::array<element_t, array_size>& array, projection_t projection = projection_t{})
			{
				return ArrayToTuple(array, projection, std::make_index_sequence<array_size>{});
			}
		}
	}
}