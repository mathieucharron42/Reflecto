#include "Utils/RelationalOperators.h"

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflecto
{
	namespace Utils
	{
		namespace Test
		{
			TEST_CLASS(RelationalOperatorTest)
			{
			public:
				TEST_METHOD(Operators)
				{
					/////////////
					// Arrange
					struct Sample : RelationalOperators<Sample>
					{
						int32_t field = 0;

						bool operator<(const Sample& other) const
						{
							return field < other.field;
						}
					};

					/////////////
					// Act
					Sample s1;
					s1.field = 1;
					Sample s2;
					s2.field = 42;

					/////////////
					// Assert
					AssertOperators(s1, s2);
				}

				TEST_METHOD(OperatorsMultiple)
				{
					/////////////
					// Arrange
					struct SampleMultipleField : RelationalOperators<SampleMultipleField>
					{
						std::string field1;
						int32_t field2 = 0;

						bool operator<(const SampleMultipleField& other) const
						{
							return std::tie(field1, field2) < std::tie(other.field1, other.field2);
						}
					};

					/////////////
					// Act
					SampleMultipleField s1;
					s1.field1 = "id123";
					s1.field2 = 1;
					SampleMultipleField s2;
					s2.field1 = "id123";
					s2.field2 = 42;

					/////////////
					// Assert
					AssertOperators(s1, s2);
				}

				template<typename type>
				void AssertOperators(const type& lesser, const type& greater)
				{
					// Totologies
					Assert::IsTrue(lesser == lesser);
					Assert::IsTrue(lesser >= lesser);
					Assert::IsTrue(lesser <= lesser);
					Assert::IsFalse(lesser != lesser);
					Assert::IsFalse(lesser > lesser);
					Assert::IsFalse(lesser < lesser);

					// Inequality
					Assert::IsTrue(lesser != greater);
					Assert::IsTrue(greater != lesser);

					// Ordering
					Assert::IsTrue(lesser < greater);
					Assert::IsTrue(greater > lesser);
				}
			};
		}
	}
}
