#pragma once

#include "Common/Definitions.h"
#include "Serialization/Writer/ISerializationWriter.h"
#include "Serialization/SerializationMetaType.h"
#include "Serialization/Strategy/TypeSerializationStrategy.h"
#include "Serialization/Writer/SerializationWriterTypeTrait.h"
#include "Type/TypeDescriptor.h"

#include <cassert>
#include <map>
#include <memory>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		template<typename serialization_format_t>
		struct SerializationWriterTypeTrait { };

		template<>
		struct SerializationWriterTypeTrait<JsonSerializationFormat> { using type = JsonSerializationWriter; };

		template<typename serialization_format_t>
		struct SerializationValueTypeTrait { };

		template<>
		struct SerializationValueTypeTrait<JsonSerializationFormat> { using type = const char; };


		template<typename serialization_format_t>
		class Serializer
		{
		public:
			using serialization_writer_t = typename SerializationWriterTypeTrait<serialization_format_t>::type;
			using serialization_unit_t = typename SerializationValueTypeTrait<serialization_format_t>::type;
			using serialization_strategy_t = typename std::function<void(const void*, TypeMetaType&, std::string&)>;

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
					//writer.WriteObjectProperty("type", TypeMetaType::String, type);

					TypeMetaType metaType;
					std::string objSerialized;
					serializationStrategy(&obj, metaType, objSerialized);
					
					//writer.WriteObjectProperty("value", metaType, objSerialized);

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