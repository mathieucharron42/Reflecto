#pragma once

#include "Common/Definitions.h"

#include <any>
#include <functional>
#include <memory>

namespace Reflecto
{
	namespace Reflection
	{
		class ConstructorDescriptor;
		using ConstructorDescriptorUniquePtr = std::unique_ptr<ConstructorDescriptor>();
		using ConstructorDescriptorWeakPtr = std::weak_ptr<ConstructorDescriptor>();
		
		class ConstructorDescriptor
		{
		public:
			template<typename object_t>
			using construction_func_t = std::function<std::unique_ptr<object_t>()>;

			template<typename object_t>
			ConstructorDescriptor(const construction_func_t<object_t>& constructor)
				: _constructor(constructor)
			{  }

			template<typename object_t>
			const construction_func_t<object_t> GetConstructorMethod() const
			{
				construction_func_t<object_t> typedConstructor = std::any_cast<construction_func_t<object_t>>(_constructor);
				return typedConstructor;
			}

		private:
			std::any _constructor;
		};
	}
}