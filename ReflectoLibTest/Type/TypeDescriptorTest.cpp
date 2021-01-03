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
			template<> inline std::wstring ToString<std::vector<TypeDescriptorMember>>(const std::vector<TypeDescriptorMember>& members)
			{
				const std::wstring membersStr = StringExt::Join<std::wstring>(members, L",", [](const TypeDescriptorMember& members) {
					const std::wstring name = StringExt::ToWstring(members.Name());
					const Type::HashType typeHash = members.Type();
					const byte offset = members.Offset();
					return StringExt::Format<std::wstring>(L"Name=%s,TypeHash=%u,Offset=%i", name.c_str(), typeHash, offset);
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

			Assert::AreEqual(descriptor.TypeHash(), Type::GetTypeHash<uint32_t>(), L"Type hash is unexpected");
			
			Assert::AreEqual(descriptor.Members(), std::vector<TypeDescriptorMember>(), L"Type members are unexpected");
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

			Assert::AreEqual(descriptor.TypeHash(), Type::GetTypeHash<Potato>(), L"Type hash is unexpected");

			const std::vector<TypeDescriptorMember> expectedMembers = [] {
				const Type::HashType type = Type::GetTypeHash<float>();
				const std::string name = "Weight";
				const byte offset = 0;
				return std::vector<TypeDescriptorMember>{ TypeDescriptorMember(type, name, offset) };
			}();

			Assert::AreEqual(descriptor.Members(), expectedMembers, L"Type members are unexpected");
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

			Assert::AreEqual(descriptor.TypeHash(), Type::GetTypeHash<PotatoNoPadding>(), L"Type hash is unexpected");

			const std::vector<TypeDescriptorMember> expectedMembers = [] {
				using Member1Type = std::string;
				const TypeDescriptorMember member1 = [] {
					const Type::HashType type = Type::GetTypeHash<Member1Type>();
					const std::string name = "Name";
					const byte offset = 0;
					return TypeDescriptorMember(type, name, offset);
				}();
				using Member2Type = float;
				const TypeDescriptorMember member2 = [] {
					const Type::HashType type = Type::GetTypeHash<float>();
					const std::string name = "Weight";
					const byte offset = sizeof(Member1Type);
					return TypeDescriptorMember(type, name, offset);
				}();
				using Member3Type = int64_t;
				const TypeDescriptorMember member3 = [] {
					const Type::HashType type = Type::GetTypeHash<int64_t>();
					const std::string name = "CookedTime";
					const byte offset = sizeof(Member1Type) + sizeof(Member2Type);
					return TypeDescriptorMember(type, name, offset);
				}();
				return std::vector<TypeDescriptorMember>{member1, member2, member3};
			}();

			Assert::AreEqual(descriptor.Members(), expectedMembers, L"Type members are unexpected");
		}
	};
}
