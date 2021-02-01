#pragma once

#include "Common/Definitions.h"
#include "Type/TypeDescriptor.h"
#include "Writer/JsonSerializationWriter.h"

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
			using serialization_strategy_t = typename std::function<void(const void*, JsonSerializationWriter& writer)>;

			template<typename object_t>
			void Serialize(const object_t& obj, std::vector<byte>& bytes)
			{
				serialization_writer_t writer;

				const TypeInformation* typeInformation = GetTypeInformation<object_t>();

				if (typeInformation)
				{
					const Type::TypeDescriptor& descriptor = typeInformation->Descriptor;
					const serialization_strategy_t serializationStrategy = typeInformation->SerializationStrategy;

					writer.WriteBeginObject();
					
					std::string type = descriptor.Type().Name();
					writer.WriteObjectPropertyString("type", type);

					serialization_writer_t valueWriter;
					serializationStrategy(&obj, valueWriter);
					writer.WriteObjectPropertyWriter("value", objSerialized);

					writer.WriteEndObject();
				}

				writer.Transpose(bytes);
			}

			void RegisterType(const Type::TypeDescriptor& typeDescriptor, serialization_strategy_t strategy)
			{
				const TypeInformation information = { typeDescriptor, strategy };
				_typeInformations.insert({ typeDescriptor.Type(), information });
			}

		private:
			struct TypeInformation
			{
				Type::TypeDescriptor Descriptor;
				serialization_strategy_t SerializationStrategy;
			};

			using TypeInformationMap = std::map<Type::TypeDescriptorType, TypeInformation>;

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