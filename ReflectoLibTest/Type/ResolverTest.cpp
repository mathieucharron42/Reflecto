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

			const float expectedWeight = 42.24f;

			Potato potato;
			potato.Weight = expectedWeight;
			
			const float actualWeight = Resolver::Resolve<float>(potato, *descriptor.GetMemberByName("Weight"));
			Assert::AreEqual(expectedWeight, actualWeight, L"Value is not expected");
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

			const std::string expectedName = "Backed Potato Man";
			const float expectedWeight = 42.24f;
			const bool expectedIsBacked = true;
			const int64_t expectedCookedTime = 666;

			Potato potato;
			potato.Name = expectedName;
			potato.Weight = expectedWeight;
			potato.IsBaked = expectedIsBacked;
			potato.CookedTime = expectedCookedTime;

			const std::string actualName = Resolver::Resolve<std::string>(potato, *descriptor.GetMemberByName("Name"));
			Assert::AreEqual(expectedName, actualName, L"Value is not expected");

			const float actualWeight = Resolver::Resolve<float>(potato, *descriptor.GetMemberByName("Weight"));
			Assert::AreEqual(expectedWeight, actualWeight, L"Value is not expected");

			const bool actualIsBacked = Resolver::Resolve<bool>(potato, *descriptor.GetMemberByName("IsBacked"));
			Assert::AreEqual(expectedIsBacked, actualIsBacked, L"Value is not expected");

			const int64_t actualCookedTime = Resolver::Resolve<int64_t>(potato, *descriptor.GetMemberByName("CookedTime"));
			Assert::AreEqual(expectedCookedTime, actualCookedTime, L"Value is not expected");
		}
	};
}
