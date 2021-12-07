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

class Potato
{
public:
	void Bake()
	{
		IsBaked = true;
	}

	void AddToppings()
	{
		Calory += 100;
		Weight += 5;
	}

	std::string Type;
	float Weight;
	int32_t Calory;
	bool IsBaked;
};

enum class InstructionResult
{
	Ok,
	UnsupportedType,
	InternalError,
	BadInstruction,
	Leave
};

InstructionResult AssignValue(Reflection::TypeLibrary typeLibrary, const Reflection::Type& type, void* value, const std::string& memberValue)
{
	InstructionResult result;
	if (type == typeLibrary.GetChecked<int32_t>())
	{
		int32_t* int32Value = reinterpret_cast<int32_t*>(value);
		*int32Value = std::stoi(memberValue);
		result = InstructionResult::Ok;
	}
	else if (type == typeLibrary.GetChecked<float>())
	{
		float* floatValue = reinterpret_cast<float*>(value);
		*floatValue = std::stof(memberValue);
		result = InstructionResult::Ok;
	}
	else if (type == typeLibrary.GetChecked<bool>())
	{
		bool* boolValue = reinterpret_cast<bool*>(value);
		*boolValue = memberValue == "true" ? true : false;
		result = InstructionResult::Ok;
	}
	else if (type == typeLibrary.GetChecked<std::string>())
	{
		std::string* stringValue = reinterpret_cast<std::string*>(value);
		*stringValue = memberValue;
		result = InstructionResult::Ok;
	}
	else
	{
		result = InstructionResult::UnsupportedType;
	}
	return result;
}

InstructionResult ProcessMemberInstruction(Reflection::TypeLibrary typeLibrary, Reflection::TypeDescriptor typeDescriptor, Potato& instance, const std::string& memberName, const std::string& memberValue)
{
	InstructionResult result;

	Reflection::Resolver<Potato> resolver{ typeDescriptor };
	const Reflection::MemberDescriptor* memberDescriptor = typeDescriptor.GetMemberByName(memberName);
	if (memberDescriptor)
	{
		void* member = resolver.ResolveMember(instance, memberName);
		result = AssignValue(typeLibrary, memberDescriptor->GetType(), member, memberValue);
	}
	else
	{
		result = InstructionResult::InternalError;
	}

	return result;
}

template<typename method_t>
InstructionResult ExecuteMethod(Reflection::TypeLibrary typeLibrary, method_t method)
{
	InstructionResult result;
	// to do: gérer les paramètres et fonction avec type de retour
	method();
	result = InstructionResult::Ok;
	return result;
}

InstructionResult ProcessMethodInstruction(Reflection::TypeLibrary typeLibrary, Reflection::TypeDescriptor typeDescriptor, Potato& instance, const std::string& methodName)
{
	InstructionResult result;

	Reflection::Resolver<Potato> resolver(typeDescriptor);
	const Reflection::MethodDescriptor* methodDescriptor = typeDescriptor.GetMethodByName(methodName);
	if (methodDescriptor)
	{
		auto method = resolver.ResolveMethod(*methodDescriptor, instance);
		result = ExecuteMethod(typeLibrary, method);
	}
	else
	{
		result = InstructionResult::InternalError;
	}

	return result;
}

InstructionResult ProcessInstruction(Reflection::TypeLibrary typeLibrary, Potato& instance, Reflection::TypeDescriptor typeDescriptor, const std::string& instruction)
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

				result = ProcessMemberInstruction(typeLibrary, typeDescriptor, instance, memberName, memberValue);
			}
			else
			{
				result = InstructionResult::BadInstruction;
			}
		}
		else if (instruction.find(kMethodInstructionParamBeginDelimiter) != std::string::npos && instruction.find(kMethodInstructionParamEndDelimiter) != std::string::npos)
		{
			const std::string methodName = instruction.substr(0, instruction.find(kMethodInstructionParamBeginDelimiter));
			result = ProcessMethodInstruction(typeLibrary, typeDescriptor, instance, methodName);
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
void WriteValue(Reflection::TypeLibrary typeLibrary, const Reflection::Type& type, const void* value, stream_t& ouput)
{
	if (type == typeLibrary.GetChecked<int32_t>())
	{
		const int32_t* int32Value = reinterpret_cast<const int32_t*>(value);
		ouput << *int32Value;
	}
	else if (type == typeLibrary.GetChecked<float>())
	{
		const float* floatValue = reinterpret_cast<const float*>(value);
		ouput << *floatValue;
	}
	else if (type == typeLibrary.GetChecked<bool>())
	{
		const bool* boolValue = reinterpret_cast<const bool*>(value);
		ouput << *boolValue ? "true" : "false";
	}
	else if (type == typeLibrary.GetChecked<std::string>())
	{
		const std::string* stringValue = reinterpret_cast<const std::string*>(value);
		ouput << *stringValue;
	}
}

template<typename stream_t>
void WriteState(const Reflection::TypeLibrary& typeLibrary, Reflection::TypeDescriptor typeDescriptor, const Potato& instance, stream_t& ouput)
{
	Reflection::Resolver<Potato> resolver{ typeDescriptor };
	ouput << "Instance[" << typeDescriptor.GetType().GetName() << "]" << std::endl;
	ouput << "Methodes:" << std::endl;
	for (const Reflection::MethodDescriptor& methodDescriptor : typeDescriptor.GetMethods())
	{
		const std::string& methodName = methodDescriptor.GetName();
		ouput << "  ";
		ouput << "void " << methodName << "()" << std::endl;
	}
	ouput << "Donnees membres:" << std::endl;
	for (const Reflection::MemberDescriptor& memberDescriptor : typeDescriptor.GetMembers())
	{
		const std::string& memberName = memberDescriptor.GetName();
		const std::string& memberType = memberDescriptor.GetType().GetName();
		const void* member = resolver.ResolveMember(instance, memberDescriptor);
		ouput << "  ";
		ouput << memberType << " " << memberName;
		ouput << " = ";
		WriteValue(typeLibrary, memberDescriptor.GetType(), member, ouput);
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
		WriteState(typeLibrary, potatoTypeDescriptor, instance, ouput);
		std::string instruction = PromptInstruction(input, ouput);
		InstructionResult result = ProcessInstruction(typeLibrary, instance, potatoTypeDescriptor, instruction);
		WriteResult(result, ouput);
		leave = result == InstructionResult::Leave;
		ouput << std::endl;
	} while (!leave);
}