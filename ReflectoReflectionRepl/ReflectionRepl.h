#pragma once

#include "Serialization/Reader/JsonSerializationReader.h"
#include "Serialization/Serializer.h"
#include "Serialization/SerializerFactory.h"
#include "Serialization/Strategy/SerializationStrategy.h"
#include "Serialization/Writer/JsonSerializationWriter.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeLibrary.h"
#include "Utils/StringExt.h"
#include "jsoncpp/json.h"

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
			void Run(object_t& instance, const TypeLibrary& typeLibrary, const Serialization::Serializer& serializer, stream_in_t& input, stream_out_t& ouput)
			{
				bool leave;
				do
				{
					WriteState(serializer, typeLibrary, instance, ouput);
					std::string instruction = PromptInstruction(input, ouput);
					InstructionResult result = ProcessInstruction(serializer, instance, typeLibrary, instruction);
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

			InstructionResult ProcessMemberInstruction(const Serialization::Serializer& serializer, const Reflection::TypeDescriptorPtr& typeDescriptor, object_t& instance, const std::string& memberName, const std::string& memberValue)
			{
				InstructionResult result;

				const Reflection::MemberDescriptor* memberDescriptor = typeDescriptor->GetMemberByName(memberName);
				if (memberDescriptor)
				{
					void* member = memberDescriptor->ResolveMember(instance);
					if (member)
					{
						Serialization::JsonSerializationReader reader;
						std::stringstream stream = std::stringstream(memberValue);
						reader.Import(stream);

						if (serializer.RawDeserialize(memberDescriptor->GetType(), member, reader))
						{
							result = InstructionResult::Ok;
						}
						else
						{
							result = InstructionResult::UnsupportedType;
						}
					}
					else
					{
						result = InstructionResult::InternalError;
					}
				}
				else
				{
					result = InstructionResult::InternalError;
				}

				return result;
			}

			InstructionResult ProcessMethodInstruction(const Serialization::Serializer& serializer, const Reflection::TypeDescriptorPtr& typeDescriptor, object_t& instance, const std::string& methodName, const std::vector<std::string>& parameters)
			{
				InstructionResult result;

				const Reflection::MethodDescriptor* methodDescriptor = typeDescriptor->GetMethodByName(methodName);
				if (methodDescriptor)
				{
					Serialization::JsonSerializationReader reader;
					Json::Value element(Json::arrayValue);
					for (const std::string& parameter : parameters)
					{
						element.append(parameter);
					}
					reader.Import(element);

					MethodDescriptor::resolved_method_t<void> method = methodDescriptor->ResolveMethod<object_t>(&instance);
					method();
					result = InstructionResult::Ok;
				}
				else
				{
					result = InstructionResult::InternalError;
				}

				return result;
			}

			InstructionResult ProcessInstruction(const Serialization::Serializer& serializer, object_t& instance, const Reflection::TypeLibrary& typeLibrary, const std::string& instruction)
			{
				InstructionResult result;
				TypeDescriptorPtr typeDescriptor = typeLibrary.GetDescriptor<object_t>();
				if (!typeDescriptor)
				{
					result = InstructionResult::UnsupportedType;
				}
				else
				{
					if (instruction.empty() || instruction == "q")
					{
						result = InstructionResult::Leave;
					}
					else
					{
						const std::string kMemberInstructionDelimiter = "=";
						const std::string kMethodInstructionParamBeginDelimiter = "(";
						const std::string kMethodInstructionParamEndDelimiter = ")";

						const std::size_t memberInstructorDelimiterPos = instruction.find(kMemberInstructionDelimiter);
						const std::size_t methodInstructionParamBeginDelimiterPos = instruction.find(kMethodInstructionParamBeginDelimiter);
						const std::size_t methodInstructionParamEndDelimiterPos = instruction.find(kMethodInstructionParamEndDelimiter);
						if (memberInstructorDelimiterPos != std::string::npos)
						{
							std::vector<std::string> tokens = StringExt::Tokenize(instruction, kMemberInstructionDelimiter);
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
						else if (methodInstructionParamBeginDelimiterPos != std::string::npos && methodInstructionParamEndDelimiterPos != std::string::npos)
						{
							const std::string methodName = instruction.substr(0, methodInstructionParamBeginDelimiterPos);
							const std::vector<std::string> arguments = StringExt::Tokenize<std::string>(instruction.substr(methodInstructionParamBeginDelimiterPos + 1, methodInstructionParamEndDelimiterPos - methodInstructionParamBeginDelimiterPos - 1), ",");
							result = ProcessMethodInstruction(serializer, typeDescriptor, instance, methodName, arguments);
						}
						else
						{
							result = InstructionResult::BadInstruction;
						}
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
			void WriteValue(const Serialization::Serializer& serializer, const Reflection::TypeDescriptorPtr& type, const void* value, stream_t& ouput)
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
			void WriteState(const Serialization::Serializer& serializer, const Reflection::TypeLibrary& typeLibrary, const object_t& instance, stream_t& ouput)
			{
				Reflection::TypeDescriptorPtr typeDescriptor = typeLibrary.GetDescriptor<object_t>();
				if (typeDescriptor)
				{
					ouput << "Instance[" << typeDescriptor->GetName() << "]" << std::endl;
					ouput << "Methods:" << std::endl;
					for (const Reflection::MethodDescriptor& methodDescriptor : typeDescriptor->GetMethods())
					{
						const std::string& methodName = methodDescriptor.GetName();
						const std::string& returnType = methodDescriptor.GetReturnType()->GetName();
						const std::string& parameters = StringExt::Join<std::string>(methodDescriptor.GetParameters(), ", ", [](const ParameterDescriptor& descriptor) {
							return StringExt::Format<std::string>("%s %s", descriptor.GetType()->GetName().c_str(), descriptor.GetName().c_str());
							});
						ouput << "  ";
						ouput << returnType << " " << methodName << "(" << parameters << ")" << std::endl;
					}
					ouput << "Members:" << std::endl;
					for (const Reflection::MemberDescriptor& memberDescriptor : typeDescriptor->GetMembers())
					{
						const std::string& memberName = memberDescriptor.GetName();
						const std::string& memberType = memberDescriptor.GetType()->GetName();
						const void* member = memberDescriptor.ResolveMember(instance);
						ouput << "  ";
						ouput << memberType << " " << memberName;
						ouput << " = ";
						WriteValue(serializer, memberDescriptor.GetType(), member, ouput);
						ouput << std::endl;
					}
				}
			}
		};
	}
}