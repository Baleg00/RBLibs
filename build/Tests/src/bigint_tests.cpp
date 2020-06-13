#include <CppUnitTest.h>

#include "math/bigint.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace rb::math;

namespace rb::tests
{
	TEST_CLASS(BIGINT)
	{
	public:

		TEST_METHOD(ADD_128)
		{
			int128 v = "67a7389d60ef539e65bed3893a34e08e"_i128 + "360eb0c3d1d8fba1ad0c732103394c66"_i128;
			int128 r = "9db5e96132c84f4012cb46aa3d6e2cf4"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(SUB_128)
		{
			int128 v = "968879be89ebc6c8dd2bced6b0070dce"_i128 - "2ae8d0de2051b2a24b6171455d88d8be"_i128;
			int128 r = "6b9fa8e0699a142691ca5d91527e3510"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(MUL_128)
		{
			int128 v0 = "3548f387df1a1925"_i128 * "3078efc52b86a075"_i128;
			int128 r0 = "a16d9c3c7140048040b46e4a7029de9"_i128;
			Assert::IsTrue(v0 == r0);

			int128 v1 = -"3548f387df1a1925"_i128 * "3078efc52b86a075"_i128;
			int128 r1 = -"a16d9c3c7140048040b46e4a7029de9"_i128;
			Assert::IsTrue(v1 == r1);

			int128 v2 = -"3548f387df1a1925"_i128 * -"3078efc52b86a075"_i128;
			int128 r2 = "a16d9c3c7140048040b46e4a7029de9"_i128;
			Assert::IsTrue(v2 == r2);
		}

		TEST_METHOD(DIV_128)
		{
			int128 v0 = "71dec9787c38aaf1f9738021bdbca89c"_i128 / "4d049d247ccf9b29e9aa33b200e37c37"_i128;
			int128 r0 = "01"_i128;
			Assert::IsTrue(v0 == r0);

			int128 v1 = -"71dec9787c38aaf1f9738021bdbca89c"_i128 / "4d049d247ccf9b29e9aa33b200e37c37"_i128;
			int128 r1 = -"01"_i128;
			Assert::IsTrue(v1 == r1);

			int128 v2 = -"71dec9787c38aaf1f9738021bdbca89c"_i128 / -"4d049d247ccf9b29e9aa33b200e37c37"_i128;
			int128 r2 = "01"_i128;
			Assert::IsTrue(v2 == r2);
		}

		TEST_METHOD(REM_128)
		{
			int128 v0 = "68e0649a92bb1aadbf485bad2637a75e"_i128 % "15859e93105512c3a27e07c499bd723d"_i128;
			int128 r0 = "12c9ea4e5166cf9f35503c9abf41de6a"_i128;
			Assert::IsTrue(v0 == r0);

			int128 v1 = -"68e0649a92bb1aadbf485bad2637a75e"_i128 % "15859e93105512c3a27e07c499bd723d"_i128;
			int128 r1 = "2bbb444beee43246d2dcb29da7b93d3"_i128;
			Assert::IsTrue(v1 == r1);

			int128 v2 = "68e0649a92bb1aadbf485bad2637a75e"_i128 % -"15859e93105512c3a27e07c499bd723d"_i128;
			int128 r2 = -"2bbb444beee43246d2dcb29da7b93d3"_i128;
			Assert::IsTrue(v2 == r2);

			int128 v3 = -"68e0649a92bb1aadbf485bad2637a75e"_i128 % -"15859e93105512c3a27e07c499bd723d"_i128;
			int128 r3 = -"12c9ea4e5166cf9f35503c9abf41de6a"_i128;
			Assert::IsTrue(v3 == r3);
		}

		TEST_METHOD(SQRT_128)
		{
			int128 v = "254a3f6bd8a408eda2332f0b2030af54"_i128.sqrt();
			int128 r = "61b474697b361ca6"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(POW_128)
		{
			int128 v = "ea0336de"_i128.pow("04"_i128);
			int128 r = "b2bf04fcdfee4f84107c7bda685b8410"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(AND_128)
		{
			int128 v = "23c857d06582543dab9c450ba9c5a5b1"_i128 & "16267c48064d57baa758cecc58754863"_i128;
			int128 r = "200544004005438a318440808450021"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(OR_128)
		{
			int128 v = "fdf573c6bc2365ae3df2cf210ea9348a"_i128 | "e8fb0cd1235e3e90ccd642a12098508e"_i128;
			int128 r = "fdff7fd7bf7f7fbefdf6cfa12eb9748e"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(XOR_128)
		{
			int128 v = "259a93dcfba2f0dc9e45540f406707df"_i128 ^ "2359a6461ae95b41e6ebeb916e7cf734"_i128;
			int128 r = "6c3359ae14bab9d78aebf9e2e1bf0eb"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(NOT_128)
		{
			int128 v = ~"9e7039ad4f0c8a2e47a8c7783a577575"_i128;
			int128 r = "618fc652b0f375d1b8573887c5a88a8a"_i128;
			Assert::IsTrue(v == r);
		}

		TEST_METHOD(EQUAL_128)
		{
			int128 v0 = "ccd4755e518c8719aad33a4d28dad995"_i128;
			int128 v1 = "e8823d2da5478f417a89636bbb3213b5"_i128;
			Assert::IsTrue(v0 == v0);
			Assert::IsFalse(v0 == v1);
		}

		TEST_METHOD(NOT_EQUAL_128)
		{
			int128 v0 = "77662b707b5595f0bf7a9f94bea98102"_i128;
			int128 v1 = "728f6146a5f928f6573536c3bfad9e7d"_i128;
			Assert::IsFalse(v0 != v0);
			Assert::IsTrue(v0 != v1);
		}

		TEST_METHOD(LESS_128)
		{
			int128 v0 = "d4087a040e35fbf8a67235f140cc8526"_i128;
			int128 v1 = "e08a9f770f33f45ebe236ae7aad0c41c"_i128;
			Assert::IsTrue(v0 < v1);
		}

		TEST_METHOD(LESS_EQ_128)
		{
			int128 v0 = "6744db9bbb146b49212a0b4d2d935566"_i128;
			int128 v1 = "74a4c6dbcc0afa5a59f73cfa52970715"_i128;
			Assert::IsTrue(v0 <= v0);
			Assert::IsTrue(v0 <= v1);
		}

		TEST_METHOD(GREATER_128)
		{
			int128 v0 = "42328599344565660bc5073119758e67"_i128;
			int128 v1 = "39806edab60194a5115360c9dda4d9e7"_i128;
			Assert::IsTrue(v0 > v1);
		}

		TEST_METHOD(GREATER_EQ_128)
		{
			int128 v0 = "76075536c5e7b74d65bfb6bf3cb33d72"_i128;
			int128 v1 = "17cc686996c972c024855cc213849b2a"_i128;
			Assert::IsTrue(v0 >= v0);
			Assert::IsTrue(v0 >= v1);
		}

	};
}
