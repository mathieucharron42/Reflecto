#pragma once

#include "Serialization/Reader/JsonSerializationReader.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerializerFactory.h"
#include "Serialization/Strategy/SerializationStrategy.h"
#include "Serialization/Writer/JsonSerializationWriter.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeLibrary.h"
#include "Resolver/Resolver.h"
#include "Utils/StringExt.h"

#include <string>

namespace Reflecto
{
	namespace Reflection
	{
		template <typename object_t>
		class Repl
		{
		public:
			template<typename stream_in_t, typename stream_out_t>
			void Run(object_t& instance, TypeDescriptor& typeDescriptor, const Serialization::Serializer& serializer, stream_in_t& input, stream_out_t& ouput)
			{
				bool leave;
				do
				{
					WriteState(serializer, typeDescriptor, instance, ouput);
					std::string instruction = PromptInstruction(input, ouput);
					InstructionResult result = ProcessInstruction(serializer, instance, typeDescriptor, instruction);
					WriteResult(result, ouput);
					leave = result == InstructionResult::Leave;
					ouput << std::endl;
				} while (!leave);
			}

		private:
			enum class InstructionResult
			{
				Ok,
				UnsupportedType,
				InternalError,
				BadInstruction,
				Leave
			};

			InstructionResult ProcessMemberInstruction(const Serialization::Serializer& serializer, Reflection::TypeDescriptor typeDescriptor, object_t& instance, const std::string& memberName, const std::string& memberValue)
			{
				InstructionResult result;

				Reflection::Resolver<object_t> resolver(typeDescriptor);
				const Reflection::MemberDescriptor* memberDescriptor = typeDescriptor.GetMemberByName(memberName);
				if (memberDescriptor)
				{
					void* member = resolver.ResolveMember(instance, memberName);

					Serialization::JsonSerializationReader reader;
					std::stringstream stream = std::stringstream(memberValue);
					reader.Import(stream);

					bool success = serializer.RawDeserialize(memberDescriptor->GetType(), member, reader);
					result = success ? InstructionResult::Ok : InstructionResult::UnsupportedType;
				}
				else
				{
					result = InstructionResult::InternalError;
				}

				return result;
			}

			InstructionResult ProcessMethodInstruction(const Serialization::Serializer& serializer, Reflection::TypeDescriptor typeDescriptor, object_t& instance, const std::string& methodName)
			{
				InstructionResult result;

				Reflection::Resolver<object_t> resolver(typeDescriptor);
				const Reflection::MethodDescriptor* methodDescriptor = typeDescriptor.GetMethodByName(methodName);
				if (methodDescriptor)
				{
					auto method = resolver.ResolveMethod<void>(*methodDescriptor, instance);
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

			InstructionResult ProcessInstruction(const Serialization::Serializer& serializer, object_t& instance, Reflection::TypeDescriptor typeDescriptor, const std::string& instruction)
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
				else if (result == InstructionResult::UnsupportedType)
				{
					ouput << "Type non supporte";
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

				bool success = serializer.RawSerialize(type, value, writer);
				if (!success)
				{
					ouput << "FAILED";
				}

				writer.Export(ouput);
			}

			template<typename stream_t>
			void WriteState(const Serialization::Serializer& serializer, Reflection::TypeDescriptor typeDescriptor, const object_t& instance, stream_t& ouput)
			{
				Reflection::Resolver<object_t> resolver{ typeDescriptor };
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
		};
	}
}