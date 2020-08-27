#pragma once

#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "bit/endian.h"
#include "bit/rotate.h"

#include "crypto/basic_cipher.h"

namespace rb::crypto
{
	static const uint8_t S_BOX[16][16]{
		{ 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76 }, 
		{ 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0 }, 
		{ 0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15 }, 
		{ 0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75 }, 
		{ 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84 }, 
		{ 0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF }, 
		{ 0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8 }, 
		{ 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2 }, 
		{ 0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73 }, 
		{ 0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB }, 
		{ 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79 }, 
		{ 0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08 }, 
		{ 0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A }, 
		{ 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E }, 
		{ 0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF }, 
		{ 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 }
	};

	static const uint8_t S_BOX_INV[16][16]{
		{ 0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB }, 
		{ 0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB }, 
		{ 0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E }, 
		{ 0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25 }, 
		{ 0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92 }, 
		{ 0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84 }, 
		{ 0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06 }, 
		{ 0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B }, 
		{ 0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73 }, 
		{ 0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E }, 
		{ 0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B }, 
		{ 0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4 }, 
		{ 0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F }, 
		{ 0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF }, 
		{ 0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61 }, 
		{ 0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D }
	};

	template<cipher_mode M, size_t K, size_t B, size_t R>
	class aes : public basic_cipher
	{
	public:
		static constexpr cipher_mode MODE = M;

		static constexpr size_t KEY_SIZE = K * 32;
		static constexpr size_t BLOCK_SIZE = B * 4 * 8;
		static constexpr size_t ROUNDS = R;

		static_assert(M == cipher_mode::ECB || 
			M == cipher_mode::CBC || 
			M == cipher_mode::PCBC, "cipher mode is not implemented");

	public:
		aes() noexcept
		{
			clear();
		}

		aes& write(const void* data, size_t size) noexcept override
		{
			std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
			return *this;
		}

		size_t encrypt(const void* key, size_t size, void* dest) noexcept override
		{
			const size_t digest_size = m_data.size();
			uint32_t w[B * (R + 1)];
			uint8_t real_key[K * 4]{ 0 };

			std::memcpy(real_key, key, std::min(size, K * 4));
			key_expansion(real_key, w);

			pad_data();

			if constexpr (M == cipher_mode::ECB)
			{
				for (size_t i = 0; i < m_data.size(); i += B * 4)
					cipher(m_data.data() + i, reinterpret_cast<uint8_t*>(dest) + i, w);
			}
			else if constexpr (M == cipher_mode::CBC)
			{
				uint8_t v[B * 4]{ 0 };

				for (size_t i = 0; i < digest_size; i += B * 4)
				{
					uint8_t in[B * 4];
					std::memcpy(in, m_data.data() + i, B * 4);

					for (size_t j = 0; j < B * 4; j++)
						in[j] ^= v[j];

					cipher(in, v, w);

					std::memcpy(reinterpret_cast<uint8_t*>(dest) + i, v, B * 4);
				}
			}
			else if constexpr (M == cipher_mode::PCBC)
			{
				uint8_t v[B * 4]{ 0 };

				for (size_t i = 0; i < digest_size; i += B * 4)
				{
					uint8_t in[B * 4];
					std::memcpy(in, m_data.data() + i, B * 4);

					for (size_t j = 0; j < B * 4; j++)
						in[j] ^= v[j];

					cipher(in, v, w);

					std::memcpy(reinterpret_cast<uint8_t*>(dest) + i, v, B * 4);

					for (size_t j = 0; j < B * 4; j++)
						v[j] ^= m_data[i + j];
				}
			}

			return m_data.size();
		}

		size_t decrypt(const void* key, size_t size, void* dest) noexcept override
		{
			const size_t digest_size = m_data.size();
			uint32_t w[B * (R + 1)];
			uint8_t real_key[K * 4]{ 0 };

			std::memcpy(real_key, key, std::min(size, K * 4));
			key_expansion(real_key, w);

			if constexpr (M == cipher_mode::ECB)
			{
				for (size_t i = 0; i < digest_size; i += B * 4)
					inv_cipher(m_data.data() + i, reinterpret_cast<uint8_t*>(dest) + i, w);
			}
			else if constexpr (M == cipher_mode::CBC)
			{
				uint8_t v[B * 4]{ 0 };

				for (size_t i = 0; i < digest_size; i += B * 4)
				{
					uint8_t out[B * 4];
					inv_cipher(m_data.data() + i, out, w);

					for (size_t j = 0; j < B * 4; j++)
						out[j] ^= v[j];

					std::memcpy(reinterpret_cast<uint8_t*>(dest) + i, out, B * 4);
					std::memcpy(v, m_data.data() + i, B * 4);
				}
			}
			else if constexpr (M == cipher_mode::PCBC)
			{
				uint8_t v[B * 4]{ 0 };

				for (size_t i = 0; i < digest_size; i += B * 4)
				{
					uint8_t out[B * 4];
					inv_cipher(m_data.data() + i, out, w);

					for (size_t j = 0; j < B * 4; j++)
						out[j] ^= v[j];

					std::memcpy(reinterpret_cast<uint8_t*>(dest) + i, out, B * 4);

					for (size_t j = 0; j < B * 4; j++)
						v[j] = out[j] ^ m_data[i + j];
				}
			}

			return digest_size;
		}

		[[nodiscard]] std::string hex_encrypt(const void* key, size_t size) noexcept override
		{
			const size_t digest_size = padded_size();
			uint8_t* digest = new uint8_t[digest_size];
			encrypt(key, size, digest);

			std::stringstream hex_digest;

			for (size_t i = 0; i < digest_size; i++)
				hex_digest << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(digest[i]);

			delete[] digest;

			return hex_digest.str();
		}

		[[nodiscard]] std::string hex_decrypt(const void* key, size_t size) noexcept override
		{
			const size_t digest_size = m_data.size();
			uint8_t* digest = new uint8_t[digest_size];
			decrypt(key, size, digest);

			std::stringstream hex_digest;

			for (size_t i = 0; i < digest_size; i++)
				hex_digest << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(digest[i]);

			delete[] digest;

			return hex_digest.str();
		}

		void clear() noexcept override
		{
			m_data.clear();
			std::memset(m_state, 0, B * 4);
		}

		aes& operator<<(char value) noexcept override
		{
			m_data.push_back(value);
			return *this;
		}

		aes& operator<<(unsigned char value) noexcept override
		{
			m_data.push_back(value);
			return *this;
		}

		aes& operator<<(short value) noexcept override
		{
			uint8_t bytes[sizeof(short)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(short));
			return *this;
		}

		aes& operator<<(unsigned short value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned short)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned short));
			return *this;
		}

		aes& operator<<(int value) noexcept override
		{
			uint8_t bytes[sizeof(int)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(int));
			return *this;
		}

		aes& operator<<(unsigned int value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned int)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned int));
			return *this;
		}

		aes& operator<<(long value) noexcept override
		{
			uint8_t bytes[sizeof(long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(long));
			return *this;
		}

		aes& operator<<(unsigned long value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned long));
			return *this;
		}

		aes& operator<<(long long value) noexcept override
		{
			uint8_t bytes[sizeof(long long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(long long));
			return *this;
		}

		aes& operator<<(unsigned long long value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned long long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned long long));
			return *this;
		}

		aes& operator<<(const std::string& str) noexcept override
		{
			write(str.data(), str.size());
			return *this;
		}

	private:
		std::vector<uint8_t> m_data;
		uint8_t m_state[4][B];

		size_t padded_size()
		{
			size_t s = m_data.size();

			while (s % (B * 4) != 0)
				s++;

			return s;
		}

		void pad_data()
		{
			m_data.insert(m_data.end(), padded_size() - m_data.size(), 0);
		}

		void sub_bytes()
		{
			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < B; j++)
				{
					uint8_t r = (m_state[i][j] >> 4) & 0xF;
					uint8_t c = m_state[i][j] & 0xF;

					m_state[i][j] = S_BOX[r][c];
				}
			}
		}

		void inv_sub_bytes()
		{
			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < B; j++)
				{
					uint8_t r = (m_state[i][j] >> 4) & 0xF;
					uint8_t c = m_state[i][j] & 0xF;

					m_state[i][j] = S_BOX_INV[r][c];
				}
			}
		}

		void shift_rows()
		{
			for (size_t i = 1; i < 4; i++)
			{
				uint8_t temp[B];
				std::memcpy(temp, m_state[i], B);

				for (size_t j = 0; j < B; j++)
					m_state[i][j] = temp[(j + i) % B];
			}
		}

		void inv_shift_rows()
		{
			for (size_t i = 1; i < 4; i++)
			{
				uint8_t temp[B];
				std::memcpy(temp, m_state[i], B);

				for (size_t j = 0; j < B; j++)
					m_state[i][j] = temp[(B - i + j) % B];
			}
		}

		uint8_t xtime(uint8_t b)
		{
			if (b & 0x80)
			{
				b <<= 1;
				b ^= 0x1b;
			}
			else
				b <<= 1;

			return b;
		}

		uint8_t mul_bytes(uint8_t a, uint8_t b)
		{
			uint8_t result = 0;

			for (int i = 0; i < 8; i++)
			{
				if (b & 1)
				{
					uint8_t temp = a;

					for (int j = 0; j < i; j++)
						temp = xtime(temp);

					result ^= temp;
				}

				b >>= 1;
			}

			return result;
		}

		void mix_columns()
		{
			for (size_t i = 0; i < B; i++)
			{
				uint8_t temp[4] = { m_state[0][i], m_state[1][i], m_state[2][i], m_state[3][i] };

				m_state[0][i] = mul_bytes(2, temp[0]) ^ mul_bytes(3, temp[1]) ^ temp[2] ^ temp[3];
				m_state[1][i] = temp[0] ^ mul_bytes(2, temp[1]) ^ mul_bytes(3, temp[2]) ^ temp[3];
				m_state[2][i] = temp[0] ^ temp[1] ^ mul_bytes(2, temp[2]) ^ mul_bytes(3, temp[3]);
				m_state[3][i] = mul_bytes(3, temp[0]) ^ temp[1] ^ temp[2] ^ mul_bytes(2, temp[3]);
			}
		}

		void inv_mix_columns()
		{
			for (size_t i = 0; i < B; i++)
			{
				uint8_t temp[4] = { m_state[0][i], m_state[1][i], m_state[2][i], m_state[3][i] };

				m_state[0][i] = mul_bytes(14, temp[0]) ^ mul_bytes(11, temp[1]) ^ mul_bytes(13, temp[2]) ^ mul_bytes(9, temp[3]);
				m_state[1][i] = mul_bytes(9, temp[0]) ^ mul_bytes(14, temp[1]) ^ mul_bytes(11, temp[2]) ^ mul_bytes(13, temp[3]);
				m_state[2][i] = mul_bytes(13, temp[0]) ^ mul_bytes(9, temp[1]) ^ mul_bytes(14, temp[2]) ^ mul_bytes(11, temp[3]);
				m_state[3][i] = mul_bytes(11, temp[0]) ^ mul_bytes(13, temp[1]) ^ mul_bytes(9, temp[2]) ^ mul_bytes(14, temp[3]);
			}
		}

		void add_round_key(uint32_t keys[B])
		{
			for (size_t i = 0; i < B; i++)
			{
				m_state[0][i] ^= (keys[i] >> 24) & 0xFF;
				m_state[1][i] ^= (keys[i] >> 16) & 0xFF;
				m_state[2][i] ^= (keys[i] >> 8) & 0xFF;
				m_state[3][i] ^= keys[i] & 0xFF;
			}
		}

		uint32_t sub_word(uint32_t w)
		{
			uint32_t result = 0;

			result |= S_BOX[(w >> 28) & 0xF][(w >> 24) & 0xF] << 24;
			result |= S_BOX[(w >> 20) & 0xF][(w >> 16) & 0xF] << 16;
			result |= S_BOX[(w >> 12) & 0xF][(w >> 8) & 0xF] << 8;
			result |= S_BOX[(w >> 4) & 0xF][w & 0xF];

			return result;
		}

		uint32_t rot_word(uint32_t w)
		{
			return (w << 8) | (w >> 24);
		}

		void key_expansion(const uint8_t key[K * 4], uint32_t w[B * (R + 1)])
		{
			uint32_t temp;
			size_t i = 0;

			while (i < K)
			{
				w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8) | key[4 * i + 3];
				i++;
			}

			while (i < B * (R + 1))
			{
				temp = w[i - 1];

				if (i % K == 0)
				{
					temp = rot_word(temp);
					temp = sub_word(temp);
					uint32_t rcon = (((1 << (i / K - 1)) % 229) << 24);
					temp ^= rcon;
				}
				else if (K > 6 && i % K == 4)
					temp = sub_word(temp);

				w[i] = w[i - K] ^ temp;
				i++;
			}
		}

		void cipher(const uint8_t in[B * 4], uint8_t out[B * 4], uint32_t w[B * (R + 1)])
		{
			for (size_t i = 0; i < 4; i++)
				for (size_t j = 0; j < B; j++)
					m_state[i][j] = in[j * 4 + i];

			add_round_key(w);

			for (size_t r = 1; r < R; r++)
			{
				sub_bytes();
				shift_rows();
				mix_columns();
				add_round_key(w + (r * B));
			}

			sub_bytes();
			shift_rows();
			add_round_key(w + (R * B));

			for (size_t i = 0; i < 4; i++)
				for (size_t j = 0; j < B; j++)
					out[j * 4 + i] = m_state[i][j];
		}

		void inv_cipher(const uint8_t in[B * 4], uint8_t out[B * 4], uint32_t w[B * (R + 1)])
		{
			for (size_t i = 0; i < 4; i++)
				for (size_t j = 0; j < B; j++)
					m_state[i][j] = in[j * 4 + i];

			add_round_key(w + (R * B));

			for (size_t r = R - 1; r > 0; r--)
			{
				inv_shift_rows();
				inv_sub_bytes();
				add_round_key(w + (r * B));
				inv_mix_columns();
			}

			inv_shift_rows();
			inv_sub_bytes();
			add_round_key(w);

			for (size_t i = 0; i < 4; i++)
				for (size_t j = 0; j < B; j++)
					out[j * 4 + i] = m_state[i][j];
		}
	};

	using aes_128 = aes<cipher_mode::PCBC, 4, 4, 10>;
	using aes_192 = aes<cipher_mode::PCBC, 6, 4, 12>;
	using aes_256 = aes<cipher_mode::PCBC, 8, 4, 14>;
}
