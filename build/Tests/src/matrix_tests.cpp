#include <CppUnitTest.h>

#include "math/matrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace rb::math;

namespace rb::tests
{
	TEST_CLASS(MATRIX)
	{
	public:

		TEST_METHOD(ADD)
		{
			mat3f m1{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f m2{
				10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f,
				16.0f, 17.0f, 18.0f
			};

			mat3f add_expected{
				11.0f, 13.0f, 15.0f,
				17.0f, 19.0f, 21.0f,
				23.0f, 25.0f, 27.0f
			};

			Assert::IsTrue(add_expected == (m1 + m2));
		}

		TEST_METHOD(SUB)
		{
			mat3f m1{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f m2{
				10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f,
				16.0f, 17.0f, 18.0f
			};

			mat3f sub_expected{
				-9.0f, -9.0f, -9.0f,
				-9.0f, -9.0f, -9.0f,
				-9.0f, -9.0f, -9.0f
			};

			Assert::IsTrue(sub_expected == (m1 - m2));
		}

		TEST_METHOD(MUL_SCALAR)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f mul_expected{
				2.0f, 4.0f, 6.0f, 
				8.0f, 10.0f, 12.0f, 
				14.0f, 16.0f, 18.0f
			};

			Assert::IsTrue(mul_expected == (m * 2.0f));
		}

		TEST_METHOD(DIV_SCALAR)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f div_expected{
				0.5f, 1.0f, 1.5f,
				2.0f, 2.5f, 3.0f,
				3.5f, 4.0f, 4.5f
			};

			Assert::IsTrue(div_expected == (m / 2.0f));
		}

		TEST_METHOD(MUL_MATRIX)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f mul_expected{
				30.0f, 36.0f, 42.0f,
				66.0f, 81.0f, 96.0f,
				102.0f, 126.0f, 150.0f
			};

			Assert::IsTrue(mul_expected == (m * m));
		}

		TEST_METHOD(TRANSPOSE)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f tp_expected{
				1.0f, 4.0f, 7.0f,
				2.0f, 5.0f, 8.0f,
				3.0f, 6.0f, 9.0f
			};

			mat3f tp = m.transpose();

			Assert::IsTrue(tp_expected == tp);
		}

		TEST_METHOD(SUBMATRIX)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat2f sm_expected{
				1.0f, 2.0f,
				4.0f, 5.0f
			};

			mat2f sm = m.submatrix(2, 2);

			Assert::IsTrue(sm_expected == sm);
		}

		TEST_METHOD(SWITCH_ROWS)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f sr_expected{
				7.0f, 8.0f, 9.0f,
				4.0f, 5.0f, 6.0f,
				1.0f, 2.0f, 3.0f
			};

			mat3f sr = m.switch_rows(0, 2);

			Assert::IsTrue(sr_expected == sr);
		}

		TEST_METHOD(SWITCH_COLUMNS)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f sc_expected{
				2.0f, 1.0f, 3.0f,
				5.0f, 4.0f, 6.0f,
				8.0f, 7.0f, 9.0f
			};

			mat3f sc = m.switch_columns(0, 1);

			Assert::IsTrue(sc_expected == sc);
		}

		TEST_METHOD(MINORS)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f min_expected{
				-3.0f, -6.0f, -3.0f, 
				-6.0f, -12.0f, -6.0f, 
				-3.0f, -6.0f, -3.0f
			};

			mat3f min = m.minors();

			Assert::IsTrue(min_expected == min);
		}

		TEST_METHOD(COFACTORS)
		{
			mat3f m{
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			};

			mat3f cof_expected{
				-3.0f, 6.0f, -3.0f,
				6.0f, -12.0f, 6.0f,
				-3.0f, 6.0f, -3.0f
			};

			mat3f cof = m.cofactors();

			Assert::IsTrue(cof_expected == cof);
		}

		TEST_METHOD(INVERSE)
		{
			mat3f m{
				3.0f, 0.0f, 2.0f, 
				2.0f, 0.0f, -2.0f, 
				0.0f, 1.0f, 1.0f
			};

			mat3f inv_expected{
				0.2f, 0.2f, 0.0f,
				-0.2f, 0.3f, 1.0f,
				0.2f, -0.3f, 0.0f
			};

			mat3f inv = m.inverse();
			
			Assert::IsTrue(inv_expected == inv);
		}

	};
}
