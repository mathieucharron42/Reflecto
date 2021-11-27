#pragma once

#include <any>
#include <functional>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class MethodDescriptor
		{
		public:
			template<typename object_t, typename ... args_t>
			using method_ptr_t = std::function<void(object_t&, args_t...)>;

			template<typename object_t, typename ... args_t>
			MethodDescriptor(method_ptr_t<object_t, args_t...> method, const std::string& name)
				: _method(method)
				, _name(name)
			{ }

			const std::string& GetName() const
			{
				return _name;
			}

			template<typename object_t, typename ... args_t>
			method_ptr_t<object_t, args_t...> GetMethod() const
			{
				const method_ptr_t<object_t, args_t...>* typedMethod = std::any_cast<method_ptr_t<object_t, args_t...>>(&_method);
				return typedMethod ? *typedMethod : method_ptr_t<object_t, args_t...>();
			}

			bool operator==(const MethodDescriptor& other) const
			{
				return /*_method == other._method &&*/ _name == other._name;
			}
		private:
			std::string _name;
			std::any _method;
		};
	}
}