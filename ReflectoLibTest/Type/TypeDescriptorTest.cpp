#include "../TestExtensions.h"

#include "Resolver/Resolver.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"

#include "Common/Utils/EncapsulationBreaker.h"

#include "CppUnitTest.h"
#include <tuple>
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

			template<> inline std::wstring ToString<std::vector<MemberDescriptor>>(const std::vector<MemberDescriptor>& members)
			{
				const std::wstring membersStr = StringExt::Join<std::wstring>(members, L",", [](const MemberDescriptor& members) {
					const std::wstring name = StringExt::ToWstring(members.Name());
					const std::string& type = members.Type().Name();
					const byte offset = members.Offset();
					return StringExt::Format<std::wstring>(L"Name=%s,Type=%s,Offset=%i", name.c_str(), type.c_str(), offset);
				});
				return StringExt::Format<std::wstring>(L"[%s]", membersStr.c_str());
			}

			template<> inline std::wstring ToString<std::vector<MethodDescriptor>>(const std::vector<MethodDescriptor>& methods)
			{
				const std::wstring membersStr = StringExt::Join<std::wstring>(methods, L",", [](const MethodDescriptor& method) {
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
#pragma pack(push, 1)
	class PrivatePotato
	{
	private:
		float _weight = 0.f;
		std::string _name;
	};
#pragma pack(pop)

	struct PrivatePotatoWeightTag : public TypeMemberTag<PrivatePotato, float> { };
	struct PrivatePotatoNameTag : public TypeMemberTag<PrivatePotato, std::string> { };

	template struct EncapsulationBreaker<PrivatePotatoWeightTag, &PrivatePotato::_weight>;
	template struct EncapsulationBreaker<PrivatePotatoNameTag, &PrivatePotato::_name>;

	TEST_CLASS(TypeDescriptorTest)
	{
	public:
		TEST_METHOD(EmptyType)
		{
			const TypeDescriptor descriptor = TypeDescriptorFactory<uint32_t>().Build();

			TypeDescriptorType expectedType = TypeDescriptorTypeFactory<uint32_t>().Build();
			Assert::AreEqual(expectedType, descriptor.Type(), L"Type hash is unexpected");
			
			std::vector<MemberDescriptor> expectedMembers;
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

			const std::vector<MemberDescriptor> expectedMembers = [] {
				const TypeDescriptorType type = TypeDescriptorTypeFactory<float>().Build();
				const std::string name = "Weight";
				const byte offset = 0;
				return std::vector<MemberDescriptor>{ MemberDescriptor(type, name, offset) };
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

			const std::vector<MemberDescriptor> expectedMembers = [] {
				using member1_t = std::string;
				const MemberDescriptor member1 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<member1_t>().Build();
					const std::string name = "Name";
					const byte offset = 0;
					return MemberDescriptor(type, name, offset);
				}();
				using member2_t = float;
				const MemberDescriptor member2 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<member2_t>().Build();
					const std::string name = "Weight";
					const byte offset = sizeof(member1_t);
					return MemberDescriptor(type, name, offset);
				}();
				using member3_t = int64_t;
				const MemberDescriptor member3 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<member3_t>().Build();
					const std::string name = "CookedTime";
					const byte offset = sizeof(member1_t) + sizeof(member2_t);
					return MemberDescriptor(type, name, offset);
				}();
				return std::vector<MemberDescriptor>{member1, member2, member3};
			}();

			Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
		}

		TEST_METHOD(MultipleMembers)
		{
			class Potato
			{
			public:
				std::string Name;
				float Weight = 0.f;
				int64_t CookedTime = 0;
			};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>()
				.Register(&Potato::Name, "Name")
				.Register(&Potato::Weight, "Weight")
				.Register(&Potato::CookedTime, "CookedTime")
			.Build();

			const TypeDescriptorType expectedType = TypeDescriptorTypeFactory<Potato>().Build();
			Assert::AreEqual(expectedType, descriptor.Type(), L"Type hash is unexpected");

			const std::size_t expectedMemberCount = 3;
			Assert::AreEqual(expectedMemberCount, descriptor.Members().size(), L"Type members count is unexpected");

			using member_information_t = std::tuple<std::string, TypeDescriptorType>;
			std::vector<member_information_t> expectedMember = [] {
				auto builderFunc = std::make_tuple<std::string, TypeDescriptorType>;
				member_information_t member1 = builderFunc("Name", TypeDescriptorTypeFactory<std::string>().Build());
				member_information_t member2 = builderFunc("Weight", TypeDescriptorTypeFactory<float>().Build());
				member_information_t member3 = builderFunc("CookedTime", TypeDescriptorTypeFactory<int64_t>().Build());
				return std::vector<member_information_t>{member1, member2, member3};
			}();

			for (std::size_t i = 0; i < descriptor.Members().size(); ++i)
			{
				const MemberDescriptor& member = descriptor.Members()[i];
				const member_information_t& expectedMemberInfo = expectedMember[i];
				Assert::AreEqual(std::get<0>(expectedMemberInfo), member.Name(), L"Type member name is unexpected");
				Assert::AreEqual(std::get<1>(expectedMemberInfo), member.Type(), L"Type member type is unexpected");
				if (i > 0)
				{
					const MemberDescriptor& previousMember = descriptor.Members()[i-1];
					Assert::IsTrue(member.Offset() > previousMember.Offset(), L"Type member offset are out of order!");
				}
			}
		}

		TEST_METHOD(PrivateMember)
		{
			const TypeDescriptor descriptor = TypeDescriptorFactory<PrivatePotato>()
				.Register(PryPrivate(PrivatePotatoWeightTag()), "Weight")
				.Register(PryPrivate(PrivatePotatoNameTag()), "Name")
			.Build();

			const TypeDescriptorType expectedType = TypeDescriptorTypeFactory<PrivatePotato>().Build();
			Assert::AreEqual(expectedType, descriptor.Type(), L"Type hash is unexpected");

			const std::vector<MemberDescriptor> expectedMembers = [] {
				const MemberDescriptor member1 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<float>().Build();
					const std::string name = "Weight";
					const byte offset = 0;
					return MemberDescriptor(type, name, offset);
				}();
				const MemberDescriptor member2 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<std::string>().Build();
					const std::string name = "Name";
					const byte offset = sizeof(float);
					return MemberDescriptor(type, name, offset);
				}();
				return std::vector<MemberDescriptor>{member1, member2};
			}();
			Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
		}

		TEST_METHOD(PublicInheritanceNoPadding)
		{
#pragma pack(push, 1)
			class VegetableNoPadding
			{
			public:
				float Weight = 0.f;
				float Condition = 1.f;
			};

			class PotatoNoPadding : public VegetableNoPadding
			{
			public:
				bool IsBacked = false;
			};
#pragma pack(pop)

			const TypeDescriptor baseDescriptor = TypeDescriptorFactory<VegetableNoPadding>()
				.Register(&VegetableNoPadding::Weight, "Weight")
				.Register(&VegetableNoPadding::Condition, "Condition")
			.Build();

			const TypeDescriptor childDescriptor = TypeDescriptorFactory<PotatoNoPadding>(&baseDescriptor)
				.Register(&PotatoNoPadding::IsBacked, "IsBacked")
			.Build();

			const TypeDescriptorType baseExpectedType = TypeDescriptorTypeFactory<VegetableNoPadding>().Build();
			Assert::AreEqual(baseExpectedType, baseDescriptor.Type(), L"Type hash is unexpected");

			const TypeDescriptorType childExpectedType = TypeDescriptorTypeFactory<PotatoNoPadding>().Build();
			Assert::AreEqual(childExpectedType, childDescriptor.Type(), L"Type hash is unexpected");

			using baseMember1_t = float;
			using baseMember2_t = float;
			const std::vector<MemberDescriptor> baseExpectedMembers = [] {
				MemberDescriptor member1 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<baseMember1_t>().Build();
					const std::string name = "Weight";
					const byte offset = 0;
					return MemberDescriptor{ type, name, offset };
				}();

				MemberDescriptor member2 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<baseMember2_t>().Build();
					const std::string name = "Condition";
					const byte offset = sizeof(baseMember1_t);
					return MemberDescriptor{ type, name, offset };
				}();

				return std::vector<MemberDescriptor> { member1, member2 };
			}();
			Assert::AreEqual(baseExpectedMembers, baseDescriptor.Members(), L"Type members are unexpected");

			using childMember1_t = bool;
			const std::vector<MemberDescriptor> childExpectedMembers = [] {
				MemberDescriptor member1 = [] {
					const TypeDescriptorType type = TypeDescriptorTypeFactory<childMember1_t>().Build();
					const std::string name = "IsBacked";
					const byte offset = sizeof(baseMember1_t) + sizeof(baseMember2_t);
					return MemberDescriptor{ type, name, offset };
				}();

				return std::vector<MemberDescriptor> { member1 };
			}();
			Assert::AreEqual(childExpectedMembers, childDescriptor.Members(), L"Type members are unexpected");

			const std::vector<MemberDescriptor> childExpectedMembersRecursive = [&] {
				std::vector<MemberDescriptor> recursive;
				recursive.insert(recursive.end(), baseExpectedMembers.begin(), baseExpectedMembers.end());
				recursive.insert(recursive.end(), childExpectedMembers.begin(), childExpectedMembers.end());
				return recursive;
			}();
			Assert::AreEqual(childExpectedMembersRecursive, childDescriptor.MemberResursive(), L"Type recursive members are unexpected");
		}

		TEST_METHOD(PublicInheritanceRegisterOnChild)
		{
			class Vegetable
			{
			public:
				int Age = 0;
				float Weight = 0.f;
			};

			class Potato : public Vegetable
			{
			};

			const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>()	
				.Register(&Vegetable::Age, "Age")
			.Build();

			const TypeDescriptor childDescriptor = TypeDescriptorFactory<Potato>(&baseDescriptor)
				.Register(&Vegetable::Weight, "Weight")
			.Build();

			using member_information_t = std::tuple<std::string, TypeDescriptorType>;
			std::vector<member_information_t> expectedMembers = [] {
				auto builderFunc = std::make_tuple<std::string, TypeDescriptorType>;
				member_information_t member1 = builderFunc("Age", TypeDescriptorTypeFactory<int>().Build());
				member_information_t member2 = builderFunc("Weight", TypeDescriptorTypeFactory<float>().Build());
				return std::vector<member_information_t>{member1, member2};
			}();

			std::vector<MemberDescriptor> members = childDescriptor.MemberResursive();
			for (std::size_t i = 0; i < members.size(); ++i)
			{
				const MemberDescriptor& member = members[i];
				const member_information_t& expectedMemberInfo = expectedMembers[i];
				Assert::AreEqual(std::get<0>(expectedMemberInfo), member.Name(), L"Type member name is unexpected");
				Assert::AreEqual(std::get<1>(expectedMemberInfo), member.Type(), L"Type member type is unexpected");
				if (i > 0)
				{
					const MemberDescriptor& previousMember = members[i - 1];
					Assert::IsTrue(member.Offset() > previousMember.Offset(), L"Type member offset are out of order!");
				}
			}
		}

		TEST_METHOD(Constructor)
		{
			class Potato {};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>().Build();

			const ConstructorDescriptor& constructorDescriptor = descriptor.Constructor();
			Assert::IsTrue(static_cast<bool>(constructorDescriptor.Function()), L"No constructor function set!");
		}
	};
}
