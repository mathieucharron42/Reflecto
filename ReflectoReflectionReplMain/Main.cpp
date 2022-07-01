#include "Potato.h"

#include "ReflectionRepl.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"

#include <iostream>
#include "Type/TypeDescriptorFactory.h"

using namespace Reflecto;

int main()
{
	const Reflection::TypeLibrary typeLibrary = Reflection::TypeLibraryFactory()
		.Add<std::string>("string")
		.Add<float>("float")
		.Add<int32_t>("int32")
		.Add<bool>("bool")
		.Add<Potato>("Potato")
	.Build();

	Reflection::TypeDescriptor potatoTypeDescriptor = Reflection::TypeDescriptorFactory<Potato>(typeLibrary)
		.RegisterMember(&Potato::Type, "Type")
		.RegisterMember(&Potato::Weight, "Weight")
		.RegisterMember(&Potato::Calories, "Calories")
		.RegisterMember(&Potato::Sodium, "Sodium")
		.RegisterMember(&Potato::IsBaked, "IsBaked")
		.RegisterMethod(&Potato::Bake, "Bake")
		.RegisterMethod(&Potato::AddButter, "AddButter")
		.RegisterMethod(&Potato::AddSalt, "AddSalt")
	.Build();

	Serialization::Serializer serializer = Serialization::SerializerFactory(typeLibrary)
		.LearnType<int32_t, Serialization::Int32SerializationStrategy>()
		.LearnType<std::string, Serialization::StringSerializationStrategy>()
		.LearnType<float, Serialization::FloatSerializationStrategy>()
		.LearnType<bool, Serialization::BooleanSerializationStrategy>()
	.Build();

	Potato instance = Potato::BuildRusset();

	auto& input = std::cin;
	auto& ouput = std::cout;

	Reflection::Repl<Potato> repl;
	repl.Run(instance, potatoTypeDescriptor, serializer, input, ouput);
}