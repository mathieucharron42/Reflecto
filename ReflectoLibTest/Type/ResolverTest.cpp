#include "../TestExtensions.h"

#include "Type/Resolver.h"
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
			resolver.SetInstance(potato);
			
			const float* actualWeight = resolver.ResolveMember<float>(descriptor.MemberResursive()[0]);
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
			resolver.SetInstance(potato);

			const float* actualWeight = resolver.ResolveMember<float>("Weight");
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
				.Register(&Potato::IsBaked, "IsBacked")
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

			resolver.SetInstance(potato);

			const std::string* actualName = resolver.ResolveMember<std::string>("Name");
			Assert::IsNotNull(actualName, L"Unable to resolve member");
			if (actualName)
			{
				Assert::AreEqual(expectedName, *actualName, L"Value is not expected");
			}

			const float* actualWeight = resolver.ResolveMember<float>("Weight");
			Assert::IsNotNull(actualWeight, L"Unable to resolve member");
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}

			const bool* actualIsBacked = resolver.ResolveMember<bool>("IsBacked");
			Assert::IsNotNull(actualIsBacked, L"Unable to resolve member");
			if (actualIsBacked)
			{
				Assert::AreEqual(expectedIsBacked, *actualIsBacked, L"Value is not expected");
			}

			const int64_t* actualCookedTime = resolver.ResolveMember<int64_t>("CookedTime");
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
				.Register(&Potato::IsBaked, "IsBacked")
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

			resolver.SetInstance(potato);

			const std::string* actualName = resolver.ResolveMember<std::string>("Name");
			Assert::IsNotNull(actualName, L"Unable to resolve member");
			if (actualName)
			{
				Assert::AreEqual(expectedName, *actualName, L"Value is not expected");
			}

			const float* actualWeight = resolver.ResolveMember<float>("Weight");
			Assert::IsNotNull(actualWeight, L"Unable to resolve member");
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}

			const bool* actualIsBacked = resolver.ResolveMember<bool>("IsBacked");
			Assert::IsNotNull(actualIsBacked, L"Unable to resolve member");
			if (actualIsBacked)
			{
				Assert::AreEqual(expectedIsBacked, *actualIsBacked, L"Value is not expected");
			}

			const int64_t* actualCookedTime = resolver.ResolveMember<int64_t>("CookedTime");
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
				.Register(&Potato::IsBaked, "IsBacked")
				.Register(&Potato::CookedTime, "CookedTime")
			.Build();

			Resolver<Vegetable> resolver{ baseDescriptor };
			
			const std::string expectedName = "Backed Potato Man";
			const float expectedWeight = 42.24f;
			const bool expectedIsBacked = true;
			const int64_t expectedCookedTime = 666;

			Potato potato;
			potato.Name = expectedName;
			potato.Weight = expectedWeight;
			potato.IsBaked = expectedIsBacked;
			potato.CookedTime = expectedCookedTime;

			resolver.SetInstance(potato);

			const float* actualWeight = resolver.ResolveMember<float>("Weight");
			Assert::IsNotNull(actualWeight, L"Unable to resolve member");
			if (actualWeight)
			{
				Assert::AreEqual(expectedWeight, *actualWeight, L"Value is not expected");
			}
		}
	};
}
