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
				_constructor = []() -> std::unique_ptr<std::any>
				{
					return std::make_unique<std::any>(object_t());
				};
			}

			ConstructorDescriptor Build()
			{
				return ConstructorDescriptor(_constructor);
			}

		private:
			ConstructorDescriptor::weak_construction_func_t _constructor;
		};
	}
}