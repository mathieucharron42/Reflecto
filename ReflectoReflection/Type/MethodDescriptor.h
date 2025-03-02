#pragma once

#include "ParameterDescriptor.h"

#include "Utils/AnyExt.h"
#include "Utils/CollectionExt.h"

#include <any>
#include <array>
#include <functional>
#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		class TypeDescriptor;
		using TypeDescriptorPtr = std::shared_ptr<TypeDescriptor>;

		class MethodDescriptor : RelationalOperators<MethodDescriptor>
		{
		public:
			template <std::size_t nb_param_t>
			using weak_method_params_t = std::array<std::any, nb_param_t>;

			template<typename object_t, std::size_t nb_param_t>
			using weak_method_ptr_t = std::function<std::any(object_t*, const weak_method_params_t<nb_param_t>&)>;

			template<typename return_t, typename object_t, typename ... args_t>
			using method_ptr_t = std::function<return_t(object_t*, args_t...)>;

			template<typename return_t, typename ... args_t>
			using resolved_method_t = std::function<return_t(args_t...)>;

			template<std::size_t nb_param_t>
			using weak_resolved_method_t = std::function<std::any(const weak_method_params_t<nb_param_t>&)>;

			template<typename object_t, typename return_t = void, size_t nb_param_t>
			MethodDescriptor(const TypeDescriptorPtr& returnType, const std::string& name, const std::vector<ParameterDescriptor>& parameters, weak_method_ptr_t<object_t, nb_param_t> method)
				: _returnType(returnType)
				, _name(name)
				, _parameters(parameters)
				, _method(method)
			{

			}

			const std::string& GetName() const
			{
				return _name;
			}

			template<typename object_t, std::size_t nb_param_t>
			weak_method_ptr_t<object_t, nb_param_t> GetWeakMethod() const
			{
				const weak_method_ptr_t<object_t, nb_param_t>* weakMethod = std::any_cast<weak_method_ptr_t<object_t, nb_param_t>>(&_method);
				return weakMethod ? *weakMethod : weak_method_ptr_t<object_t, nb_param_t>();
			}

			template<typename return_t, typename object_t, typename ... args_t>
			method_ptr_t<return_t, object_t, args_t...> GetMethod() const
			{
				constexpr int32_t param_count = sizeof...(args_t);
				method_ptr_t<return_t, object_t, args_t...> method;
				const weak_method_ptr_t<object_t, param_count> weakMethod = GetWeakMethod<object_t, param_count>();
				if (weakMethod)
				{
					method = [=](object_t& obj, args_t ... args) -> return_t {
						weak_method_params_t<param_count> params = weak_method_params_t<param_count>(args...);
						const std::any returnValue = AnyExt::Invoke(weakMethod, obj, params);
						return AnyExt::AnyCast<return_t>(returnValue);
					};
				}
				return method;
			}
			
			const TypeDescriptorPtr& GetReturnType() const
			{
				return _returnType;
			}

			const std::vector<ParameterDescriptor>& GetParameters() const
			{
				return _parameters;
			}

			const size_t GetParameterCount() const
			{
				return _parameters.size();
			}

			template<typename object_t, typename return_t = void, typename ... args_t>
			resolved_method_t<return_t, args_t...> ResolveMethod(object_t& object) const
			{
				return ResolveMethod<object_t, return_t, args_t...>(&object);
			}

			template<typename object_t, typename return_t = void, typename ... args_t>
			resolved_method_t<return_t, args_t...> ResolveMethod(void* object) const
			{
				resolved_method_t<return_t, args_t...> resolvedMethod;
				MethodDescriptor::method_ptr_t<return_t, object_t, args_t...> method = GetMethod<return_t, object_t, args_t...>();
				if (method)
				{
					resolvedMethod = [=](args_t ... args) -> return_t {
						object_t& typedObject = *reinterpret_cast<object_t*>(object);
						return std::invoke(method, typedObject, args...);
					};
				}
				return resolvedMethod;
			}

			template<typename object_t, typename return_t = void, size_t nb_param_t = 0>
			weak_resolved_method_t<nb_param_t> WeakResolveMethod(object_t& object) const
			{
				return WeakResolveMethod<object_t, return_t, nb_param_t>(object);
			}

			template<typename object_t, size_t nb_param_t = 0>
			weak_resolved_method_t<nb_param_t> WeakResolveMethod(void* object) const
			{
				weak_resolved_method_t<nb_param_t> weakResolvedMethod;
				MethodDescriptor::weak_method_ptr_t<object_t, nb_param_t> weakMethod = GetWeakMethod<object_t, nb_param_t>();
				if (weakMethod)
				{
					weakResolvedMethod = [=](const weak_method_params_t<nb_param_t>& args) -> std::any {
						object_t* typedObject = reinterpret_cast<object_t*>(object);
						std::tuple<object_t*, weak_method_params_t<nb_param_t>> invocationArgs = std::make_tuple(typedObject, args);
						return AnyExt::Apply(weakMethod, invocationArgs);
					};
				}
				return weakResolvedMethod;
			}

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