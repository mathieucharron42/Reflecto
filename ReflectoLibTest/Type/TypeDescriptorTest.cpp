#include "../TestExtensions.h"

#include "Type/Resolver.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"

#include "CppUnitTest.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<TypeDescriptorType>(const TypeDescriptorType& type)
			{
				std::string name = type.Name();
				typehash_t hash = type.Hash();
				return StringExt::Format<std::wstring>(L"{Name=%s,TypeHash=%i}", name.c_str(), hash);
			}

			template<> inline std::wstring ToString<std::vector<TypeDescriptorMember>>(const std::vector<TypeDescriptorMember>& members)
			{
				const std::wstring membersStr = StringExt::Join<std::wstring>(members, L",", [](const TypeDescriptorMember& members) {
					const std::wstring name = StringExt::ToWstring(members.Name());
					const std::string& type = members.Type().Name();
					const byte offset = members.Offset();
					return StringExt::Format<std::wstring>(L"Name=%s,TypeHash=%s,Offset=%i", name.c_str(), type.c_str(), offset);
				});
				return StringExt::Format<std::wstring>(L"[%s]", membersStr.c_str());
			}

			template<> inline std::wstring ToString<std::vector<TypeDescriptorMethod>>(const std::vector<TypeDescriptorMethod>& methods)
			{
				const std::wstring membersStr = StringExt::Join<std::wstring>(methods, L",", [](const TypeDescriptorMethod& method) {
					const std::wstring name = StringExt::ToWstring(method.Name());
					return StringExt::Format<std::wstring>(L"Name=%s", name.c_str());
					});
				return StringExt::Format<std::wstring>(L"[%s]", membersStr.c_str());
			}
		}
	}
}

namespace LibTest
{	
	TEST_CLASS(TypeDescriptorTest)
	{
	public:
		TEST_METHOD(EmptyType)
		{
			const TypeDescriptor descriptor = TypeDescriptorFactory<uint32_t>().Build();

			TypeDescriptorType expectedType = TypeDescriptorTypeFactory<uint32_t>().Build();
			Assert::AreEqual(expectedType, descriptor.Type(), L"Type hash is unexpected");
			
			std::vector<TypeDescriptorMember> expectedMembers;
			Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
		}

		TEST_METHOD(SingleMember)
		{
			class Potato
			{
			public:
				float Weight = 0.f;
			};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>()
				.Register(&Potato::Weight, "Weight")
			.Build();

			const TypeDescriptorType expectedType = TypeDescriptorTypeFactory<Potato>().Build();
			Assert::AreEqual(expectedType, descriptor.Type(), L"Type hash is unexpected");

			const std::vector<TypeDescriptorMember> expectedMembers = [] {
				const TypeDescriptorType type = TypeDescriptorTypeFactory<float>().Build();
				const std::string name = "Weight";
				const byte offset = 0;
				return std::vector<TypeDescriptorMember>{ TypeDescriptorMember(type, name, offset) };
			}();
			Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
		}

		TEST_METHOD(MultipleMembersNoPadding)
		{
#pragma pack(push, 1)
			class PotatoNoPadding
			{
			public:
				std::string Name;
				float Weight = 0.f;
				int64_t CookedTime = 0;
			};
#pragma pack(pop)

			const TypeDescriptor descriptor = TypeDescriptorFactory<PotatoNoPadding>()
				.Register(&PotatoNoPadding::Name, "Name")
				.Register(&PotatoNoPadding::Weight, "Weight")
				.Register(&PotatoNoPadding::CookedTime, "CookedTime")
			.Build();

			const TypeDescriptorType expectedType = TypeDescriptorTypeFactory<PotatoNoPadding>().Build();
			Assert::AreEqual(expectedType, descriptor.Type(), L"Type hash is unexpected");

			const std::vector<TypeDescriptorMember> expectedMembers = [] {
				using member1_t = std::string;
				const TypeDescriptorMember member1 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<member1_t>().Build();
					const std::string name = "Name";
					const byte offset = 0;
					return TypeDescriptorMember(type, name, offset);
				}();
				using member2_t = float;
				const TypeDescriptorMember member2 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<member2_t>().Build();
					const std::string name = "Weight";
					const byte offset = sizeof(member1_t);
					return TypeDescriptorMember(type, name, offset);
				}();
				using member3_t = int64_t;
				const TypeDescriptorMember member3 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<member3_t>().Build();
					const std::string name = "CookedTime";
					const byte offset = sizeof(member1_t) + sizeof(member2_t);
					return TypeDescriptorMember(type, name, offset);
				}();
				return std::vector<TypeDescriptorMember>{member1, member2, member3};
			}();

			Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
		}
	};
}
