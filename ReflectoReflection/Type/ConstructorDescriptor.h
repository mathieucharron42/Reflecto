#pragma once

#include "Common/Definitions.h"
#include "Utils/AnyExt.h"

#include <any>
#include <functional>
#include <memory>

namespace Reflecto
{
	namespace Reflection
	{
		class ConstructorDescriptor
		{
		public:
			template<typename object_t>
			using construction_func_t = std::function<std::unique_ptr<object_t>()>;

			using weak_construction_func_t = std::function<std::unique_ptr<std::any>()>;

			ConstructorDescriptor(const weak_construction_func_t& constructor)
				: _constructor(constructor)
			{  }

			template<typename object_t>
			const construction_func_t<object_t> GetConstructorMethod() const
			{
				construction_func_t<object_t> constructorMethod;
				weak_construction_func_t weakConstructorMethod = GetWeakConstructorMethod();
				if (weakConstructorMethod)
				{
					constructorMethod = [weakConstructorMethod = weakConstructorMethod] {
						std::unique_ptr<std::any> anyValue = weakConstructorMethod();
						return object_t();
					};
				}
				return constructorMethod;
			}

			std::unique_ptr<std::any> NewWeakInstance() const
			{
				std::unique_ptr<std::any> newInstance;
				weak_construction_func_t weakConstructorFunc = GetWeakConstructorMethod();
				if (weakConstructorFunc)
				{
					newInstance = weakConstructorFunc();
				}
				return newInstance;
			}

			const weak_construction_func_t GetWeakConstructorMethod() const
			{
				weak_construction_func_t weakConstructorMethod = std::any_cast<weak_construction_func_t>(_constructor);
				return weakConstructorMethod;
			}

		private:
			std::any _constructor;
		};
	}
}