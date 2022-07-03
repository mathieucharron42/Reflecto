#pragma once

#include "MethodDescriptor.h"
#include "TypeDescriptor.h"
#include "TypeLibrary.h"
#include "ParameterDescriptorFactory.h"

#include "Common/Ensure.h"
#include "Utils/AnyExt.h"

#include <assert.h>
#include <memory.h>

namespace Reflecto
{
	namespace Reflection
	{
		template <typename object_t, typename return_t, typename ... args_t>
		class MethodDescriptorFactory
		{
		public:
			using method_ptr_t = return_t (object_t::*)(args_t ...);

			MethodDescriptorFactory(const TypeLibrary& typeLibrary, object_t& sampleObj, method_ptr_t methodPointer, const std::string& name, const std::vector<std::string>& parameterNames)
				: _typeLibrary(typeLibrary)
				, _sampleObj(sampleObj)
				, _methodPointer(methodPointer)
				, _name(name)
				, _parametersName(parameterNames)
			{ }

			MethodDescriptor Build()
			{
				ensure(sizeof...(args_t) == _parametersName.size());
				const Type returnType = *_typeLibrary.Get<return_t>();
				const std::vector<ParameterDescriptor> parameters = BuildParameters();
				const MethodDescriptor::weak_method_ptr_t<object_t, args_t...> method_wrapper = BuildMethodWrapper();
				return MethodDescriptor{ returnType, _name, parameters, method_wrapper };
			}

		private:
			std::vector<ParameterDescriptor> BuildParameters()
			{
				return BuildParameters(std::make_index_sequence<sizeof...(args_t)>{});
			}

			template <size_t... i>
			std::vector<ParameterDescriptor> BuildParameters(std::index_sequence<i...>)
			{
				return { ParameterDescriptorFactory<args_t>(_typeLibrary, _parametersName[i]).Build()... };
			}

			MethodDescriptor::weak_method_ptr_t<object_t, args_t...> BuildMethodWrapper()
			{
				return [methodPointer = _methodPointer](object_t& obj, args_t ... args) -> std::any {
					return AnyExt::Invoke(methodPointer, obj, args...);
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