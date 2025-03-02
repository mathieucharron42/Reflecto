#pragma once

#include "MethodDescriptor.h"
#include "TypeDescriptor.h"
#include "TypeLibrary.h"
#include "ParameterDescriptorFactory.h"

#include "Common/Ensure.h"
#include "Utils/AnyExt.h"
#include "Utils/FunctionalExt.h"

#include <assert.h>
#include <memory.h>

namespace Reflecto
{
	namespace Reflection
	{
		template <typename object_t, typename return_t, typename ... params_t>
		class MethodDescriptorFactory
		{
		public:
			using method_ptr_t = return_t (object_t::*)(params_t ...);
			static constexpr size_t param_count = sizeof...(params_t);

			MethodDescriptorFactory(const TypeLibrary& typeLibrary, object_t& sampleObj, method_ptr_t methodPointer, const std::string& name, const std::vector<std::string>& parameterNames)
				: _typeLibrary(typeLibrary)
				, _sampleObj(sampleObj)
				, _methodPointer(methodPointer)
				, _name(name)
				, _parametersName(parameterNames)
			{ }

			MethodDescriptor Build()
			{
				ensure(param_count == _parametersName.size());

				const TypeDescriptorPtr returnType = _typeLibrary.GetDescriptor<return_t>();
				const std::vector<ParameterDescriptor> parameters = BuildParameters(_typeLibrary, _parametersName);
				const MethodDescriptor::weak_method_ptr_t<object_t, param_count> method_wrapper = BuildMethodWrapper();

				return MethodDescriptor{ returnType, _name, parameters, method_wrapper };
			}

		private:
			static std::vector<ParameterDescriptor> BuildParameters(const TypeLibrary& typeLibrary, const std::vector<std::string>& parameterName)
			{
				return BuildParameters(typeLibrary, parameterName, std::make_index_sequence<sizeof...(params_t)>());
			}

			template <size_t... i>
			static std::vector<ParameterDescriptor> BuildParameters(const TypeLibrary& typeLibrary, const std::vector<std::string>& parametersName, std::index_sequence<i...>)
			{
				return { ParameterDescriptorFactory<params_t>(typeLibrary, parametersName[i]).Build()... };
			}

			static std::tuple<params_t...> CastParameters(const std::array<std::any, param_count>& params)
			{
				return CastParameters(params, std::make_index_sequence<param_count>());
			}

			template <size_t... i>
			static std::tuple<params_t...> CastParameters(const std::array<std::any, param_count>& params, std::index_sequence<i...>)
			{
				return std::make_tuple(AnyExt::AnyCast<params_t>(params[i])...);
			}

			MethodDescriptor::weak_method_ptr_t<object_t, param_count> BuildMethodWrapper()
			{
				return [methodPointer = _methodPointer](object_t* obj, const std::array<std::any, param_count>& params) -> std::any {
					const std::tuple<params_t...> typedParams = CastParameters(params);
					const std::tuple<object_t*, params_t...> instanceAndTypedParams = std::tuple_cat(std::make_tuple(obj), typedParams);
					return AnyExt::Apply(methodPointer, instanceAndTypedParams);
				};
			}

			TypeLibrary _typeLibrary;
			object_t _sampleObj;
			method_ptr_t _methodPointer;
			std::string _name;
			std::vector<std::string> _parametersName;
		};
	}
}