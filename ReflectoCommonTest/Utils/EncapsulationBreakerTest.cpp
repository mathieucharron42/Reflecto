#include "Common/Utils/EncapsulationBreaker.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ReflectoCommonTest
{
	class PrivateTestClass
	{
	public:
		int i;

		static int PrivateTestClass::* GetIPtr()
		{
			return &PrivateTestClass::i;
		}

		int* GetIMember()
		{
			return &i;
		}

		static std::string PrivateTestClass::* GetJPtr()
		{
			return &PrivateTestClass::_j;
		}

		std::string* GetJMember()
		{
			return &_j;
		}

		static float PrivateTestClass::* GetKPtr()
		{
			return &PrivateTestClass::_k;
		}

		float* GetKMember()
		{
			return &_k;
		}

	private:
		std::string _j;
		float _k;
	};

	struct IMemberTag : public TypeMemberTag<PrivateTestClass, int> { };
	struct JMemberTag : public TypeMemberTag<PrivateTestClass, std::string> { };
	struct KMemberTag : public TypeMemberTag<PrivateTestClass, float> { };

	template struct EncapsulationBreaker<IMemberTag, &PrivateTestClass::i>;
	template struct EncapsulationBreaker<JMemberTag, &PrivateTestClass::_j>;
	template struct EncapsulationBreaker<KMemberTag, &PrivateTestClass::_k>;

	TEST_CLASS(EncapsulationBreaker)
	{
	public:
		TEST_METHOD(GetPrivateMemberPointerTest)
		{
			int PrivateTestClass::* expectedIMemberPtr = PrivateTestClass::GetIPtr();
			int PrivateTestClass::* actualIMemberPtr = GetPrivateMemberPointer(IMemberTag());
			Assert::IsTrue(expectedIMemberPtr == &PrivateTestClass::i, L"Unexpected member pointer");
			Assert::IsTrue(expectedIMemberPtr == actualIMemberPtr, L"Unexpected member pointer");

			std::string PrivateTestClass::* expectedJMemberPtr = PrivateTestClass::GetJPtr();
			std::string PrivateTestClass::* actualJMemberPtr = GetPrivateMemberPointer(JMemberTag());
			Assert::IsTrue(expectedJMemberPtr == actualJMemberPtr, L"Unexpected member pointer");

			float PrivateTestClass::* expectedKMemberPtr = PrivateTestClass::GetKPtr();
			float PrivateTestClass::* actualKMemberPtr = GetPrivateMemberPointer(KMemberTag());
			Assert::IsTrue(expectedKMemberPtr == actualKMemberPtr, L"Unexpected member pointer");
		}

		TEST_METHOD(GetPrivateMemberTest)
		{
			PrivateTestClass instance;

			int expectedI = 42;
			std::string expectedJ = "This is a very good test";
			float expectedK = 12.34f;
			
			int* actualIMember = GetPrivateMember(instance, IMemberTag());
			int* expectedIMember = instance.GetIMember();
			Assert::IsTrue(expectedIMember == actualIMember, L"Unexpected member pointer");

			std::string* actualJMember = GetPrivateMember(instance, JMemberTag());
			std::string* expectedJMember = instance.GetJMember();
			Assert::IsTrue(expectedJMember == actualJMember, L"Unexpected member pointer");

			float* actualKMember = GetPrivateMember(instance, KMemberTag());
			float* expectedKMember = instance.GetKMember();
			Assert::IsTrue(expectedKMember == actualKMember, L"Unexpected member pointer");
		}
	};
}
