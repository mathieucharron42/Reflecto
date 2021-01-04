#pragma once

#include "io.h"
#include "TypeExt.h"

#include <cassert>
#include <tuple>

template <typename SerializerType, typename ObjectType>
class SerializationWorkbench
{
public:
    template <typename ... MappingTypes>
    using MappingCollectionType = std::tuple<MappingTypes...>;

    void SetSerializer(const SerializerType& serializer)
    {
        _serializer = serializer;
    }

    void SetTraceFile(const std::string& traceFile, const std::string& extension)
    {
        _traceFile = traceFile + "." + extension;
    }

    template<typename ... MappingTypes>
    void Execute(const MappingCollectionType<MappingTypes...>& mappings)
    {
        {
            ObjectType original = BuildOriginal(mappings);
            std::vector<byte> bytes1;
            _serializer.Serialize(original, bytes1);
            io::WriteToFile(_traceFile, bytes1);
        }

        {
            std::vector<byte> bytes2 = io::ReadFromFile(_traceFile);
            ObjectType deserialized;
            _serializer.Deserialize<ObjectType>(bytes2, deserialized);
            Validate(mappings, deserialized);
        }

        std::cout << "Validated " << TypeExt::GetClassName<ObjectType>() << " with " << TypeExt::GetClassName<SerializerType>() << " to '" << _traceFile << "'" << std::endl;
    }

private:
    template<typename ... MappingTypes>
    ObjectType BuildOriginal(const MappingCollectionType<MappingTypes...>& mappings)
    {
        ObjectType obj;

        ExecuteMappings(mappings, [&obj](const auto& mappingPair) {
            auto memberPointer = mappingPair.first;
            auto value = mappingPair.second;
            obj.*memberPointer = value;
        });

        return obj;
    }

    template<typename ... MappingTypes>
    void Validate(const MappingCollectionType<MappingTypes...>& mappings, const ObjectType& obj)
    {
        ExecuteMappings(mappings, [&obj](const auto& mappingPair) {
            auto memberPointer = mappingPair.first;
            auto expectedValue = mappingPair.second;
            auto actualValue = obj.*memberPointer;
            if (expectedValue != actualValue)
            {
                std::cerr << "Field: " << typeid(memberPointer).name() << " Expected: " << expectedValue << " Actual: " << actualValue << std::endl;
                assert(false && "Serialization/deserialization inconsistency!");
            }
        });
    }

    template<typename ... MappingTypes, typename FuncType>
    void ExecuteMappings(const MappingCollectionType<MappingTypes...>& mappings, const FuncType& func)
    {
        auto iterationMappingFunc = [&func](const auto& ... mappings) {
            (func(mappings), ...);
        };
        std::apply(iterationMappingFunc, mappings);
    }

    std::string _traceFile = "tmp.bin";
    SerializerType _serializer;
};

template <class... BindingTypes>
auto BuildBindings(BindingTypes&&... bindings)
{
    return std::make_tuple(bindings...);
}

template<typename MemberPointerType, typename ValueType>
auto Bind(MemberPointerType&& memberPointer, ValueType&& value)
{
    return std::make_pair(memberPointer, value);
}