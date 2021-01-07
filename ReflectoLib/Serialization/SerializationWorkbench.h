#pragma once

#include "io.h"
#include "TypeExt.h"

#include <cassert>
#include <tuple>

template <typename SerializerType, typename object_t>
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
            object_t original = BuildOriginal(mappings);
            std::vector<byte> bytes1;
            _serializer.Serialize(original, bytes1);
            io::WriteToFile(_traceFile, bytes1);
        }

        {
            std::vector<byte> bytes2 = io::ReadFromFile(_traceFile);
            object_t deserialized;
            _serializer.Deserialize<object_t>(bytes2, deserialized);
            Validate(mappings, deserialized);
        }

        std::cout << "Validated " << type::GetClassName<object_t>() << " with " << type::GetClassName<SerializerType>() << " to '" << _traceFile << "'" << std::endl;
    }

private:
    template<typename ... MappingTypes>
    object_t BuildOriginal(const MappingCollectionType<MappingTypes...>& mappings)
    {
        object_t obj;

        ExecuteMappings(mappings, [&obj](const auto& mappingPair) {
            auto memberPointer = mappingPair.first;
            auto value = mappingPair.second;
            obj.*memberPointer = value;
        });

        return obj;
    }

    template<typename ... MappingTypes>
    void Validate(const MappingCollectionType<MappingTypes...>& mappings, const object_t& obj)
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

template<typename member_t, typename ValueType>
auto Bind(member_t&& memberPointer, ValueType&& value)
{
    return std::make_pair(memberPointer, value);
}