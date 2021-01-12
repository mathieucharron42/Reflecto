#include "Resolver/Resolver.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"

#include "CppUnitTest.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LibTest
{
	TEST_CLASS(ResolverTest)
	{
	public:
		TEST_METHOD(ResolveSingle)
		{
			class Potato
			{
			public:
				float Weight = 0.f;
			};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>()
				.Register(&Potato::Weight, "Weight")
				.Build();

			Resolver<Potato> resolver{ descriptor };
			
			const float expectedWeight = 42.24f;

			Potato potato;
			potato.Weight = expectedWeight;
			
			const float* actualWeight = resolver.ResolveMember<float>(potato, descriptor.MemberResursive()[0]);
			Assert::IsNotNull(actualWeight, L"Unable to resolve member"); 
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}
		}

		TEST_METHOD(ResolveSingleByName)
		{
			class Potato
			{
			public:
				float Weight = 0.f;
			};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>()
				.Register(&Potato::Weight, "Weight")
				.Build();

			Resolver<Potato> resolver{ descriptor };

			const float expectedWeight = 42.24f;

			Potato potato;
			potato.Weight = expectedWeight;

			const float* actualWeight = resolver.ResolveMember<float>(potato, "Weight");
			Assert::IsNotNull(actualWeight, L"Unable to resolve member");
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}
		}

		TEST_METHOD(ResolveMultiple)
		{
			class Potato
			{
			public:
				std::string Name;
				float Weight = 0.f;
				bool IsBaked = false;
				int64_t CookedTime = 0;
			};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>()
				.Register(&Potato::Name, "Name")
				.Register(&Potato::Weight, "Weight")
				.Register(&Potato::IsBaked, "IsBaked")
				.Register(&Potato::CookedTime, "CookedTime")
			.Build();

			Resolver<Potato> resolver{ descriptor };

			const std::string expectedName = "Backed Potato Man";
			const float expectedWeight = 42.24f;
			const bool expectedIsBacked = true;
			const int64_t expectedCookedTime = 666;

			Potato potato;
			potato.Name = expectedName;
			potato.Weight = expectedWeight;
			potato.IsBaked = expectedIsBacked;
			potato.CookedTime = expectedCookedTime;

			const std::string* actualName = resolver.ResolveMember<std::string>(potato, "Name");
			Assert::IsNotNull(actualName, L"Unable to resolve member");
			if (actualName)
			{
				Assert::AreEqual(expectedName, *actualName, L"Value is not expected");
			}

			const float* actualWeight = resolver.ResolveMember<float>(potato, "Weight");
			Assert::IsNotNull(actualWeight, L"Unable to resolve member");
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}

			const bool* actualIsBacked = resolver.ResolveMember<bool>(potato, "IsBaked");
			Assert::IsNotNull(actualIsBacked, L"Unable to resolve member");
			if (actualIsBacked)
			{
				Assert::AreEqual(expectedIsBacked, *actualIsBacked, L"Value is not expected");
			}

			const int64_t* actualCookedTime = resolver.ResolveMember<int64_t>(potato, "CookedTime");
			Assert::IsNotNull(actualCookedTime, L"Unable to resolve member");
			if (actualCookedTime)
			{
				Assert::AreEqual(expectedCookedTime, *actualCookedTime, L"Value is not expected");
			}
		}

		TEST_METHOD(ResolvePublicInheritance)
		{
			class Vegetable
			{
			public:
				float Weight = 0.f;
			};

			class Potato : public Vegetable
			{
			public:
				std::string Name;
				bool IsBaked = false;
				int64_t CookedTime = 0;
			};

			const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>()
				.Register(&Vegetable::Weight, "Weight")
			.Build();

			const TypeDescriptor childDescriptor = TypeDescriptorFactory<Potato>(&baseDescriptor)
				.Register(&Potato::Name, "Name")
				.Register(&Potato::IsBaked, "IsBaked")
				.Register(&Potato::CookedTime, "CookedTime")
			.Build();

			Resolver<Potato> resolver{ childDescriptor };

			const std::string expectedName = "Backed Potato Man";
			const float expectedWeight = 42.24f;
			const bool expectedIsBacked = true;
			const int64_t expectedCookedTime = 666;

			Potato potato;
			potato.Name = expectedName;
			potato.Weight = expectedWeight;
			potato.IsBaked = expectedIsBacked;
			potato.CookedTime = expectedCookedTime;

			const std::string* actualName = resolver.ResolveMember<std::string>(potato, "Name");
			Assert::IsNotNull(actualName, L"Unable to resolve member");
			if (actualName)
			{
				Assert::AreEqual(expectedName, *actualName, L"Value is not expected");
			}

			const float* actualWeight = resolver.ResolveMember<float>(potato, "Weight");
			Assert::IsNotNull(actualWeight, L"Unable to resolve member");
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}

			const bool* actualIsBacked = resolver.ResolveMember<bool>(potato, "IsBaked");
			Assert::IsNotNull(actualIsBacked, L"Unable to resolve member");
			if (actualIsBacked)
			{
				Assert::AreEqual(expectedIsBacked, *actualIsBacked, L"Value is not expected");
			}

			const int64_t* actualCookedTime = resolver.ResolveMember<int64_t>(potato, "CookedTime");
			Assert::IsNotNull(actualCookedTime, L"Unable to resolve member");
			if (actualCookedTime)
			{
				Assert::AreEqual(expectedCookedTime, *actualCookedTime, L"Value is not expected");
			}
		}

		TEST_METHOD(ResolvePublicInheritanceWithBaseDescriptor)
		{
			class Vegetable
			{
			public:
				float Weight = 0.f;
			};

			class Potato : public Vegetable
			{
			public:
				std::string Name;
				bool IsBaked = false;
				int64_t CookedTime = 0;
			};

			const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>()
				.Register(&Vegetable::Weight, "Weight")
			.Build();

			const TypeDescriptor childDescriptor = TypeDescriptorFactory<Potato>(&baseDescriptor)
				.Register(&Potato::Name, "Name")
				.Register(&Potato::IsBaked, "IsBaked")
				.Register(&Potato::CookedTime, "CookedTime")
			.Build();

			Resolver<Vegetable> resolver{ baseDescriptor };
			
			const std::string expectedName = "Baked Potato Man";
			const float expectedWeight = 42.24f;
			const bool expectedIsBacked = true;
			const int64_t expectedCookedTime = 666;

			Potato potato;
			potato.Name = expectedName;
			potato.Weight = expectedWeight;
			potato.IsBaked = expectedIsBacked;
			potato.CookedTime = expectedCookedTime;

			const float* actualWeight = resolver.ResolveMember<float>(potato, "Weight");
			Assert::IsNotNull(actualWeight, L"Unable to resolve member");
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}
		}

		TEST_METHOD(Instantiate)
		{
			class Potato 
			{
			public:
				int Weight;
			};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>().Build();

			Resolver<Potato> resolver{ descriptor };
			
			Potato* potato = resolver.Instantiate();
			Assert::IsNotNull(potato, L"Unable to build new instance");

			const int expectedWeight = 42;
			potato->Weight = expectedWeight;
			Assert::AreEqual(expectedWeight, potato->Weight, L"Unexpected value on member of instantiated instance");

			delete potato;
		}

		TEST_METHOD(InstantiateInheritance)
		{
			class Vegetable
			{
			public:
				virtual void Func1() { }
				int Weight = 0;
			};

			class Potato : public Vegetable
			{
			public:
				virtual void Func1() override { }
				float Tastiness = 0.f;
			};

			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>().Build();

			Resolver<Potato> resolver{ descriptor };

			Potato* potato = resolver.Instantiate();
			Assert::IsNotNull(potato, L"Unable to build new instance");

			const int expectedWeight = 42;
			const float expectedTastiness = 1.f;
			potato->Weight = expectedWeight;
			potato->Tastiness = expectedTastiness;
			Assert::AreEqual(expectedWeight, potato->Weight, L"Unexpected value on member of instantiated instance");
			Assert::AreEqual(expectedTastiness, potato->Tastiness, L"Unexpected value on member of instantiated instance");

			delete potato;
		}


		TEST_METHOD(InstantiateAndResolveMember)
		{
			class Vegetable
			{
			public:
				virtual void Func1() { }
				int Weight = 0;
			};

			class Potato : public Vegetable
			{
			public:
				virtual void Func1() override { }
				float Tastiness = 0.f;
			};


			const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>()
				.Register(&Vegetable::Weight, "Weight")
			.Build();
			const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(&baseDescriptor)
				.Register(&Potato::Tastiness, "Tastiness")
			.Build();

			Resolver<Potato> resolver{ descriptor };

			Potato* potato = resolver.Instantiate();
			Assert::IsNotNull(potato, L"Unable to build new instance");

			const int expectedWeight = 42;
			const float expectedTastiness = 1.f;
			
			auto weightMember = resolver.ResolveMember<int>(*potato, "Weight");
			auto tastinessMember = resolver.ResolveMember<float>(*potato, "Tastiness");

			Assert::IsNotNull(weightMember, L"Unable to resolve member!");
			Assert::IsNotNull(weightMember, L"Unable to resolve member!");

			*weightMember = expectedWeight;
			*tastinessMember = expectedTastiness;

			Assert::AreEqual(expectedWeight, potato->Weight, L"Unexpected value on member of instantiated instance");
			Assert::AreEqual(expectedTastiness, potato->Tastiness, L"Unexpected value on member of instantiated instance");

			delete potato;
		}
	};
}