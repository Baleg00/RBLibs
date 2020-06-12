#include <CppUnitTest.h>

#include "bit/byte_concat.h"
#include "bit/byte_swap.h"
#include "bit/endian.h"
#include "bit/rotate.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace rb::tests
{
	TEST_CLASS(BIT)
	{
	public:

		TEST_METHOD(CONCAT_BE_8TO64)
		{
			uint8_t v[8]{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
			uint64_t u = rb::bit::bconcat_be<uint64_t>(v);
			
			Assert::AreEqual(0x0001020304050607ull, u);
		}

		TEST_METHOD(CONCAT_BE_32TO32)
		{
			uint32_t v[1]{ 0x00010203u };
			uint32_t u = rb::bit::bconcat_be<uint32_t>(v);
			
			Assert::AreEqual(0x00010203u, u);
		}

		TEST_METHOD(CONCAT_BE_SIGNED)
		{
			int8_t v[4]{ 0x00, 0x01, 0x02, 0x03 };
			int32_t u = rb::bit::bconcat_be<int32_t>(v);
			
			Assert::AreEqual(0x00010203, u);
		}

		TEST_METHOD(CONCAT_LE_8TO64)
		{
			uint8_t v[8]{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
			uint64_t u = rb::bit::bconcat_le<uint64_t>(v);
			
			Assert::AreEqual(0x0706050403020100ull, u);
		}

		TEST_METHOD(CONCAT_LE_32TO32)
		{
			uint32_t v[1]{ 0x00010203u };
			uint32_t u = rb::bit::bconcat_le<uint32_t>(v);
			
			Assert::AreEqual(0x00010203u, u);
		}

		TEST_METHOD(CONCAT_LE_SIGNED)
		{
			int8_t v[4]{ 0x00, 0x01, 0x02, 0x03 };
			int32_t u = rb::bit::bconcat_le<int32_t>(v);
			
			Assert::AreEqual(0x03020100, u);
		}

		TEST_METHOD(BSWAP_64)
		{
			constexpr uint64_t v = rb::bit::bswap(0x0001020304050607ull);

			Assert::AreEqual(0x0706050403020100ull, v);
		}

		TEST_METHOD(BSWAP_8)
		{
			constexpr uint8_t v = rb::bit::bswap<uint8_t>(0xAA);

			Assert::AreEqual((uint8_t)0xAA, v);
		}

		TEST_METHOD(WRITE_BE)
		{
			uint8_t d[8];
			rb::bit::write_be(d, 0x0001020304050607ull);

			Assert::AreEqual(uint8_t(0x00), d[0]);
			Assert::AreEqual(uint8_t(0x01), d[1]);
			Assert::AreEqual(uint8_t(0x02), d[2]);
			Assert::AreEqual(uint8_t(0x03), d[3]);
			Assert::AreEqual(uint8_t(0x04), d[4]);
			Assert::AreEqual(uint8_t(0x05), d[5]);
			Assert::AreEqual(uint8_t(0x06), d[6]);
			Assert::AreEqual(uint8_t(0x07), d[7]);
		}

		TEST_METHOD(WRITE_LE)
		{
			uint8_t d[8];
			rb::bit::write_le(d, 0x0001020304050607ull);

			Assert::AreEqual(uint8_t(0x07), d[0]);
			Assert::AreEqual(uint8_t(0x06), d[1]);
			Assert::AreEqual(uint8_t(0x05), d[2]);
			Assert::AreEqual(uint8_t(0x04), d[3]);
			Assert::AreEqual(uint8_t(0x03), d[4]);
			Assert::AreEqual(uint8_t(0x02), d[5]);
			Assert::AreEqual(uint8_t(0x01), d[6]);
			Assert::AreEqual(uint8_t(0x00), d[7]);
		}

		TEST_METHOD(READ_BE)
		{
			uint8_t v[8]{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
			uint64_t u = rb::bit::read_be<uint64_t>(v);
			
			Assert::AreEqual(0x0001020304050607ull, u);
		}

		TEST_METHOD(READ_LE)
		{
			uint8_t v[8]{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
			uint64_t u = rb::bit::read_le<uint64_t>(v);
			
			Assert::AreEqual(0x0706050403020100ull, u);
		}

	};
}
