#pragma once

#include "IdentityTransform.h"

#include <array>
#include <tuple>
#include <utility>
#include <vector>

namespace Reflecto
{
	namespace CollectionExt
	{
		template<
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
		}

		template<
			typename element_t,
			std::size_t array_size,
			typename projection_t,
			std::size_t... i
		>
			constexpr auto ArrayToTupleProjection(const std::array<element_t, array_size>& array, projection_t projection, std::index_sequence<i...>)
		{
			return std::make_tuple(projection(array[i])...);
		}

		template<
			typename element_t,
			std::size_t array_size,
			typename projection_t = IdentityTransform
		>
			constexpr auto ArrayToTupleProjection(const std::array<element_t, array_size>& array, projection_t projection)
		{
			return ArrayToTupleProjection(array, projection, std::make_index_sequence<array_size>{});
		}

		template<
			typename element_t,
			std::size_t array_size,
			typename ... projections_t,
			std::size_t... i
		>
		constexpr auto ArrayToTupleProjections(const std::array<element_t, array_size>& array, const std::tuple<projections_t...> projections, std::index_sequence<i...>)
		{
			return std::make_tuple(std::get<i>(projections)(array[i])...);
		}

		template<
			typename element_t,
			std::size_t array_size,
			typename ... projections_t
		>
		constexpr auto ArrayToTupleProjections(const std::array<element_t, array_size>& array, const std::tuple<projections_t...> projections)
		{
			return ArrayToTupleProjections(array, projections, std::make_index_sequence<array_size>{});
		}

		template<typename element_t>
		std::vector<element_t> Concatenate()
		{
			return std::vector<element_t>();
		}

		template<
			typename element_t,
			typename... rest_t
		>
		std::vector<element_t> Concatenate(const std::vector<element_t>& first, const rest_t& ... rest)
		{
			std::vector<element_t> result;
				
			std::vector<element_t> remains = Concatenate<element_t>(rest...);

			result.reserve(first.size() + remains.size());
			result.insert(result.end(), first.begin(), first.end());
			result.insert(result.end(), remains.begin(), remains.end());

			return result;
		}
	}
}