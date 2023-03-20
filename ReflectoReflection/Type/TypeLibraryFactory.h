#pragma once

#include "TypeDescriptorFactory.h"
#include "TypeLibrary.h"

#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>


namespace Reflecto
{
	namespace Reflection
	{
		class TypeLibraryFactory
		{
		public:
			template<class value_t>
			TypeLibraryFactory& Add(const std::string& typeName)
			{
				TypeDescriptorFactory<value_t> typeFactory = TypeDescriptorFactory<value_t>(Build(), typeName);
				Add(typeFactory.Build());
				
				return *this;
			}

			template<>
			TypeLibraryFactory& Add<void>(const std::string& typeName)
			{
				TypeDescriptorUniquePtr voidTypeDescriptor = std::make_unique<TypeDescriptor>("void", TypeExt::GetTypeInfo<void>(), TypeExt::GetTypeHash<void>());
				Add(std::move(voidTypeDescriptor));
				return *this;
			}

			template<class value_t>
			TypeLibraryFactory& BeginType(const std::string& typeName)
			{
				const TypeLibrary typeLibrary = Build();
				_currentTypeFactory = TypeDescriptorFactory<value_t>(typeLibrary, typeName);
				return *this;
			}

			template<class value_t, class parent_value_t>
			TypeLibraryFactory& BeginType(const std::string& typeName)
			{
				const TypeLibrary typeLibrary = Build();
				TypeDescriptorPtr parentTypeDescriptor = typeLibrary.GetDescriptor<parent_value_t>();
				if (ensure(parentTypeDescriptor))
				{
					_currentTypeFactory = TypeDescriptorFactory<value_t>(typeLibrary, typeName, parentTypeDescriptor);
				}
				return *this;
			}

			template <typename member_t, typename object_t>
			TypeLibraryFactory& RegisterMember(typename member_t typename object_t::* memberPointer, const std::string& memberName)
			{
				TypeDescriptorFactory<object_t>* typeFactory = Get<object_t>();
				if (ensure(typeFactory))
				{
					typeFactory->RegisterMember<member_t, object_t>(memberPointer, memberName);
				}
				return *this;
			}

			template <typename object_t, typename return_t, typename ... args_t>
			TypeLibraryFactory& RegisterMethod(return_t(object_t::* methodPointer)(args_t ...), const std::string& methodName)
			{
				TypeDescriptorFactory<object_t>* typeFactory = Get<object_t>();
				if (ensure(typeFactory))
				{
					typeFactory->RegisterMethod<object_t, return_t, args_t...>(methodPointer, methodName);
				}
				return *this;
			}


			template <typename object_t, typename return_t, typename ... args_t>
			TypeLibraryFactory& RegisterMethod(return_t(object_t::* methodPointer)(args_t ...), const std::string& methodName, const std::vector<std::string>& parameterNames)
			{
				TypeDescriptorFactory<object_t>* typeFactory = Get<object_t>();
				if (ensure(typeFactory))
				{
					typeFactory->RegisterMethod<object_t, return_t, args_t...>(methodPointer, methodName, parameterNames);
				}
				return *this;
			}

			template<class value_t>
			TypeLibraryFactory& EndType()
			{
				TypeDescriptorFactory<value_t>* typeFactory = Get<value_t>();
				if (ensure(typeFactory))
				{
					Add(typeFactory->Build());
				}
				_currentTypeFactory.reset();
				return *this;
			}

			TypeLibrary Build()
			{
				return TypeLibrary(_typeDescriptors);
			}

		private:
			
			template<typename value_t>
			TypeDescriptorFactory<value_t>* Get()
			{
				return std::any_cast<TypeDescriptorFactory<value_t>>(&_currentTypeFactory);
			}

			TypeLibraryFactory& Add(TypeDescriptorUniquePtr&& type)
			{
				TypeDescriptorPtr sharedType = std::move(type);
				if (std::find(_typeDescriptors.begin(), _typeDescriptors.end(), sharedType) == _typeDescriptors.end())
				{
					_typeDescriptors.push_back(sharedType);
				}
				return *this;
			}

			std::any _currentTypeFactory;
			std::vector<TypeDescriptorPtr> _typeDescriptors;
		};
	}
}