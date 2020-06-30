#include "pch.h"

#include "crypto/sha1.h"

namespace rb::crypto
{
	void sha1::pad_data() noexcept
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

	void sha1::process_data() noexcept
	{
		for (size_t i = 0; i < m_data.size() / 64; i++)
		{
			uint32_t w[80] = { 0 };
			
			for (size_t j = 0; j < 16; j++)
				w[j] = m_data[j * 4 + 3] | (m_data[j * 4 + 2] << 8) | (m_data[j * 4 + 1] << 16) | (m_data[j * 4] << 24);

			for (size_t j = 16; j < 80; j++)
				w[j] = bit::rotl(w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16], 1);

			uint32_t a = m_hash[0];
			uint32_t b = m_hash[1];
			uint32_t c = m_hash[2];
			uint32_t d = m_hash[3];
			uint32_t e = m_hash[4];
			uint32_t f = 0, k = 0;

			for (size_t j = 0; j < 80; j++)
			{
				if (j >= 60)
				{
					f = b ^ c ^ d;
					k = 0xCA62C1D6;
				}
				else if (j >= 40)
				{
					f = (b & c) | (b & d) | (c & d);
					k = 0x8F1BBCDC;
				}
				else if (j >= 20)
				{
					f = b ^ c ^ d;
					k = 0x6ED9EBA1;
				}
				else
				{
					f = (b & c) | (~b & d);
					k = 0x5A827999;
				}

				uint32_t temp = bit::rotl(a, 5) + f + e + k + w[j];
				e = d;
				d = c;
				c = bit::rotl(b, 30);
				b = a;
				a = temp;
			}

			m_hash[0] += a;
			m_hash[1] += b;
			m_hash[2] += c;
			m_hash[3] += d;
			m_hash[4] += e;
		}
	}

	sha1::sha1() noexcept
	{
		clear();
	}

	sha1& sha1::write(const void* data, size_t size) noexcept
	{
		std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
		return *this;
	}
	
	void sha1::digest(void* dest) noexcept
	{
		pad_data();
		process_data();

		uint32_t* dest_u32 = reinterpret_cast<uint32_t*>(dest);

		for (uint32_t h : m_hash)
			bit::write_be(dest_u32++, h);
	}

	std::string sha1::hex_digest() noexcept
	{
		pad_data();
		process_data();

		std::stringstream hex_digest;
		
		for (uint32_t h : m_hash)
			hex_digest << std::setw(8) << std::setfill('0') << std::hex << h;

		return hex_digest.str();
	}

	void sha1::clear() noexcept
	{
		m_data.clear();

		m_hash[0] = 0x67452301;
		m_hash[1] = 0xEFCDAB89;
		m_hash[2] = 0x98BADCFE;
		m_hash[3] = 0x10325476;
		m_hash[4] = 0xC3D2E1F0;
	}
	
	sha1& sha1::operator<<(char value) noexcept
	{
		m_data.push_back(value);
		return *this;
	}

	sha1& sha1::operator<<(unsigned char value) noexcept
	{
		m_data.push_back(value);
		return *this;
	}

	sha1& sha1::operator<<(short value) noexcept
	{
		uint8_t bytes[sizeof(short)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(short));
		return *this;
	}

	sha1& sha1::operator<<(unsigned short value) noexcept
	{
		uint8_t bytes[sizeof(unsigned short)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned short));
		return *this;
	}

	sha1& sha1::operator<<(int value) noexcept
	{
		uint8_t bytes[sizeof(int)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(int));
		return *this;
	}

	sha1& sha1::operator<<(unsigned int value) noexcept
	{
		uint8_t bytes[sizeof(unsigned int)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned int));
		return *this;
	}

	sha1& sha1::operator<<(long value) noexcept
	{
		uint8_t bytes[sizeof(long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(long));
		return *this;
	}

	sha1& sha1::operator<<(unsigned long value) noexcept
	{
		uint8_t bytes[sizeof(unsigned long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned long));
		return *this;
	}

	sha1& sha1::operator<<(long long value) noexcept
	{
		uint8_t bytes[sizeof(long long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(long long));
		return *this;
	}

	sha1& sha1::operator<<(unsigned long long value) noexcept
	{
		uint8_t bytes[sizeof(unsigned long long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned long long));
		return *this;
	}

	sha1& sha1::operator<<(const std::string& str) noexcept
	{
		write(str.data(), str.size());
		return *this;
	}

	std::string sha1::operator()(const void* data, size_t size) noexcept
	{
		clear();
		write(data, size);
		return hex_digest();
	}

	std::string sha1::operator()(const std::string& str) noexcept
	{
		clear();
		write(str.data(), str.size());
		return hex_digest();
	}
}
