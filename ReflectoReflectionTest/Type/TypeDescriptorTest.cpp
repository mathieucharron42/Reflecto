#include "TestCommon.h"

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

			struct PrivateSampleClassFieldTag : public Reflecto::TypeMemberTag<PrivateSampleClass, float> { };
			
			template struct Reflecto::EncapsulationBreaker<PrivateSampleClassFieldTag, &PrivateSampleClass::_field>;

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
						MemberDescriptor m1(typeLibrary.GetDescriptor<float>(), "Field1", 0);
						MemberDescriptor m2(typeLibrary.GetDescriptor<std::string>(), "Field2", 0);
						MemberDescriptor m3(typeLibrary.GetDescriptor<bool>(), "Field3", 0);
						return {m1, m2, m3};
					}();
					const std::vector<MemberDescriptor> kExpectedMembersChildSampleClass = [&]() -> std::vector<MemberDescriptor> {
						MemberDescriptor m4(typeLibrary.GetDescriptor<uint64_t>(), "Field4", 0);
						return { m4 };
					}();
					const std::vector<MemberDescriptor> kExpectedMembersRecursiveChildSampleClass = [&]() -> std::vector<MemberDescriptor> {
						return CollectionExt::Concatenate(kExpectedMembersChildSampleClass, kExpectedMembersRecursiveChildSampleClass);
					}();
					const std::vector<MemberDescriptor> kExpectedPrivateMembers = [&] () -> std::vector<MemberDescriptor> {
						MemberDescriptor m1(typeLibrary.GetDescriptor<float>(), "Field", 0);
						return { m1 };
					}();

					/////////////
					// Act
					const TypeDescriptorUniquePtr uint64Descriptor = TypeDescriptorFactory<uint64_t>(typeLibrary, "uint64").Build();
					const TypeDescriptorUniquePtr sampleClassDescriptor = TypeDescriptorFactory<SampleClass>(typeLibrary, "SampleClass")
						.RegisterMember(&SampleClass::Field1, "Field1")
						.RegisterMember(&SampleClass::Field2, "Field2")
						.RegisterMember(&SampleClass::Field3, "Field3")
					.Build();
					const TypeDescriptorUniquePtr childSampleClassDescriptor = TypeDescriptorFactory<ChildSampleClass>(typeLibrary, "ChildSampleClass")
						.RegisterMember(&ChildSampleClass::Field4, "Field4")
					.Build();
					const TypeDescriptorUniquePtr privateSampleClassDescriptor = TypeDescriptorFactory<PrivateSampleClass>(typeLibrary, "PrivateSampleClass")
						.RegisterMember(GetPrivateMemberPointer(PrivateSampleClassFieldTag()), "Field")
					.Build();

					////////////
					// Arrange
					Assert::AreEqual(kExpectedMemberEmpty, uint64Descriptor->GetMembers(), L"Members are unexpected");
					Assert::AreEqual(kExpectedMembersSampleClass, sampleClassDescriptor->GetMembers(), L"Members are unexpected");
					Assert::AreEqual(kExpectedMembersChildSampleClass, childSampleClassDescriptor->GetMembers(), L"Members are unexpected");
					Assert::AreEqual(kExpectedMembersRecursiveChildSampleClass, childSampleClassDescriptor->FetchMemberResursive(), L"Members are unexpected");
					Assert::AreEqual(kExpectedPrivateMembers, privateSampleClassDescriptor->GetMembers(), L"Members are unexpected");
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
					const TypeDescriptorUniquePtr descriptor = TypeDescriptorFactory<SampleClass>(typeLibrary, "SampleClass").Build();

					/////////////
					// Assert
					const OptionalConstructorDescriptor& constructorDescriptor = descriptor->GetConstructor();
					Assert::IsTrue(constructorDescriptor.has_value(), L"No constructor function set!");
					Assert::IsTrue(static_cast<bool>(constructorDescriptor.value().GetConstructorMethod<SampleClass>()), L"No constructor function set!");
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
						void Method1Parameter(std::string p) { }
						void Method2Parameter(int32_t param1, int32_t param2) { }
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleClass>("SampleClass")
						.Add<int32_t>("int32")
						.Add<bool>("bool")
						.Add<std::string>("string")
						.Add<void>("void")
					.Build();

					const std::vector<MethodDescriptor> kExpectedMethod = [&] () -> std::vector<MethodDescriptor> {
						MethodDescriptor m1(typeLibrary.GetDescriptor<void>(), "MethodNoParameter", {}, MethodDescriptor::weak_method_ptr_t<SampleClass>());
						MethodDescriptor m2(typeLibrary.GetDescriptor<bool>(), "MethodReturn", {}, MethodDescriptor::weak_method_ptr_t<TestClass, bool>());
						MethodDescriptor m3(typeLibrary.GetDescriptor<void>(), "Method1Parameter", {ParameterDescriptor(typeLibrary.GetDescriptor<std::string>(), "p")}, MethodDescriptor::weak_method_ptr_t<TestClass, int32_t>());
						MethodDescriptor m4(typeLibrary.GetDescriptor<void>(), "Method2Parameter", { ParameterDescriptor(typeLibrary.GetDescriptor<int32_t>(), "param1"), ParameterDescriptor(typeLibrary.GetDescriptor<int32_t>(), "param2") }, MethodDescriptor::weak_method_ptr_t<SampleClass, int32_t, int32_t>());
						return { m1, m2, m3, m4 };
					}();

					/////////////
					// Act
					const TypeDescriptorUniquePtr descriptor = TypeDescriptorFactory<SampleClass>(typeLibrary, "SampleClass")
						.RegisterMethod(&SampleClass::MethodNoParameter, "MethodNoParameter")
						.RegisterMethod(&SampleClass::MethodReturn, "MethodReturn")
						.RegisterMethod(&SampleClass::Method1Parameter, "Method1Parameter", {"p"})
						.RegisterMethod(&SampleClass::Method2Parameter, "Method2Parameter", {"param1", "param2"})
					.Build();


					/////////////
					// Assert
					Assert::AreEqual(kExpectedMethod, descriptor->GetMethods());
				}
			};
		}
	}
}
