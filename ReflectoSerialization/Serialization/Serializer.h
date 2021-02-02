#pragma once

#include "Common/Definitions.h"
#include "Type/TypeDescriptor.h"

#include <cassert>
#include <map>
#include <memory>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		template<typename serialization_writer_t>
		class Serializer
		{
		public:
			using serialization_strategy_t = typename std::function<void(const Serializer<serialization_writer_t>& serializer, const Type::TypeDescriptor& descriptor, const void*, serialization_writer_t& writer)>;

			
			void Serialize(const Type::TypeDescriptorType& type, const void* value, serialization_writer_t& writer) const
			{
				const TypeInformation* typeInformation = GetTypeInformation(type);
				Serialize(typeInformation, value, writer);
			}

			template<typename value_t>
			void Serialize(const value_t& value, serialization_writer_t& writer) const
			{
				Type::TypeDescriptorType type = Type::TypeDescriptorTypeFactory<value_t>().Build();
				Serialize(type, &value, writer);
			}

			void RegisterType(const Type::TypeDescriptor& typeDescriptor, serialization_strategy_t strategy)
			{
				const TypeInformation information = { typeDescriptor, strategy };
				_typeInformations.insert({ typeDescriptor.Type(), information });
			}

			template<typename value_t>
			void RegisterType(serialization_strategy_t strategy)
			{
				Type::TypeDescriptor type = Type::TypeDescriptorTypeFactory<value_t>().Build();
				RegisterType(type, strategy);
			}


		private:
			struct TypeInformation
			{
				Type::TypeDescriptor Descriptor;
				serialization_strategy_t SerializationStrategy;
			};

			using TypeInformationMap = std::map<Type::TypeDescriptorType, TypeInformation>;

			void Serialize(const TypeInformation* typeInformation, const void* value, serialization_writer_t& writer) const
			{
				if (typeInformation)
				{
					const Type::TypeDescriptor& descriptor = typeInformation->Descriptor;
					const serialization_strategy_t serializationStrategy = typeInformation->SerializationStrategy;

					writer.WriteBeginObjectProperty("type");
					writer.WriteString(descriptor.Type().Name());
					writer.WriteEndObjectProperty();

					writer.WriteBeginObjectProperty("value");
					serializationStrategy(*this, descriptor, value, writer);
					writer.WriteEndObjectProperty();
				}
			}

			template<typename object_t>
			const TypeInformation* GetTypeInformation() const
			{
				Type::TypeDescriptorType type = Type::TypeDescriptorTypeFactory<object_t>().Build();
				return GetTypeInformation(type);
			}

			const TypeInformation* GetTypeInformation(const Type::TypeDescriptorType& type) const
			{
				TypeInformationMap::const_iterator found = _typeInformations.find(type);
				return found != _typeInformations.end() ? &(*found).second : nullptr;
			}

			TypeInformationMap _typeInformations;
		};
	}
}