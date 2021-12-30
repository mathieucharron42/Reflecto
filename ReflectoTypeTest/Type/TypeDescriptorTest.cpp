#include "TestCommon.h"

#include "Resolver/Resolver.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"
#include "Utils/EncapsulationBreaker.h"
#include "Utils/StringExt.h"
#include "Utils/CollectionExt.h"

#include <CppUnitTest.h>
#include <tuple>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Reflection
	{
		namespace Test
		{
			class PrivateSampleClass
			{
			private:
				float _field = 0.f;
			};

			struct PrivateSampleClassFieldTag : public Reflecto::Utils::TypeMemberTag<PrivateSampleClass, float> { };
			
			template struct Reflecto::Utils::EncapsulationBreaker<PrivateSampleClassFieldTag, &PrivateSampleClass::_field>;

			TEST_CLASS(TypeDescriptorTest)
			{
			public:
				TEST_METHOD(Members)
				{
					/////////////
					// Arrange
					class SampleClass
					{
					public:
						float Field1 = 0.f;
						std::string Field2;
						bool Field3 = false;
					};

					class ChildSampleClass : public SampleClass
					{
					public:
						uint64_t Field4 = 0;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<uint64_t>("uint64")
						.Add<float>("float")
						.Add<std::string>("string")
						.Add<bool>("bool")
						.Add<SampleClass>("SampleClass")
						.Add<ChildSampleClass>("ChildSampleClass")
						.Add<PrivateSampleClass>("PrivateSampleClass")
					.Build();

					const std::vector<MemberDescriptor> kExpectedMemberEmpty;
					const std::vector<MemberDescriptor> kExpectedMembersSampleClass = [&] () -> std::vector<MemberDescriptor> {
						MemberDescriptor m1(typeLibrary.GetChecked<float>(), "Field1", 0);
						MemberDescriptor m2(typeLibrary.GetChecked<std::string>(), "Field2", 0);
						MemberDescriptor m3(typeLibrary.GetChecked<bool>(), "Field3", 0);
						return {m1, m2, m3};
					}();
					const std::vector<MemberDescriptor> kExpectedMembersChildSampleClass = [&]() -> std::vector<MemberDescriptor> {
						MemberDescriptor m4(typeLibrary.GetChecked<uint64_t>(), "Field4", 0);
						return { m4 };
					}();
					const std::vector<MemberDescriptor> kExpectedMembersRecursiveChildSampleClass = [&]() -> std::vector<MemberDescriptor> {
						return Utils::CollectionExt::Concatenate(kExpectedMembersChildSampleClass, kExpectedMembersRecursiveChildSampleClass);
					}();
					const std::vector<MemberDescriptor> kExpectedPrivateMembers = [&] () -> std::vector<MemberDescriptor> {
						MemberDescriptor m1(typeLibrary.GetChecked<float>(), "Field", 0);
						return { m1 };
					}();

					/////////////
					// Act
					const TypeDescriptor uint64Descriptor = TypeDescriptorFactory<uint64_t>(typeLibrary).Build();
					const TypeDescriptor sampleClassDescriptor = TypeDescriptorFactory<SampleClass>(typeLibrary)
						.RegisterMember(&SampleClass::Field1, "Field1")
						.RegisterMember(&SampleClass::Field2, "Field2")
						.RegisterMember(&SampleClass::Field3, "Field3")
					.Build();
					const TypeDescriptor childSampleClassDescriptor = TypeDescriptorFactory<ChildSampleClass>(typeLibrary)
						.RegisterMember(&ChildSampleClass::Field4, "Field4")
					.Build();
					const TypeDescriptor privateSampleClassDescriptor = TypeDescriptorFactory<PrivateSampleClass>(typeLibrary)
						.RegisterMember(GetPrivateMemberPointer(PrivateSampleClassFieldTag()), "Field")
					.Build();

					////////////
					// Arrange
					Assert::AreEqual(kExpectedMemberEmpty, uint64Descriptor.GetMembers(), L"Members are unexpected");
					Assert::AreEqual(kExpectedMembersSampleClass, sampleClassDescriptor.GetMembers(), L"Members are unexpected");
					Assert::AreEqual(kExpectedMembersChildSampleClass, childSampleClassDescriptor.GetMembers(), L"Members are unexpected");
					Assert::AreEqual(kExpectedMembersRecursiveChildSampleClass, childSampleClassDescriptor.FetchMemberResursive(), L"Members are unexpected");
					Assert::AreEqual(kExpectedPrivateMembers, privateSampleClassDescriptor.GetMembers(), L"Members are unexpected");
				}

				TEST_METHOD(Constructor)
				{
					/////////////
					// Arrange
					class SampleClass {};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleClass>("SampleClass")
					.Build();

					/////////////
					// Act
					const TypeDescriptor descriptor = TypeDescriptorFactory<SampleClass>(typeLibrary).Build();

					/////////////
					// Assert
					const ConstructorDescriptor& constructorDescriptor = descriptor.GetConstructor();
					Assert::IsTrue(static_cast<bool>(constructorDescriptor.GetConstructorMethod<SampleClass>()), L"No constructor function set!");
				}

				TEST_METHOD(Methods)
				{
					/////////////
					// Arrange
					class SampleClass
					{
					public:
						void MethodNoParameter() { }
						bool MethodReturn() { return bool(); }
						void Method1Parameter(int32_t) { }
						void Method2Parameter(int32_t, int32_t) { }
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleClass>("SampleClass")
						.Add<int32_t>("int32")
					.Build();

					const std::vector<MethodDescriptor> kExpectedMethod = [] () -> std::vector<MethodDescriptor> {
						MethodDescriptor m1(MethodDescriptor::method_ptr_t<SampleClass>(), "MethodNoParameter");
						MethodDescriptor m2(MethodDescriptor::method_ptr_t<TestClass, bool>(), "MethodReturn");
						MethodDescriptor m3(MethodDescriptor::method_ptr_t<TestClass, int32_t>(), "Method1Parameter");
						MethodDescriptor m4(MethodDescriptor::method_ptr_t<SampleClass, int32_t, int32_t>(), "Method2Parameter");
						return { m1, m2, m3, m4 };
					}();

					/////////////
					// Act
					const TypeDescriptor descriptor = TypeDescriptorFactory<SampleClass>(typeLibrary)
						.RegisterMethod(&SampleClass::MethodNoParameter, "MethodNoParameter")
						.RegisterMethod(&SampleClass::MethodReturn, "MethodReturn")
						.RegisterMethod(&SampleClass::Method1Parameter, "Method1Parameter")
						.RegisterMethod(&SampleClass::Method2Parameter, "Method2Parameter")
					.Build();


					/////////////
					// Assert
					Assert::AreEqual(kExpectedMethod, descriptor.GetMethods());
				}
			};
		}
	}
}
