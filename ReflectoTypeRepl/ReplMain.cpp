#include "Potato.h"

#include "Serialization/Reader/JsonSerializationReader.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerializerFactory.h"
#include "Serialization/Strategy/SerializationStrategy.h"
#include "Serialization/Writer/JsonSerializationWriter.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"
#include "Resolver/Resolver.h"
#include "Utils/StringExt.h"

#include <iostream>
#include <string>
#include <stdint.h>

using namespace Reflecto;

enum class InstructionResult
{
	Ok,
	UnsupportedType,
	InternalError,
	BadInstruction,
	Leave
};

InstructionResult ProcessMemberInstruction(const Serialization::Serializer& serializer, Reflection::TypeDescriptor typeDescriptor, Potato& instance, const std::string& memberName, const std::string& memberValue)
{
	InstructionResult result;

	Reflection::Resolver<Potato> resolver(typeDescriptor);
	const Reflection::MemberDescriptor* memberDescriptor = typeDescriptor.GetMemberByName(memberName);
	if (memberDescriptor)
	{
		void* member = resolver.ResolveMember(instance, memberName);
		
		Serialization::JsonSerializationReader reader;
		reader.Import(memberValue);

		InstructionResult result = InstructionResult::Ok;
		serializer.RawDeserialize(memberDescriptor->GetType(), member, reader);
	}
	else
	{
		result = InstructionResult::InternalError;
	}

	return result;
}

InstructionResult ProcessMethodInstruction(const Serialization::Serializer& serializer, Reflection::TypeDescriptor typeDescriptor, Potato& instance, const std::string& methodName)
{
	InstructionResult result;

	Reflection::Resolver<Potato> resolver(typeDescriptor);
	const Reflection::MethodDescriptor* methodDescriptor = typeDescriptor.GetMethodByName(methodName);
	if (methodDescriptor)
	{
		auto method = resolver.ResolveMethod(*methodDescriptor, instance);
		// to do: gérer les paramètres et fonction avec type de retour
		method();
		result = InstructionResult::Ok;
	}
	else
	{
		result = InstructionResult::InternalError;
	}

	return result;
}

InstructionResult ProcessInstruction(const Serialization::Serializer& serializer, Potato& instance, Reflection::TypeDescriptor typeDescriptor, const std::string& instruction)
{
	InstructionResult result;
	if (instruction.empty() || instruction == "q")
	{
		result = InstructionResult::Leave;
	}
	else
	{
		const std::string kMemberInstructionDelimiter = "=";
		const std::string kMethodInstructionParamBeginDelimiter = "(";
		const std::string kMethodInstructionParamEndDelimiter = "(";
		if (instruction.find(kMemberInstructionDelimiter) != std::string::npos)
		{
			std::vector<std::string> tokens = Utils::StringExt::Tokenize(instruction, kMemberInstructionDelimiter);
			if (tokens.size() == 2)
			{
				const std::string memberName = tokens[0];
				const std::string memberValue = tokens[1];

				result = ProcessMemberInstruction(serializer, typeDescriptor, instance, memberName, memberValue);
			}
			else
			{
				result = InstructionResult::BadInstruction;
			}
		}
		else if (instruction.find(kMethodInstructionParamBeginDelimiter) != std::string::npos && instruction.find(kMethodInstructionParamEndDelimiter) != std::string::npos)
		{
			const std::string methodName = instruction.substr(0, instruction.find(kMethodInstructionParamBeginDelimiter));
			result = ProcessMethodInstruction(serializer, typeDescriptor, instance, methodName);
		}
		else
		{
			result = InstructionResult::BadInstruction;
		}
	}
	return result;
}

template<typename stream_in_t, typename stream_out_t>
std::string PromptInstruction(stream_in_t& input, stream_out_t& ouput)
{
	ouput << "Entrer instruction: ";

	std::string response;
	std::getline(input, response);
	return response;
}

template<typename stream_out_t>
void WriteResult(InstructionResult result, stream_out_t& ouput)
{
	ouput << "Resultat: ";
	if (result == InstructionResult::Ok)
	{
		ouput << "Succes";
	}
	else if (result == InstructionResult::InternalError)
	{
		ouput << "Erreur interne";
	}
	else if (result == InstructionResult::BadInstruction)
	{
		ouput << "Instruction erronee";
	}
	else if (result == InstructionResult::Leave)
	{
		ouput << "Sortie";
	}
	else
	{
		ouput << "Inconnu";
	}
	ouput << std::endl;
}

template<typename stream_t>
void WriteValue(const Serialization::Serializer& serializer, const Reflection::Type& type, const void* value, stream_t& ouput)
{
	Serialization::JsonSerializationWriter writer;
	
	serializer.RawSerialize(type, value, writer);

	std::string str;
	writer.Transpose(str);

	ouput << str;
}

template<typename stream_t>
void WriteState(const Serialization::Serializer& serializer, Reflection::TypeDescriptor typeDescriptor, const Potato& instance, stream_t& ouput)
{
	Reflection::Resolver<Potato> resolver{ typeDescriptor };
	ouput << "Instance[" << typeDescriptor.GetType().GetName() << "]" << std::endl;
	ouput << "Methods:" << std::endl;
	for (const Reflection::MethodDescriptor& methodDescriptor : typeDescriptor.GetMethods())
	{
		const std::string& methodName = methodDescriptor.GetName();
		ouput << "  ";
		ouput << "void " << methodName << "()" << std::endl;
	}
	ouput << "Members:" << std::endl;
	for (const Reflection::MemberDescriptor& memberDescriptor : typeDescriptor.GetMembers())
	{
		const std::string& memberName = memberDescriptor.GetName();
		const std::string& memberType = memberDescriptor.GetType().GetName();
		const void* member = resolver.ResolveMember(instance, memberDescriptor);
		ouput << "  ";
		ouput << memberType << " " << memberName;
		ouput << " = ";
		WriteValue(serializer, memberDescriptor.GetType(), member, ouput);
		ouput << std::endl;
	}
}

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
		.RegisterMember(&Potato::Calory, "Calory")
		.RegisterMember(&Potato::IsBaked, "IsBaked")
		.RegisterMethod(&Potato::Bake, "Bake")
		.RegisterMethod(&Potato::AddToppings, "AddToppings")
	.Build();

	Serialization::Serializer serializer = Serialization::SerializerFactory(typeLibrary)
		.LearnType<int32_t, Serialization::Int32SerializationStrategy>()
		.LearnType<std::string, Serialization::StringSerializationStrategy>()
		.LearnType<float, Serialization::FloatSerializationStrategy>()
		.LearnType<bool, Serialization::BooleanSerializationStrategy>()
	.Build();

	Potato instance;
	instance.Type = "Russet";
	instance.Weight = 89.f;
	instance.Calory = 49;
	instance.IsBaked = false;

	auto& input = std::cin;
	auto& ouput = std::cout;

	bool leave;
	do
	{
		WriteState(serializer, potatoTypeDescriptor, instance, ouput);
		std::string instruction = PromptInstruction(input, ouput);
		InstructionResult result = ProcessInstruction(serializer, instance, potatoTypeDescriptor, instruction);
		WriteResult(result, ouput);
		leave = result == InstructionResult::Leave;
		ouput << std::endl;
	} while (!leave);
}