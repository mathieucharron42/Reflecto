#pragma once

#include "Serialization/Writer/ISerializationWriter.h"

#include "Common/Definitions.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeLibrary.h"

#include <cassert>
#include <map>
#include <memory>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		class Serializer
		{
		public:				

			using serialization_strategy_t = typename std::function<void(const Serializer& serializer, const Type::TypeDescriptor& descriptor, const void*, ISerializationWriter& writer)>;
			struct TypeInformation
			{
				Type::TypeDescriptor Descriptor;
				Serializer::serialization_strategy_t SerializationStrategy;
			};
			using TypeInformationMap = std::map<Type::TypeDescriptorType, Serializer::TypeInformation>;

			Serializer(const Type::TypeLibrary& library, const TypeInformationMap& typeInformation)
				: _typeLibrary(library)
				, _typeInformations(typeInformation)
			{ }


			template<typename serialization_writer_t>
			void Serialize(const Type::TypeDescriptorType& type, const void* value, serialization_writer_t& writer) const
			{
				const TypeInformation* typeInformation = GetTypeInformation(type);
				Serialize(typeInformation, value, writer);
			}

			template<typename value_t, typename serialization_writer_t>
			void Serialize(const value_t& value, serialization_writer_t& writer) const
			{
				const Type::TypeDescriptorType* type = _typeLibrary.Get<value_t>();
				assert(type); 
				Serialize(*type, &value, writer);
			}

		private:
			template<typename serialization_writer_t>
			void Serialize(const TypeInformation* typeInformation, const void* value, serialization_writer_t& writer) const
			{
				assert(typeInformation);
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

			const TypeInformation* GetTypeInformation(const Type::TypeDescriptorType& type) const
			{
				TypeInformationMap::const_iterator found = _typeInformations.find(type);
				return found != _typeInformations.end() ? &(*found).second : nullptr;
			}

			Type::TypeLibrary _typeLibrary;
			TypeInformationMap _typeInformations;
		};
	}
}