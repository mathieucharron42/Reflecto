#pragma once

#include <any>
#include <functional>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class MethodDescriptor : RelationalOperators<MethodDescriptor>
		{
		public:
			template<typename object_t, typename return_t = void, typename ... args_t>
			using method_ptr_t = std::function<return_t(object_t&, args_t...)>;

			template<typename object_t, typename return_t = void, typename ... args_t>
			MethodDescriptor(const Type& returnType, const std::string& name, method_ptr_t<object_t, return_t, args_t...> method)
				: _returnType(returnType)
				, _name(name)
				, _method(method)
			{ }

			const std::string& GetName() const
			{
				return _name;
			}

			template<typename object_t, typename return_t = void, typename ... args_t>
			method_ptr_t<object_t, return_t, args_t...> GetMethod() const
			{
				const method_ptr_t<object_t, return_t, args_t...>* typedMethod = std::any_cast<method_ptr_t<object_t, return_t, args_t...>>(&_method);
				return typedMethod ? *typedMethod : method_ptr_t<object_t, return_t, args_t...>();
			}

			const Type& GetReturnType() const
			{
				return _returnType;
			}

			bool operator<(const MethodDescriptor& other) const
			{
				return _name < other._name;
			}

			std::string ToString() const
			{
				return Utils::StringExt::Format<std::string>("{Name=%s}", _name.c_str());
			}

		private:
			Type _returnType;
			std::string _name;
			std::any _method;
		};
	}
}