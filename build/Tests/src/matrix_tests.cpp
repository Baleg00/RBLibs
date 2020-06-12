#include <CppUnitTest.h>

#include "math/matrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace rb::tests
{
	TEST_CLASS(MATRIX)
	{
	public:

		TEST_METHOD(INVERSE)
		{
			rb::math::mat3f m{
				3.0f, 0.0f, 2.0f, 
				2.0f, 0.0f, -2.0f, 
				0.0f, 1.0f, 1.0f
			};

			rb::math::mat3f inv_expected{
				0.2f, 0.2f, 0.0f,
				-0.2f, 0.3f, 1.0f,
				0.2f, -0.3f, 0.0f
			};

			rb::math::mat3f inv = m.inverse();
			
			Assert::IsTrue(inv_expected == inv);
		}

	};
}
