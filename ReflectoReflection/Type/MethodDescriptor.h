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
		class TypeDescriptor;
		using TypeDescriptorPtr = std::shared_ptr<TypeDescriptor>;
		using TypeDescriptorUniquePtr = std::unique_ptr<TypeDescriptor>;

		class MethodDescriptor : RelationalOperators<MethodDescriptor>
		{
		public:
			template<typename object_t, typename ... args_t>
			using weak_method_ptr_t = std::function<std::any(object_t&, args_t...)>;

			template<typename return_t, typename object_t, typename ... args_t>
			using method_ptr_t = std::function<return_t(object_t&, args_t...)>;

			template<typename object_t, typename return_t = void, typename ... args_t>
			MethodDescriptor(const TypeDescriptorPtr& returnType, const std::string& name, const std::vector<ParameterDescriptor>& parameters, weak_method_ptr_t<object_t, args_t...> method)
				: _returnType(returnType)
				, _name(name)
				, _parameters(parameters)
				, _method(method)
			{

			}

			const std::string& GetName() const;

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
			
			const TypeDescriptor& GetReturnType() const;

			const std::vector<ParameterDescriptor>& GetParameters() const;

			bool operator<(const MethodDescriptor& other) const;

			std::string ToString() const;

		private:
			TypeDescriptorPtr _returnType;
			std::string _name;
			std::vector<ParameterDescriptor> _parameters;
			std::any _method;
		};
	}
}