#include "Resolver/Resolver.h"
#include "Type/TypeDescriptor.h"
#include "Type/TypeDescriptorFactory.h"
#include "Type/TypeLibrary.h"
#include "Type/TypeLibraryFactory.h"

#include <CppUnitTest.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Reflection
	{
		namespace Test
		{
			TEST_CLASS(ResolverTest)
			{
			public:
				TEST_METHOD(ResolveGet)
				{
					/////////////
					/// Arrange
					static const std::string kExpectedString = "Here is a string!";
					static const float kExpectedFloat = 42.24f;
					static const bool kExpectedBool = true;
					static const int64_t kExpectedInt64 = 666;

					class SampleClass
					{
					public:
						std::string FieldString;
						float FieldFloat = 0.f;
						bool FieldBool = false;
						int64_t FieldInt64 = 0;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleClass>("SampleClass")
						.Add<int64_t>("int64")
						.Add<float>("float")
						.Add<bool>("bool")
						.Add<std::string>("string")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<SampleClass>(typeLibrary)
						.RegisterMember(&SampleClass::FieldString, "FieldString")
						.RegisterMember(&SampleClass::FieldFloat, "FieldFloat")
						.RegisterMember(&SampleClass::FieldBool, "FieldBool")
						.RegisterMember(&SampleClass::FieldInt64, "FieldInt64")
					.Build();

					Resolver<SampleClass> resolver(descriptor);

					SampleClass instance;
					instance.FieldString = kExpectedString;
					instance.FieldFloat = kExpectedFloat;
					instance.FieldBool = kExpectedBool;
					instance.FieldInt64 = kExpectedInt64;

					/////////////
					/// Act
					const std::string* actualString = resolver.ResolveMember<std::string>(instance, "FieldString");
					const float* actualFloat = resolver.ResolveMember<float>(instance, "FieldFloat");
					const bool* actualBool = resolver.ResolveMember<bool>(instance, "FieldBool");
					const int64_t* actualInt64 = resolver.ResolveMember<int64_t>(instance, "FieldInt64");
					const int64_t* actualMissing = resolver.ResolveMember<int64_t>(instance, "FieldMissing");
					const int64_t* actualWrongType = resolver.ResolveMember<int64_t>(instance, "FieldBool");

					/////////////
					/// Assert
					Assert::IsNotNull(actualString, L"Unable to resolve member");
					Assert::AreEqual(kExpectedString, *actualString, L"Value is not expected");
					
					Assert::IsNotNull(actualFloat, L"Unable to resolve member");
					Assert::AreEqual(kExpectedFloat, *actualFloat, L"Value is not expected");
					
					Assert::IsNotNull(actualBool, L"Unable to resolve member");
					Assert::AreEqual(kExpectedBool, *actualBool, L"Value is not expected");

					Assert::IsNotNull(actualInt64, L"Unable to resolve member");
					Assert::AreEqual(kExpectedInt64, *actualInt64, L"Value is not expected");

					Assert::IsNull(actualMissing, L"Unexpected resolve of missing field");

					Assert::IsNull(actualWrongType, L"Unexpected resolve of wrong type field");
				}

				TEST_METHOD(ResolveSet)
				{
					/////////////
					/// Arrange
					static const std::string kExpectedString = "Here is a string!";
					static const float kExpectedFloat = 42.24f;
					static const bool kExpectedBool = true;
					static const int64_t kExpectedInt64 = 666;

					class SampleClass
					{
					public:
						std::string FieldString;
						float FieldFloat = 0.f;
						bool FieldBool = false;
						int64_t FieldInt64 = 0;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleClass>("SampleClass")
						.Add<int64_t>("int64")
						.Add<float>("float")
						.Add<bool>("bool")
						.Add<std::string>("string")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<SampleClass>(typeLibrary)
						.RegisterMember(&SampleClass::FieldString, "FieldString")
						.RegisterMember(&SampleClass::FieldFloat, "FieldFloat")
						.RegisterMember(&SampleClass::FieldBool, "FieldBool")
						.RegisterMember(&SampleClass::FieldInt64, "FieldInt64")
					.Build();

					Resolver<SampleClass> resolver(descriptor);

					SampleClass instance;

					///////////////
					///// Act
					std::string* actualString = resolver.ResolveMember<std::string>(instance, "FieldString");
					float* actualFloat = resolver.ResolveMember<float>(instance, "FieldFloat");
					bool* actualBool = resolver.ResolveMember<bool>(instance, "FieldBool");
					int64_t* actualInt64 = resolver.ResolveMember<int64_t>(instance, "FieldInt64");

					if (actualString)
					{
						*actualString = kExpectedString;
					}
					if (actualFloat)
					{
						*actualFloat = kExpectedFloat;
					}
					if (actualInt64)
					{
						*actualInt64 = kExpectedInt64;
					}
					if (actualBool)
					{
						*actualBool = kExpectedBool;
					}

					///////////////
					///// Assert
					Assert::IsNotNull(actualString, L"Unable to resolve member");
					Assert::AreEqual(kExpectedString, instance.FieldString, L"Value is not expected");

					Assert::IsNotNull(actualFloat, L"Unable to resolve member");
					Assert::AreEqual(kExpectedFloat, instance.FieldFloat, L"Value is not expected");

					Assert::IsNotNull(actualBool, L"Unable to resolve member");
					Assert::AreEqual(kExpectedBool, instance.FieldBool, L"Value is not expected");

					Assert::IsNotNull(actualInt64, L"Unable to resolve member");
					Assert::AreEqual(kExpectedInt64, instance.FieldInt64, L"Value is not expected");
				}

				TEST_METHOD(ResolvePublicInheritance)
				{
					/////////////
					// Arrange
					static const uint8_t kExpectedFieldBase1Value = 15;
					static const bool kExpectedFieldBase2Value = true;
					static const std::string kExpectedFieldChild1Value = "This is an average string!";
					static const float kExpectedFieldChild2Value = 42.424242f;

					class SampleBaseClass
					{
					public:
						uint8_t FieldBase1;
						bool FieldBase2;
					};

					class SampleChildClass : public SampleBaseClass
					{
					public:
						std::string FieldChild1;
						float FieldChild2 = 0.f;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleBaseClass>("SampleBaseClass")
						.Add<SampleChildClass>("SampleClass")
						.Add<float>("float")
						.Add<std::string>("string")
						.Add<uint8_t>("uint8")
						.Add<bool>("bool")
					.Build();

					const TypeDescriptor baseDescriptor = TypeDescriptorFactory<SampleBaseClass>(typeLibrary)
						.RegisterMember(&SampleBaseClass::FieldBase1, "FieldBase1")
						.RegisterMember(&SampleBaseClass::FieldBase2, "FieldBase2")
					.Build();

					const TypeDescriptor childDescriptor = TypeDescriptorFactory<SampleChildClass>(typeLibrary, &baseDescriptor)
						.RegisterMember(&SampleChildClass::FieldChild1, "FieldChild1")
						.RegisterMember(&SampleChildClass::FieldChild2, "FieldChild2")
					.Build();

					Resolver<SampleChildClass> sampleChildResolver(childDescriptor);
					Resolver<SampleBaseClass> sampleBaseResolver(baseDescriptor);

					SampleChildClass child;
					child.FieldBase1 = kExpectedFieldBase1Value;
					child.FieldBase2 = kExpectedFieldBase2Value;
					child.FieldChild1 = kExpectedFieldChild1Value;
					child.FieldChild2 = kExpectedFieldChild2Value;

					SampleChildClass parent;
					parent.FieldBase1 = kExpectedFieldBase1Value;
					parent.FieldBase2 = kExpectedFieldBase2Value;
					
					/////////////
					// Act
					const uint8_t* actualBaseField1 = sampleChildResolver.ResolveMember<uint8_t>(child, "FieldBase1");
					const uint8_t* actualBaseField1FromBase = sampleBaseResolver.ResolveMember<uint8_t>(child, "FieldBase1");
					const bool* actualBaseField2 = sampleChildResolver.ResolveMember<bool>(child, "FieldBase2");
					const bool* actualBaseField2FromBase = sampleBaseResolver.ResolveMember<bool>(child, "FieldBase2");
					const std::string* actualChildField1 = sampleChildResolver.ResolveMember<std::string>(child, "FieldChild1");
					const float* actualChildField2 = sampleChildResolver.ResolveMember<float>(child, "FieldChild2");

					
					////////////
					// Assert
					Assert::IsNotNull(actualBaseField1, L"Unable to resolve member");
					Assert::AreEqual(kExpectedFieldBase1Value, *actualBaseField1, L"Value is not expected");
					Assert::AreEqual(actualBaseField1, actualBaseField1FromBase, L"Member differs from base to parent descriptor");

					Assert::IsNotNull(actualBaseField2, L"Unable to resolve member");
					Assert::AreEqual(kExpectedFieldBase2Value, *actualBaseField2, L"Value is not expected");
					Assert::AreEqual(actualBaseField2, actualBaseField2FromBase, L"Member differs from base to parent descriptor");

					Assert::IsNotNull(actualChildField1, L"Unable to resolve member");
					Assert::AreEqual(kExpectedFieldChild1Value, *actualChildField1, L"Value is not expected");

					Assert::IsNotNull(actualChildField2, L"Unable to resolve member");
					Assert::AreEqual(kExpectedFieldChild2Value, *actualChildField2, L"Value is not expected");
				}

				TEST_METHOD(ResolveMethod)
				{
					/////////////
					// Arrange
					const int32_t expectedValueParam1 = 55;
					const float expectedValueParam2 = 3.14169f;

					class SampleClass
					{
					public:
						void MethodNoParam() { WasMethodNoParamCalled = true; }
						bool WasMethodNoParamCalled = false;

						void Method1Param(int32_t p1) { WasMethod1ParamCalled = true; Function1Param1 = p1; }
						bool WasMethod1ParamCalled = false;
						std::optional<int32_t> Function1Param1;

						void Method2Param(int32_t p1, float p2) { WasMethod2ParamCalled = true; Function2Param1 = p1; Function2Param2 = p2; }
						bool WasMethod2ParamCalled = false; 
						std::optional<int32_t> Function2Param1;
						std::optional<float> Function2Param2;

						int32_t MethodReturn(int32_t p) { WasMethodReturnCalled = true; FunctionReturnParam1 = p; return p; }
						bool WasMethodReturnCalled = false;
						std::optional<int32_t> FunctionReturnParam1;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleClass>("SampleClass")
						.Add<float>("float")
						.Add<int32_t>("int32")
						.Add<void>("void")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<SampleClass>(typeLibrary)
						.RegisterMethod(&SampleClass::MethodNoParam, "MethodNoParam")
						.RegisterMethod(&SampleClass::Method1Param, "Method1Param")
						.RegisterMethod(&SampleClass::Method2Param, "Method2Param")
						.RegisterMethod(&SampleClass::MethodReturn, "MethodReturn")
					.Build();

					Resolver<SampleClass> resolver(descriptor);

					SampleClass instance;

					/////////////
					// Act
					int32_t actualReturn = 0;
					std::function methodDoSomething = resolver.ResolveMethod("MethodNoParam", instance);
					if (methodDoSomething)
					{
						methodDoSomething();
					}

					std::function<void(int32_t)> methodDoSomethingWith1Param = resolver.ResolveMethod<void, int32_t>("Method1Param", instance);
					if (methodDoSomethingWith1Param)
					{
						methodDoSomethingWith1Param(expectedValueParam1);
					}

					std::function<void(int32_t, float)> methodDoSomethingWith2Param = resolver.ResolveMethod<void, int32_t, float>("Method2Param", instance);
					if (methodDoSomethingWith2Param)
					{
						methodDoSomethingWith2Param(expectedValueParam1, expectedValueParam2);
					}

					std::function<int32_t(int32_t)> methodReturn = resolver.ResolveMethod<int32_t, int32_t>("MethodReturn", instance);
					if (methodReturn)
					{
						actualReturn = methodReturn(expectedValueParam1);
					}

					std::function<void(int32_t)> methodMissing = resolver.ResolveMethod<void, int32_t>("Method1ParamMissing", instance);
					std::function<void(int32_t, int32_t)> methodWrongType = resolver.ResolveMethod<void, int32_t, int32_t>("Method2Param", instance);

					/////////////
					// Assert
					Assert::IsTrue(static_cast<bool>(methodDoSomething), L"Unable to resole method");
					Assert::IsTrue(instance.WasMethodNoParamCalled, L"Method was not called");
					
					Assert::IsTrue(static_cast<bool>(methodDoSomethingWith1Param), L"Unable to resole method");
					Assert::IsTrue(instance.WasMethod1ParamCalled, L"Method was not called");
					Assert::IsTrue(instance.Function1Param1.has_value(), L"Value is not as expected");
					Assert::AreEqual(expectedValueParam1, instance.Function1Param1.value(), L"Value is not as expected");

					Assert::IsTrue(static_cast<bool>(methodDoSomethingWith2Param), L"Unable to resole method");
					Assert::IsTrue(instance.WasMethod1ParamCalled, L"Method was not called");
					Assert::IsTrue(instance.Function2Param1.has_value(), L"Value is not as expected");
					Assert::AreEqual(expectedValueParam1, instance.Function2Param1.value(), L"Value is not as expected");
					Assert::IsTrue(instance.Function2Param2.has_value(), L"Value is not as expected");
					Assert::AreEqual(expectedValueParam2, instance.Function2Param2.value(), L"Value is not as expected");

					Assert::IsTrue(static_cast<bool>(methodReturn), L"Unable to resole method");
					Assert::IsTrue(instance.WasMethodReturnCalled, L"Method was not called");
					Assert::IsTrue(instance.FunctionReturnParam1.has_value(), L"Value is not as expected");
					Assert::AreEqual(expectedValueParam1, instance.FunctionReturnParam1.value(), L"Value is not as expected");
					Assert::AreEqual(expectedValueParam1, actualReturn, L"Value is not as expected");

					Assert::IsFalse(static_cast<bool>(methodMissing), L"Missing method unexpectedly resolved");
					Assert::IsFalse(static_cast<bool>(methodWrongType), L"Missing method unexpectedly resolved");
				}

				TEST_METHOD(Instantiate)
				{
					/////////////
					// Arrange
					static const int32_t kExpectedFieldValue = 424242;
					class SampleObject
					{
					public:
						int32_t Field;
					};

					const TypeLibrary typeLibrary = TypeLibraryFactory()
						.Add<SampleObject>("Potato")
						.Add<int32_t>("int32")
					.Build();

					const TypeDescriptor descriptor = TypeDescriptorFactory<SampleObject>(typeLibrary)
						.RegisterMember(&SampleObject::Field, "Field")
					.Build();

					Resolver<SampleObject> resolver(descriptor);

					/////////////
					// Act
					std::unique_ptr<SampleObject> potato = resolver.Instantiate();
					if (potato)
					{
						potato->Field = kExpectedFieldValue;
					}
					
					/////////////
					// Assert
					Assert::IsNotNull(potato.get(), L"Unable to build new instance");
					Assert::AreEqual(kExpectedFieldValue, potato->Field, L"Unexpected value on member of instantiated instance");
				}
			};
		}
	}
}