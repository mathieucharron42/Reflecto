#pragma once

#include <type_traits>

namespace Reflecto
{
	namespace Utils
	{
		template <
			template<typename> typename trait_t, 
			typename ... args_t>
		struct AreTrait;

		template <
			template<typename> typename trait_t,
			typename first_t,
			typename ... args_t
		>
		struct AreTrait<trait_t, first_t, args_t...>
		{
			static const bool value = trait_t<first_t>::value && AreTrait<trait_t, args_t...>::value;
		};

		template <
			template<typename> typename trait_t
		>
		struct AreTrait<trait_t>
		{
			static const bool value = true;
		};

		template <
			template<typename> typename trait_t,
			typename ... args_t
		>
		constexpr bool AreTrait_V = AreTrait<trait_t, args_t...>::value;
	}
}