#pragma once

#include "Common/Definitions.h"

#include <functional>

namespace Reflecto
{
	namespace Type
	{
		class ConstructorDescriptor
		{
		public:
			using construction_func_t = std::function<void*()>;

			ConstructorDescriptor(const construction_func_t& constructor)
				: _constructor(constructor)
			{

			}

			const construction_func_t& Function() const
			{
				return _constructor;
			}

		private:
			construction_func_t _constructor;
		};
	}
}