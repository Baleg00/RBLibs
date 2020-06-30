#include "pch.h"

#include "crypto/sha2.h"

namespace rb::crypto
{
	static const uint32_t K[64] = {
		0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 
		0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5, 
		0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 
		0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174, 
		0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 
		0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA, 
		0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 
		0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967, 
		0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 
		0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85, 
		0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 
		0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070, 
		0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 
		0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
		0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 
		0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
	};

	void sha2_224::pad_data() noexcept
	{
		uint64_t size = m_data.size() * 8ull;

		m_data.push_back(0x80);

		while ((m_data.size() % 64) != 56)
			m_data.push_back(0);

		m_data.push_back((size >> 56) & 0xFF);
		m_data.push_back((size >> 48) & 0xFF);
		m_data.push_back((size >> 40) & 0xFF);
		m_data.push_back((size >> 32) & 0xFF);
		m_data.push_back((size >> 24) & 0xFF);
		m_data.push_back((size >> 16) & 0xFF);
		m_data.push_back((size >> 8) & 0xFF);
		m_data.push_back(size & 0xFF);
	}
	
	void sha2_224::process_data() noexcept
	{
		for (size_t i = 0; i < m_data.size() / 64; i++)
		{
			uint32_t w[64] = { 0 };
			
			for (size_t j = 0; j < 16; j++)
				w[j] = rb::bit::read_be<uint32_t>(&m_data[i * 64 + j * 4]);

			for (size_t j = 16; j < 64; j++)
			{
				uint32_t s0 = bit::rotr(w[j - 15], 7) ^ bit::rotr(w[j - 15], 18) ^ (w[j - 15] >> 3);
				uint32_t s1 = bit::rotr(w[j - 2], 17) ^ bit::rotr(w[j - 2], 19) ^ (w[j - 2] >> 10);
				w[j] = w[j - 16] + s0 + w[j - 7] + s1;
			}

			uint32_t a = m_hash[0];
			uint32_t b = m_hash[1];
			uint32_t c = m_hash[2];
			uint32_t d = m_hash[3];
			uint32_t e = m_hash[4];
			uint32_t f = m_hash[5];
			uint32_t g = m_hash[6];
			uint32_t h = m_hash[7];

			for (size_t j = 0; j < 64; j++)
			{
				uint32_t S1 = bit::rotr(e, 6) ^ bit::rotr(e, 11) ^ bit::rotr(e, 25);
				uint32_t ch = (e & f) ^ (~e & g);
				uint32_t temp1 = h + S1 + ch + K[j] + w[j];
				uint32_t S0 = bit::rotr(a, 2) ^ bit::rotr(a, 13) ^ bit::rotr(a, 22);
				uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
				uint32_t temp2 = S0 + maj;

				h = g;
				g = f;
				f = e;
				e = d + temp1;
				d = c;
				c = b;
				b = a;
				a = temp1 + temp2;
			}

			m_hash[0] += a;
			m_hash[1] += b;
			m_hash[2] += c;
			m_hash[3] += d;
			m_hash[4] += e;
			m_hash[5] += f;
			m_hash[6] += g;
			m_hash[7] += h;
		}
	}
	
	sha2_224::sha2_224() noexcept
	{
		clear();
	}
	
	sha2_224& sha2_224::write(const void* data, size_t size) noexcept
	{
		std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
		return *this;
	}
	
	void sha2_224::digest(void* dest) noexcept
	{
		pad_data();
		process_data();

		uint32_t* dest_u32 = reinterpret_cast<uint32_t*>(dest);

		for (size_t i = 0; i < 7; i++)
			bit::write_be(dest_u32++, m_hash[i]);
	}

	std::string sha2_224::hex_digest() noexcept
	{
		pad_data();
		process_data();

		std::stringstream hex_digest;
		
		for (size_t i = 0; i < 7; i++)
			hex_digest << std::setw(8) << std::setfill('0') << std::hex << m_hash[i];

		return hex_digest.str();
	}
	
	void sha2_224::clear() noexcept
	{
		m_data.clear();

		m_hash[0] = 0xC1059ED8;
		m_hash[1] = 0x367CD507;
		m_hash[2] = 0x3070DD17;
		m_hash[3] = 0xF70E5939;
		m_hash[4] = 0xFFC00B31;
		m_hash[5] = 0x68581511;
		m_hash[6] = 0x64F98FA7;
		m_hash[7] = 0xBEFA4FA4;
	}
	
	sha2_224& sha2_224::operator<<(char value) noexcept
	{
		m_data.push_back(value);
		return *this;
	}

	sha2_224& sha2_224::operator<<(unsigned char value) noexcept
	{
		m_data.push_back(value);
		return *this;
	}

	sha2_224& sha2_224::operator<<(short value) noexcept
	{
		uint8_t bytes[sizeof(short)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(short));
		return *this;
	}

	sha2_224& sha2_224::operator<<(unsigned short value) noexcept
	{
		uint8_t bytes[sizeof(unsigned short)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned short));
		return *this;
	}

	sha2_224& sha2_224::operator<<(int value) noexcept
	{
		uint8_t bytes[sizeof(int)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(int));
		return *this;
	}

	sha2_224& sha2_224::operator<<(unsigned int value) noexcept
	{
		uint8_t bytes[sizeof(unsigned int)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned int));
		return *this;
	}

	sha2_224& sha2_224::operator<<(long value) noexcept
	{
		uint8_t bytes[sizeof(long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(long));
		return *this;
	}

	sha2_224& sha2_224::operator<<(unsigned long value) noexcept
	{
		uint8_t bytes[sizeof(unsigned long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned long));
		return *this;
	}

	sha2_224& sha2_224::operator<<(long long value) noexcept
	{
		uint8_t bytes[sizeof(long long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(long long));
		return *this;
	}

	sha2_224& sha2_224::operator<<(unsigned long long value) noexcept
	{
		uint8_t bytes[sizeof(unsigned long long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned long long));
		return *this;
	}

	sha2_224& sha2_224::operator<<(const std::string& str) noexcept
	{
		write(str.data(), str.size());
		return *this;
	}

	std::string sha2_224::operator()(const void* data, size_t size) noexcept
	{
		clear();
		write(data, size);
		return hex_digest();
	}

	std::string sha2_224::operator()(const std::string& str) noexcept
	{
		clear();
		write(str.data(), str.size());
		return hex_digest();
	}
}
