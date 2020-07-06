#include <CppUnitTest.h>

#include "math/bigint.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace rb::math;

namespace rb::tests
{
	TEST_CLASS(BIGUINT)
	{
	public:

		TEST_METHOD(ADD_128)
		{
			uint128 v = "67a7389d60ef539e65bed3893a34e08e"_u128 + "360eb0c3d1d8fba1ad0c732103394c66"_u128;
			uint128 r = "9db5e96132c84f4012cb46aa3d6e2cf4"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(SUB_128)
		{
			uint128 v = "968879be89ebc6c8dd2bced6b0070dce"_u128 - "2ae8d0de2051b2a24b6171455d88d8be"_u128;
			uint128 r = "6b9fa8e0699a142691ca5d91527e3510"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(MUL_128)
		{
			uint128 v = "3548f387df1a1925"_u128 * "3078efc52b86a075"_u128;
			uint128 r = "a16d9c3c7140048040b46e4a7029de9"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(DIV_128)
		{
			uint128 v = "b1dec9787c38aaf1f9738021bdbca89c"_u128 / "4d049d247ccf9b29e9aa33b200e37c37"_u128;
			uint128 r = "02"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(REM_128)
		{
			uint128 v = "88e0649a92bb1aadbf485bad2637a75e"_u128 % "15859e93105512c3a27e07c499bd723d"_u128;
			uint128 r = "7bead2830bcaa17f0542d118bc6f9f0"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(AND_128)
		{
			uint128 v = "23c857d06582543dab9c450ba9c5a5b1"_u128 & "16267c48064d57baa758cecc58754863"_u128;
			uint128 r = "200544004005438a318440808450021"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(OR_128)
		{
			uint128 v = "fdf573c6bc2365ae3df2cf210ea9348a"_u128 | "e8fb0cd1235e3e90ccd642a12098508e"_u128;
			uint128 r = "fdff7fd7bf7f7fbefdf6cfa12eb9748e"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(XOR_128)
		{
			uint128 v = "259a93dcfba2f0dc9e45540f406707df"_u128 ^ "2359a6461ae95b41e6ebeb916e7cf734"_u128;
			uint128 r = "6c3359ae14bab9d78aebf9e2e1bf0eb"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(LSHIFT_128)
		{
			uint128 v = "15f758ebd9b0796c64895dd46b479e97"_u128 << 40;
			uint128 r = "b0796c64895dd46b479e970000000000"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(RSHIFT_128)
		{
			uint128 v = "6c39f64c8f6b1028fedc77cd584b49c9"_u128 >> 33;
			uint128 r = "361cfb2647b588147f6e3be6"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(NOT_128)
		{
			uint128 v = ~"9e7039ad4f0c8a2e47a8c7783a577575"_u128;
			uint128 r = "618fc652b0f375d1b8573887c5a88a8a"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(EQUAL_128)
		{
			uint128 v0 = "ccd4755e518c8719aad33a4d28dad995"_u128;
			uint128 v1 = "e8823d2da5478f417a89636bbb3213b5"_u128;
			Assert::IsTrue(v0 == v0);
			Assert::IsFalse(v0 == v1);
		}

		TEST_METHOD(NOT_EQUAL_128)
		{
			uint128 v0 = "77662b707b5595f0bf7a9f94bea98102"_u128;
			uint128 v1 = "728f6146a5f928f6573536c3bfad9e7d"_u128;
			Assert::IsFalse(v0 != v0);
			Assert::IsTrue(v0 != v1);
		}

		TEST_METHOD(LESS_128)
		{
			uint128 v0 = "d4087a040e35fbf8a67235f140cc8526"_u128;
			uint128 v1 = "e08a9f770f33f45ebe236ae7aad0c41c"_u128;
			Assert::IsTrue(v0 < v1);
		}

		TEST_METHOD(LESS_EQ_128)
		{
			uint128 v0 = "6744db9bbb146b49212a0b4d2d935566"_u128;
			uint128 v1 = "84a4c6dbcc0afa5a59f73cfa52970715"_u128;
			Assert::IsTrue(v0 <= v0);
			Assert::IsTrue(v0 <= v1);
		}

		TEST_METHOD(GREATER_128)
		{
			uint128 v0 = "f2328599344565660bc5073119758e67"_u128;
			uint128 v1 = "39806edab60194a5115360c9dda4d9e7"_u128;
			Assert::IsTrue(v0 > v1);
		}

		TEST_METHOD(GREATER_EQ_128)
		{
			uint128 v0 = "96075536c5e7b74d65bfb6bf3cb33d72"_u128;
			uint128 v1 = "17cc686996c972c024855cc213849b2a"_u128;
			Assert::IsTrue(v0 >= v0);
			Assert::IsTrue(v0 >= v1);
		}

		TEST_METHOD(ZERO_128)
		{
			uint128 v0 = "68cfccbbef8e9ad7a8a15a1eb3ae8a0f"_u128;
			Assert::IsFalse(v0.is_zero());

			uint128 v1 = "0"_u128;
			Assert::IsTrue(v1.is_zero());
		}

		TEST_METHOD(SQRT_128)
		{
			uint128 v = "254a3f6bd8a408eda2332f0b2030af54"_u128.sqrt();
			uint128 r = "61b474697b361ca6"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(POW_128)
		{
			uint128 v = "ea0336de"_u128.pow("04"_u128);
			uint128 r = "b2bf04fcdfee4f84107c7bda685b8410"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(LOG_128)
		{
			uint128 v = "0b12842e1adf664fbbdd24bf638ae80e"_u128.log("64"_u128);
			uint128 r = "12"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(LOG2_128)
		{
			uint128 v = "2ac68abd5d4fbd4dcf8bd324c75c25cd"_u128.log2();
			uint128 r = "7d"_u128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(GCD_128)
		{
			uint128 v0 = "7c88ae7dacceb8adebe7c98eeabb7b84"_u128;
			uint128 v1 = "3ff232dcdd2d081cffccd25eb13ac9b2"_u128;
			uint128 r = "02"_u128;
			Assert::IsTrue(v0.gcd(v1) == r);
		}

		TEST_METHOD(LCM_128)
		{
			uint128 v0 = "3b54bd67f818bbf4"_u128;
			uint128 v1 = "04a32b51f17de60a"_u128;
			uint128 r = "8991fa3955d9a079c7d81731fc47c4"_u128;
			Assert::IsTrue(v0.lcm(v1) == r);
		}

		TEST_METHOD(CONST_ZERO_128)
		{
			Assert::IsTrue(uint128::ZERO() == "0"_u128);
			Assert::IsTrue(uint128::ZERO().is_zero());
		}

		TEST_METHOD(CONST_ONE_128)
		{
			Assert::IsTrue(uint128::ONE() == "1"_u128);
		}

		TEST_METHOD(CONST_MIN_128)
		{
			Assert::IsTrue(uint128::MIN() == uint128::ZERO());
		}

		TEST_METHOD(CONST_MAX_128)
		{
			Assert::IsTrue(uint128::MAX() == "ffffffffffffffffffffffffffffffff"_u128);
		}

	};
}
