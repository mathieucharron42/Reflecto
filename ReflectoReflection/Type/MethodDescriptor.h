#pragma once

#include "ParameterDescriptor.h"

#include "Utils/AnyExt.h"
#include "Utils/CollectionExt.h"

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
			template<typename object_t, typename ... args_t>
			using weak_method_ptr_t = std::function<std::any(object_t&, args_t...)>;

			template<typename return_t, typename object_t, typename ... args_t>
			using method_ptr_t = std::function<return_t(object_t&, args_t...)>;

			template<typename object_t, typename return_t = void, typename ... args_t>
			MethodDescriptor(const Type& returnType, const std::string& name, const std::vector<ParameterDescriptor>& parameters, weak_method_ptr_t<object_t, args_t...> method)
				: _returnType(returnType)
				, _name(name)
				, _parameters(parameters)
				, _method(method)
			{ }

			const std::string& GetName() const
			{
				return _name;
			}

			template<typename object_t, typename ... args_t>
			weak_method_ptr_t<object_t, args_t...> GetWeakMethod() const
			{
				const weak_method_ptr_t<object_t, args_t...>* weakMethod = std::any_cast<weak_method_ptr_t<object_t, args_t...>>(&_method);
				return weakMethod ? *weakMethod : weak_method_ptr_t<object_t, args_t...>();
			}

			template<typename return_t, typename object_t, typename ... args_t>
			method_ptr_t<return_t, object_t, args_t...> GetMethod() const
			{
				method_ptr_t<return_t, object_t, args_t...> method;
				const weak_method_ptr_t<object_t, args_t...> weakMethod = GetWeakMethod<object_t, args_t...>();
				if (weakMethod)
				{
					method = [=](object_t& obj, args_t ... args) -> return_t {
						const std::any returnValue = std::invoke(weakMethod, obj, args...);
						return AnyExt::AnyCast<return_t>(returnValue);
					};
				}
				return method;
			}
			
			const Type& GetReturnType() const
			{
				return _returnType;
			}

			const std::vector<ParameterDescriptor> GetParameters() const
			{
				return _parameters;
			}

			bool operator<(const MethodDescriptor& other) const
			{
				return std::tie(_name, _returnType, _parameters) < std::tie(other._name, other._returnType, other._parameters);
			}

			std::string ToString() const
			{
				return StringExt::Format<std::string>("{ReturnType=%s,Name=%s,Parameters=%s}", _returnType.ToString().c_str(), _name.c_str(), StringExt::Join<std::string>(_parameters, ",", &ParameterDescriptor::ToString).c_str());
			}

		private:
			Type _returnType;
			std::string _name;
			std::vector<ParameterDescriptor> _parameters;
			std::any _method;
		};
	}
}