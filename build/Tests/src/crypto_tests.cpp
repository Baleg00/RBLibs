#include <CppUnitTest.h>

#include "crypto/md5.h"
#include "crypto/ripemd.h"
#include "crypto/sha1.h"
#include "crypto/sha2.h"
#include "crypto/keccak.h"
#include "crypto/hmac.h"
#include "crypto/bases.h"
#include "crypto/aes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace rb::crypto;

namespace rb::tests
{
	TEST_CLASS(CRYPTO)
	{
	public:

		TEST_METHOD(MD5)
		{
			md5 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "e4d909c290d0fb1ca068ffaddf22cbd0");
		}

		TEST_METHOD(RIPEMD160)
		{
			ripemd160 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "fc850169b1f2ce72e3f8aa0aeb5ca87d6f8519c6");
		}

		TEST_METHOD(SHA1)
		{
			sha1 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "408d94384216f890ff7a0c3528e8bed1e0b01621");
		}

		TEST_METHOD(SHA2_224)
		{
			sha2_224 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "619cba8e8e05826e9b8c519c0a5c68f4fb653e8a3d8aa04bb2c8cd4c");
		}

		TEST_METHOD(SHA2_256)
		{
			sha2_256 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "ef537f25c895bfa782526529a9b63d97aa631564d5d789c2b765448c8635fb6c");
		}

		TEST_METHOD(SHA2_384)
		{
			sha2_384 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "ed892481d8272ca6df370bf706e4d7bc1b5739fa2177aae6c50e946678718fc67a7af2819a021c2fc34e91bdb63409d7");
		}

		TEST_METHOD(SHA2_512)
		{
			sha2_512 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "91ea1245f20d46ae9a037a989f54f1f790f0a47607eeb8a14d12890cea77a1bbc6c7ed9cf205e67b7f2b8fd4c7dfd3a7a8617e45f3c463d481c7e586c39ac1ed");
		}

		TEST_METHOD(SHA3_224)
		{
			sha3_224 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "2d0708903833afabdd232a20201176e8b58c5be8a6fe74265ac54db0");
		}

		TEST_METHOD(SHA3_256)
		{
			sha3_256 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "a80f839cd4f83f6c3dafc87feae470045e4eb0d366397d5c6ce34ba1739f734d");
		}

		TEST_METHOD(SHA3_384)
		{
			sha3_384 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "1a34d81695b622df178bc74df7124fe12fac0f64ba5250b78b99c1273d4b080168e10652894ecad5f1f4d5b965437fb9");
		}

		TEST_METHOD(SHA3_512)
		{
			sha3_512 hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "18f4f4bd419603f95538837003d9d254c26c23765565162247483f65c50303597bc9ce4d289f21d1c2f1f458828e33dc442100331b35e7eb031b5d38ba6460f8");
		}

		TEST_METHOD(SHAKE128_256)
		{
			shake128<256> hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "634069e6b13c3af64c57f05babf5911b6acf1d309b9624fc92b0c0bd9f27f538");
		}

		TEST_METHOD(SHAKE256_512)
		{
			shake256<512> hash;
			hash << "The quick brown fox jumps over the lazy dog.";
			
			Assert::IsTrue(hash.hex_digest() == "bd225bfc8b255f3036f0c8866010ed0053b5163a3cae111e723c0c8e704eca4e5d0f1e2a2fa18c8a219de6b88d5917ff5dd75b5fb345e7409a3b333b508a65fb");
		}

		TEST_METHOD(HMAC_SHA2_256)
		{
			hmac<sha2_256> hash;
			hash << "The quick brown fox jumps over the lazy dog.";

			Assert::IsTrue(hash.hex_digest("key", 3) == "e98139c39d76eb80d8db982552b44b251b94f312987f91ee72d12ef673caa813");
		}

		TEST_METHOD(BASE16_TO)
		{
			Assert::IsTrue(base16::encode("foobar", 6) == "666F6F626172");
		}

		TEST_METHOD(BASE16_FROM)
		{
			char result[7]{ '\0' };
			base16::decode("666F6F626172", result, 7);
			
			Assert::IsTrue(std::memcmp(result, "foobar", 7) == 0);
		}

		TEST_METHOD(BASE16_LITERAL)
		{
			Assert::IsTrue("foobar"_base16 == "666F6F626172");
		}

		TEST_METHOD(BASE32_TO)
		{
			Assert::IsTrue(base32::encode("foobar", 6) == "MZXW6YTBOI======");
		}

		TEST_METHOD(BASE32_FROM)
		{
			char result[7]{ '\0' };
			base32::decode("MZXW6YTBOI======", result, 7);

			Assert::IsTrue(std::memcmp(result, "foobar", 7) == 0);
		}

		TEST_METHOD(BASE32_LITERAL)
		{
			Assert::IsTrue("foobar"_base32 == "MZXW6YTBOI======");
		}

		TEST_METHOD(BASE32HEX_TO)
		{
			Assert::IsTrue(base32hex::encode("foobar", 6) == "CPNMUOJ1E8======");
		}

		TEST_METHOD(BASE32HEX_FROM)
		{
			char result[7]{ '\0' };
			base32hex::decode("CPNMUOJ1E8======", result, 7);

			Assert::IsTrue(std::memcmp(result, "foobar", 7) == 0);
		}

		TEST_METHOD(BASE32HEX_LITERAL)
		{
			Assert::IsTrue("foobar"_base32hex == "CPNMUOJ1E8======");
		}

		TEST_METHOD(BASE64_TO)
		{
			Assert::IsTrue(base64::encode("foobar", 6) == "Zm9vYmFy");
		}

		TEST_METHOD(BASE64_FROM)
		{
			char result[7]{ '\0' };
			base64::decode("Zm9vYmFy", result, 7);

			Assert::IsTrue(std::memcmp(result, "foobar", 7) == 0);
		}

		TEST_METHOD(BASE64_LITERAL)
		{
			Assert::IsTrue("foobar"_base64 == "Zm9vYmFy");
		}

		TEST_METHOD(BASE64URL_TO)
		{
			Assert::IsTrue(base64::encode("foobar", 6) == "Zm9vYmFy");
		}

		TEST_METHOD(BASE64URL_FROM)
		{
			char result[7]{ '\0' };
			base64::decode("Zm9vYmFy", result, 7);

			Assert::IsTrue(std::memcmp(result, "foobar", 7) == 0);
		}

		TEST_METHOD(BASE64URL_LITERAL)
		{
			Assert::IsTrue("foobar"_base64url == "Zm9vYmFy");
		}

		TEST_METHOD(AES_128_ENC)
		{
			uint8_t data[16] = {
				0x00, 0x11, 0x22, 0x33,
				0x44, 0x55, 0x66, 0x77,
				0x88, 0x99, 0xaa, 0xbb,
				0xcc, 0xdd, 0xee, 0xff
			};

			uint8_t key[16] = {
				0x00, 0x01, 0x02, 0x03,
				0x04, 0x05, 0x06, 0x07,
				0x08, 0x09, 0x0a, 0x0b,
				0x0c, 0x0d, 0x0e, 0x0f
			};

			aes_128 cipher;
			cipher.write(data, 16);

			Assert::IsTrue(cipher.hex_encrypt(key, 16) == "69c4e0d86a7b0430d8cdb78070b4c55a");
		}

		TEST_METHOD(AES_128_DEC)
		{
			uint8_t data[16] = {
				0x69, 0xc4, 0xe0, 0xd8, 
				0x6a, 0x7b, 0x04, 0x30, 
				0xd8, 0xcd, 0xb7, 0x80, 
				0x70, 0xb4, 0xc5, 0x5a
			};

			uint8_t key[16] = {
				0x00, 0x01, 0x02, 0x03,
				0x04, 0x05, 0x06, 0x07,
				0x08, 0x09, 0x0a, 0x0b,
				0x0c, 0x0d, 0x0e, 0x0f
			};

			aes_128 cipher;
			cipher.write(data, 16);

			Assert::IsTrue(cipher.hex_decrypt(key, 16) == "00112233445566778899aabbccddeeff");
		}

		TEST_METHOD(AES_192_ENC)
		{
			uint8_t data[16] = {
				0x00, 0x11, 0x22, 0x33,
				0x44, 0x55, 0x66, 0x77,
				0x88, 0x99, 0xaa, 0xbb,
				0xcc, 0xdd, 0xee, 0xff
			};

			uint8_t key[24] = {
				0x00, 0x01, 0x02, 0x03, 
				0x04, 0x05, 0x06, 0x07, 
				0x08, 0x09, 0x0a, 0x0b, 
				0x0c, 0x0d, 0x0e, 0x0f, 
				0x10, 0x11, 0x12, 0x13, 
				0x14, 0x15, 0x16, 0x17
			};

			aes_192 cipher;
			cipher.write(data, 16);

			Assert::IsTrue(cipher.hex_encrypt(key, 24) == "dda97ca4864cdfe06eaf70a0ec0d7191");
		}

		TEST_METHOD(AES_192_DEC)
		{
			uint8_t data[16] = {
				0xdd, 0xa9, 0x7c, 0xa4, 
				0x86, 0x4c, 0xdf, 0xe0, 
				0x6e, 0xaf, 0x70, 0xa0, 
				0xec, 0x0d, 0x71, 0x91
			};

			uint8_t key[24] = {
				0x00, 0x01, 0x02, 0x03,
				0x04, 0x05, 0x06, 0x07,
				0x08, 0x09, 0x0a, 0x0b,
				0x0c, 0x0d, 0x0e, 0x0f,
				0x10, 0x11, 0x12, 0x13,
				0x14, 0x15, 0x16, 0x17
			};

			aes_192 cipher;
			cipher.write(data, 16);

			Assert::IsTrue(cipher.hex_decrypt(key, 24) == "00112233445566778899aabbccddeeff");
		}

		TEST_METHOD(AES_256_ENC)
		{
			uint8_t data[16] = {
				0x00, 0x11, 0x22, 0x33,
				0x44, 0x55, 0x66, 0x77,
				0x88, 0x99, 0xaa, 0xbb,
				0xcc, 0xdd, 0xee, 0xff
			};

			uint8_t key[32] = {
				0x00, 0x01, 0x02, 0x03, 
				0x04, 0x05, 0x06, 0x07, 
				0x08, 0x09, 0x0a, 0x0b, 
				0x0c, 0x0d, 0x0e, 0x0f, 
				0x10, 0x11, 0x12, 0x13, 
				0x14, 0x15, 0x16, 0x17, 
				0x18, 0x19, 0x1a, 0x1b, 
				0x1c, 0x1d, 0x1e, 0x1f
			};

			aes_256 cipher;
			cipher.write(data, 16);

			Assert::IsTrue(cipher.hex_encrypt(key, 32) == "8ea2b7ca516745bfeafc49904b496089");
		}

		TEST_METHOD(AES_256_DEC)
		{
			uint8_t data[16] = {
				0x8e, 0xa2, 0xb7, 0xca, 
				0x51, 0x67, 0x45, 0xbf, 
				0xea, 0xfc, 0x49, 0x90, 
				0x4b, 0x49, 0x60, 0x89
			};

			uint8_t key[32] = {
				0x00, 0x01, 0x02, 0x03,
				0x04, 0x05, 0x06, 0x07,
				0x08, 0x09, 0x0a, 0x0b,
				0x0c, 0x0d, 0x0e, 0x0f,
				0x10, 0x11, 0x12, 0x13,
				0x14, 0x15, 0x16, 0x17,
				0x18, 0x19, 0x1a, 0x1b,
				0x1c, 0x1d, 0x1e, 0x1f
			};

			aes_256 cipher;
			cipher.write(data, 16);

			Assert::IsTrue(cipher.hex_decrypt(key, 32) == "00112233445566778899aabbccddeeff");
		}

	};
}
