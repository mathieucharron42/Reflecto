#include "Resolver/Resolver.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"

#include "CppUnitTest.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Type
	{
		namespace Test
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<float>("float")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary)
						.RegisterMember(&Potato::Weight, "Weight")
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<float>("float")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary)
						.RegisterMember(&Potato::Weight, "Weight")
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<int64_t>("int64")
						.Add<float>("float")
						.Add<bool>("bool")
						.Add<std::string>("string")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary)
						.RegisterMember(&Potato::Name, "Name")
						.RegisterMember(&Potato::Weight, "Weight")
						.RegisterMember(&Potato::IsBaked, "IsBaked")
						.RegisterMember(&Potato::CookedTime, "CookedTime")
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<Vegetable>("Vegetable")
						.Add<int64_t>("int64")
						.Add<float>("float")
						.Add<bool>("bool")
						.Add<std::string>("string")
					.Build();

					const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>(typeLibrary)
						.RegisterMember(&Vegetable::Weight, "Weight")
					.Build();

					const TypeDescriptor childDescriptor = TypeDescriptorFactory<Potato>(typeLibrary, &baseDescriptor)
						.RegisterMember(&Potato::Name, "Name")
						.RegisterMember(&Potato::IsBaked, "IsBaked")
						.RegisterMember(&Potato::CookedTime, "CookedTime")
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<Vegetable>("Vegetable")
						.Add<int64_t>("int64")
						.Add<float>("float")
						.Add<bool>("bool")
						.Add<std::string>("string")
					.Build();

					const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>(typeLibrary)
						.RegisterMember(&Vegetable::Weight, "Weight")
					.Build();

					const TypeDescriptor childDescriptor = TypeDescriptorFactory<Potato>(typeLibrary, &baseDescriptor)
						.RegisterMember(&Potato::Name, "Name")
						.RegisterMember(&Potato::IsBaked, "IsBaked")
						.RegisterMember(&Potato::CookedTime, "CookedTime")
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<int>("int")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary).Build();

					Resolver<Potato> resolver{ descriptor };

					std::unique_ptr<Potato> potato = std::unique_ptr<Potato>(resolver.Instantiate());
					Assert::IsNotNull(potato.get(), L"Unable to build new instance");

					const int expectedWeight = 42;
					potato->Weight = expectedWeight;
					Assert::AreEqual(expectedWeight, potato->Weight, L"Unexpected value on member of instantiated instance");
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Potato>("Potato")
						.Add<Vegetable>("Vegetable")
						.Add<int>("int")
						.Add<float>("float")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary).Build();

					Resolver<Potato> resolver{ descriptor };

					std::unique_ptr<Potato> potato = std::unique_ptr<Potato>(resolver.Instantiate());
					Assert::IsNotNull(potato.get(), L"Unable to build new instance");

					const int expectedWeight = 42;
					const float expectedTastiness = 1.f;
					potato->Weight = expectedWeight;
					potato->Tastiness = expectedTastiness;
					Assert::AreEqual(expectedWeight, potato->Weight, L"Unexpected value on member of instantiated instance");
					Assert::AreEqual(expectedTastiness, potato->Tastiness, L"Unexpected value on member of instantiated instance");
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

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<Vegetable>("Vegetable")
						.Add<Potato>("Potato")
						.Add<float>("float")
						.Add<int>("int")
					.Build();

					const TypeDescriptor baseDescriptor = TypeDescriptorFactory<Vegetable>(typeLibrary)
						.RegisterMember(&Vegetable::Weight, "Weight")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<Potato>(typeLibrary, &baseDescriptor)
						.RegisterMember(&Potato::Tastiness, "Tastiness")
					.Build();

					Resolver<Potato> resolver{ descriptor };

					std::unique_ptr<Potato> potato = std::unique_ptr<Potato>(resolver.Instantiate());
					Assert::IsNotNull(potato.get(), L"Unable to build new instance");

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
				}

				TEST_METHOD(ResolveSingleMethod)
				{
					class TestClass
					{
					public:
						void DoSomething() { WasDoSomethingCalled = true; }
						bool WasDoSomethingCalled = false;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<TestClass>("TestClass")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<TestClass>(typeLibrary)
						.RegisterMethod(&TestClass::DoSomething, "DoSomething")
					.Build();

					Resolver<TestClass> resolver{ descriptor };

					TestClass instance;
					
					auto methodDoSomething = resolver.ResolveMethod("DoSomething", instance);
					methodDoSomething();

					Assert::IsTrue(instance.WasDoSomethingCalled, L"Method was not called");
				}

				TEST_METHOD(ResolveMultipleMethod)
				{
					class TestClass
					{
					public:
						void DoSomething() { WasDoSomethingCalled = true; }
						bool WasDoSomethingCalled = false;

						void DoSomethingWith1Param(int32_t p1) { DoSomethingWith1ParamCalledParam1Value = p1; }
						int32_t DoSomethingWith1ParamCalledParam1Value = -1;

						void DoSomethingWith2Param(int32_t p1, float p2) { DoSomethingWith2ParamCalledParam1Value = p1; DoSomethingWith2ParamCalledParam2Value = p2; }
						int32_t DoSomethingWith2ParamCalledParam1Value = -1;
						float DoSomethingWith2ParamCalledParam2Value = -1;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<TestClass>("TestClass")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<TestClass>(typeLibrary)
						.RegisterMethod(&TestClass::DoSomething, "DoSomething")
						.RegisterMethod(&TestClass::DoSomethingWith1Param, "DoSomethingWith1Param")
						.RegisterMethod(&TestClass::DoSomethingWith2Param, "DoSomethingWith2Param")
					.Build();

					Resolver<TestClass> resolver{ descriptor };

					TestClass instance;

					auto methodDoSomething = resolver.ResolveMethod("DoSomething", instance);
					methodDoSomething();

					Assert::IsTrue(instance.WasDoSomethingCalled, L"Method was not called");

					const int32_t expectedValueParam1 = 55;
					auto methodDoSomethingWith1Param = resolver.ResolveMethod<int32_t>("DoSomethingWith1Param", instance);
					methodDoSomethingWith1Param(expectedValueParam1);
					Assert::IsTrue(instance.DoSomethingWith1ParamCalledParam1Value == expectedValueParam1, L"Method was not called with expected value");

					const float expectedValueParam2 = 3.14169;
					auto methodDoSomethingWith2Param = resolver.ResolveMethod<int32_t, float>("DoSomethingWith2Param", instance);
					methodDoSomethingWith2Param(expectedValueParam1, expectedValueParam2);
					Assert::IsTrue(instance.DoSomethingWith1ParamCalledParam1Value == expectedValueParam1, L"Method was not called with expected value");
					Assert::IsTrue(instance.DoSomethingWith2ParamCalledParam2Value == expectedValueParam2, L"Method was not called with expected value");
				}
			};
		}
	}
}