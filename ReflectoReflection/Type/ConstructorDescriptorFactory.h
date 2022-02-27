#pragma once

#include "ConstructorDescriptor.h"

#include <memory>

namespace Reflecto
{
	namespace Reflection
	{
		template <typename object_t>
		class ConstructorDescriptorFactory
		{
		public:
			ConstructorDescriptorFactory()
			{
				_constructor = []() -> std::unique_ptr<object_t>
				{
					return std::make_unique<object_t>();
				};
			}

			ConstructorDescriptor Build()
			{
				return ConstructorDescriptor(_constructor);
			}

		private:
			ConstructorDescriptor::construction_func_t<object_t> _constructor;
		};
	}
}