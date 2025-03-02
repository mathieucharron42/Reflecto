#pragma once

#include <any>
#include <tuple>

namespace Reflecto
{
	namespace AnyExt
	{
		template<typename type_t>
		std::enable_if_t<
			std::is_void_v<type_t>, 
			type_t> AnyCast(const std::any& any)
		{
			return;
		}

		template<typename type_t>
		std::enable_if_t<
			!std::is_void_v<type_t>,
			type_t> AnyCast(const std::any& any)
		{
			return std::any_cast<type_t>(any);
		}

		template<typename func_t, typename ... args_t>
		auto Invoke(func_t func, args_t && ... args) -> std::enable_if_t <
			std::is_void_v<decltype(std::invoke(func, args...))>,
			std::any>
		{
			std::invoke(func, args...);
			return std::any{};
		}

		template<typename func_t, typename ... args_t>
		auto Invoke(func_t func, args_t && ... args) -> std::enable_if_t <
			!std::is_void_v<decltype(std::invoke(func, args...))>,
			std::any>
		{
			return std::invoke(func, args...);
		}


		template<typename func_t, typename ... args_t>
		auto Apply(func_t func, const std::tuple<args_t...>& args) -> std::enable_if_t <
			std::is_void_v<decltype(std::apply(func, args))>,
			std::any>
		{
			std::apply(func, args);
			return std::any{};
		}

		template<typename func_t, typename ... args_t>
		auto Apply(func_t func, const std::tuple<args_t...>& args) -> std::enable_if_t <
			!std::is_void_v<decltype(std::apply(func, args))>,
			std::any>
		{
			return std::apply(func, args);
		}
	}
}