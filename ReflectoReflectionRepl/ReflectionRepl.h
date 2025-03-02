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
			void Run(const TypeLibrary& typeLibrary, const Serialization::Serializer& serializer, object_t& instance, stream_in_t& input, stream_out_t& ouput)
			{
				bool leave;
				do
				{
					WriteState(serializer, typeLibrary, instance, ouput);
					std::string instruction = PromptInstruction(input, ouput);
					InstructionResult result = ProcessInstruction(typeLibrary, serializer, instance, instruction);
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

			InstructionResult ProcessMemberInstruction(const Reflection::TypeLibrary& typeLibrary, const Serialization::Serializer& serializer, const Reflection::TypeDescriptorPtr& typeDescriptor, object_t& instance, const std::string& memberName, const std::string& memberValue)
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

			InstructionResult ProcessMethodInstruction(const Reflection::TypeLibrary& typeLibrary, const Serialization::Serializer& serializer, const Reflection::TypeDescriptorPtr& typeDescriptor, object_t& instance, const std::string& methodName, const std::vector<std::string>& parameters)
			{
				InstructionResult result;

				const Reflection::MethodDescriptor* methodDescriptor = typeDescriptor->GetMethodByName(methodName);
				if (methodDescriptor)
				{
					const size_t parameterCount = methodDescriptor->GetParameterCount();

					if (parameterCount == 0)
					{
						result = ProcessMethodInstruction<0>(typeLibrary, serializer, typeDescriptor, instance, *methodDescriptor, parameters);
					}
					else if (parameterCount == 1)
					{
						result = ProcessMethodInstruction<1>(typeLibrary, serializer, typeDescriptor, instance, *methodDescriptor, parameters);
					}
					else if (parameterCount == 2)
					{
						result = ProcessMethodInstruction<2>(typeLibrary, serializer, typeDescriptor, instance, *methodDescriptor, parameters);
					}
					else if (parameterCount == 3)
					{
						result = ProcessMethodInstruction<3>(typeLibrary, serializer, typeDescriptor, instance, *methodDescriptor, parameters);
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

				return result;
			}

			template<size_t param_count>
			InstructionResult ProcessMethodInstruction(const Reflection::TypeLibrary& typeLibrary, const Serialization::Serializer& serializer, const Reflection::TypeDescriptorPtr& typeDescriptor, object_t& instance, const Reflection::MethodDescriptor& methodDescriptor, const std::vector<std::string>& parameters)
			{
				const std::vector<ParameterDescriptor>& parametersDescriptor = methodDescriptor.GetParameters();

				ensure(parametersDescriptor.size() == param_count);

				MethodDescriptor::weak_resolved_method_t<param_count> method = methodDescriptor.WeakResolveMethod<object_t, param_count>(&instance);

				MethodDescriptor::weak_method_params_t<param_count> weak_parameters;

				if (param_count > 0)
				{
					for (size_t i = 0; i < param_count; ++i)
					{
						const ParameterDescriptor& parameterDescriptor = parametersDescriptor[i];
						const std::string strParameter = parameters[i];

						Serialization::JsonSerializationReader reader;
						std::istringstream stream = std::istringstream(strParameter);
						reader.Import(stream);

						std::any anyParameter;
						serializer.RawDeserialize(parameterDescriptor.GetType(), anyParameter, reader);

						weak_parameters[i] = anyParameter;
					}
				}
				
				std::any returnValue = AnyExt::Invoke(method, weak_parameters);
				return InstructionResult::Ok;
			}

			InstructionResult ProcessInstruction(const Reflection::TypeLibrary& typeLibrary, const Serialization::Serializer& serializer, object_t& instance, const std::string& instruction)
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

								result = ProcessMemberInstruction(typeLibrary, serializer, typeDescriptor, instance, memberName, memberValue);
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
							result = ProcessMethodInstruction(typeLibrary, serializer, typeDescriptor, instance, methodName, arguments);
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
						const std::string& returnType = methodDescriptor.GetReturnType() ? methodDescriptor.GetReturnType()->GetName() : "unknown";
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