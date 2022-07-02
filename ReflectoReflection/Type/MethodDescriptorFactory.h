#pragma once

#include "MethodDescriptor.h"
#include "TypeDescriptor.h"
#include "TypeLibrary.h"

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

			MethodDescriptorFactory(const TypeLibrary& typeLibrary, object_t& sampleObj, method_ptr_t methodPointer, const std::string& name)
				: _typeLibrary(typeLibrary)
				, _sampleObj(sampleObj)
				, _methodPointer(methodPointer)
				, _name(name)
			{ }

			MethodDescriptor Build()
			{
				const Type returnType = *_typeLibrary.Get<return_t>();
				const MethodDescriptor::method_ptr_t<object_t, return_t, args_t...> method_wrapper = [methodPointer = _methodPointer] (object_t& obj, args_t ... args) -> return_t {
					return std::invoke(methodPointer, obj, args...);
				};

				return MethodDescriptor{ returnType, _name, method_wrapper };
			}

		private:
			TypeLibrary _typeLibrary;
			object_t _sampleObj;
			method_ptr_t _methodPointer;
			std::string _name;
		};
	}
}