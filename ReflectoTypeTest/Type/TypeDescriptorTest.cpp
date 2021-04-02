#include "Resolver/Resolver.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"
#include "Utils/EncapsulationBreaker.h"
#include "Utils/StringExt.h"

#include "CppUnitTest.h"
#include <tuple>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Reflecto::Type;
using namespace Reflecto::Utils;
using namespace Reflecto;

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

namespace Reflecto
{
	namespace Type
	{
		namespace Test
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
					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<uint32_t>("uint32")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<uint32_t>(typeLibrary).Build();

					const TypeDescriptorType& expectedType = *typeLibrary.Get<uint32_t>();
					Assert::AreEqual(expectedType, descriptor.Type(), L"Type is unexpected");

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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<float>("float")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary)
						.RegisterMember(&Potato::Weight, "Weight")
						.Build();

					const TypeDescriptorType& expectedType = *typeLibrary.Get<Potato>();
					Assert::AreEqual(expectedType, descriptor.Type(), L"Type is unexpected");

					const std::vector<MemberDescriptor> expectedMembers = [&] {
						const TypeDescriptorType type = typeLibrary.GetChecked<float>();
						const std::string name = "Weight";
						const byte offset = 0;
						return std::vector<MemberDescriptor>{ MemberDescriptor(type, name, offset) };
					}();
					Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
				}

				TEST_METHOD(MultipleMembers)
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<PotatoNoPadding>("PotatoNoPadding")
						.Add<std::string>("string")
						.Add<float>("float")
						.Add<int64_t>("int64")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<PotatoNoPadding>(typeLibrary)
						.RegisterMember(&PotatoNoPadding::Name, "Name")
						.RegisterMember(&PotatoNoPadding::Weight, "Weight")
						.RegisterMember(&PotatoNoPadding::CookedTime, "CookedTime")
					.Build();

					const TypeDescriptorType& expectedType = *typeLibrary.Get<PotatoNoPadding>();
					Assert::AreEqual(expectedType, descriptor.Type(), L"Type is unexpected");

					const std::vector<MemberDescriptor> expectedMembers = [&] {
						using member1_t = std::string;
						const MemberDescriptor member1 = [&] {
							const TypeDescriptorType type = typeLibrary.GetChecked<member1_t>();
							const std::string name = "Name";
							const byte offset = 0;
							return MemberDescriptor(type, name, offset);
						}();
						using member2_t = float;
						const MemberDescriptor member2 = [&] {
							const TypeDescriptorType type = typeLibrary.GetChecked<member2_t>();
							const std::string name = "Weight";
							const byte offset = sizeof(member1_t);
							return MemberDescriptor(type, name, offset);
						}();
						using member3_t = int64_t;
						const MemberDescriptor member3 = [&] {
							const TypeDescriptorType type = typeLibrary.GetChecked<member3_t>();
							const std::string name = "CookedTime";
							const byte offset = sizeof(member1_t) + sizeof(member2_t);
							return MemberDescriptor(type, name, offset);
						}();
						return std::vector<MemberDescriptor>{member1, member2, member3};
					}();

					Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
				}

				TEST_METHOD(PrivateMember)
				{
					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<PrivatePotato>("PrivatePotato")
						.Add<float>("float")
						.Add<std::string>("string")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<PrivatePotato>(typeLibrary)
						.RegisterMember(GetPrivateMemberPointer(PrivatePotatoWeightTag()), "Weight")
						.RegisterMember(GetPrivateMemberPointer(PrivatePotatoNameTag()), "Name")
					.Build();

					const TypeDescriptorType& expectedType = *typeLibrary.Get<PrivatePotato>();
					Assert::AreEqual(expectedType, descriptor.Type(), L"Type is unexpected");

					const std::vector<MemberDescriptor> expectedMembers = [&] {
						const MemberDescriptor member1 = [&] {
							const TypeDescriptorType type = typeLibrary.GetChecked<float>();
							const std::string name = "Weight";
							const byte offset = 0;
							return MemberDescriptor(type, name, offset);
						}();
						const MemberDescriptor member2 = [&] {
							const TypeDescriptorType type = typeLibrary.GetChecked<std::string>();
							const std::string name = "Name";
							const byte offset = sizeof(float);
							return MemberDescriptor(type, name, offset);
						}();
						return std::vector<MemberDescriptor>{member1, member2};
					}();
					Assert::AreEqual(expectedMembers, descriptor.Members(), L"Type members are unexpected");
				}

				TEST_METHOD(PublicInheritance)
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<VegetableNoPadding>("VegetableNoPadding")
						.Add<PotatoNoPadding>("PotatoNoPadding")
						.Add<float>("float")
						.Add<bool>("bool")
					.Build();

					const TypeDescriptor baseDescriptor = TypeDescriptorFactory<VegetableNoPadding>(typeLibrary)
						.RegisterMember(&VegetableNoPadding::Weight, "Weight")
						.RegisterMember(&VegetableNoPadding::Condition, "Condition")
					.Build();

					const TypeDescriptor childDescriptor = TypeDescriptorFactory<PotatoNoPadding>(typeLibrary, &baseDescriptor)
						.RegisterMember(&PotatoNoPadding::IsBacked, "IsBacked")
						.Build();

					const TypeDescriptorType baseExpectedType = *typeLibrary.Get<VegetableNoPadding>();
					Assert::AreEqual(baseExpectedType, baseDescriptor.Type(), L"Type is unexpected");

					const TypeDescriptorType childExpectedType = *typeLibrary.Get<PotatoNoPadding>();
					Assert::AreEqual(childExpectedType, childDescriptor.Type(), L"Type is unexpected");

					using baseMember1_t = float;
					using baseMember2_t = float;
					const std::vector<MemberDescriptor> baseExpectedMembers = [&] {
						const MemberDescriptor member1 = [&] {
							const TypeDescriptorType type = *typeLibrary.Get<baseMember1_t>();
							const std::string name = "Weight";
							const byte offset = 0;
							return MemberDescriptor{ type, name, offset };
						}();

						const MemberDescriptor member2 = [&] {
							const TypeDescriptorType type = *typeLibrary.Get<baseMember2_t>();
							const std::string name = "Condition";
							const byte offset = sizeof(baseMember1_t);
							return MemberDescriptor{ type, name, offset };
						}();

						return std::vector<MemberDescriptor> { member1, member2 };
					}();
					Assert::AreEqual(baseExpectedMembers, baseDescriptor.Members(), L"Type members are unexpected");

					using childMember1_t = bool;
					const std::vector<MemberDescriptor> childExpectedMembers = [&] {
						const MemberDescriptor member1 = [&] {
							const TypeDescriptorType type = *typeLibrary.Get<childMember1_t>();
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

				TEST_METHOD(PublicInheritanceRegisterOnChildClass)
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Vegetable>("Vegetable")
						.Add<Potato>("Potato")
						.Add<int>("int")
						.Add<float>("float")
					.Build();

					const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>(typeLibrary)
						.RegisterMember(&Vegetable::Age, "Age")
					.Build();

					const TypeDescriptor childDescriptor = TypeDescriptorFactory<Potato>(typeLibrary, &baseDescriptor)
						.RegisterMember(&Vegetable::Weight, "Weight")
					.Build();

					using member_information_t = std::tuple<std::string, TypeDescriptorType>;
					std::vector<member_information_t> expectedMembers = [&] {
						auto builderFunc = std::make_tuple<std::string, TypeDescriptorType>;
						member_information_t member1 = builderFunc("Age", typeLibrary.GetChecked<int>());
						member_information_t member2 = builderFunc("Weight", typeLibrary.GetChecked<float>());
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

					const TypeLibrary typeLibrary = TypeLibraryFactory().Add<Potato>("Potato").Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary).Build();

					const ConstructorDescriptor& constructorDescriptor = descriptor.Constructor();
					Assert::IsTrue(static_cast<bool>(constructorDescriptor.Function()), L"No constructor function set!");
				}
			};
		}
	}
}
