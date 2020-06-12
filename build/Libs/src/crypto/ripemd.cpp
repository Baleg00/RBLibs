#include "pch.h"

#include "crypto/ripemd.h"

namespace rb::crypto
{
	static const uint32_t K[5] = {
		0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E
	};

	static const uint32_t K_[5] = {
		0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000
	};

	static const uint32_t r[80] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
		3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
		1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
		4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13
	};

	static const uint32_t r_[80] = {
		5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
		6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
		15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
		8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
		12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11
	};

	static const uint32_t s[80] = {
		11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
		7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
		11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
		11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
		9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6
	};

	static const uint32_t s_[80] = {
		8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
		9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
		9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
		15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
		8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11
	};

	void ripemd160::pad_data() noexcept
	{
		uint64_t size = m_data.size() * 8ull;

		m_data.push_back(0x80);

		while ((m_data.size() % 64) != 56)
			m_data.push_back(0);

		m_data.push_back(size & 0xFF); size >>= 8;
		m_data.push_back(size & 0xFF); size >>= 8;
		m_data.push_back(size & 0xFF); size >>= 8;
		m_data.push_back(size & 0xFF); size >>= 8;
		m_data.push_back(size & 0xFF); size >>= 8;
		m_data.push_back(size & 0xFF); size >>= 8;
		m_data.push_back(size & 0xFF); size >>= 8;
		m_data.push_back(size & 0xFF);
	}
	
	void ripemd160::process_data() noexcept
	{
		for (size_t i = 0; i < m_data.size() / 64; i++)
		{
			uint32_t w[16] = { 0 };
			
			for (size_t j = 0; j < 16; j++)
				w[j] = m_data[j * 4] | (m_data[j * 4 + 1] << 8) | (m_data[j * 4 + 2] << 16) | (m_data[j * 4 + 3] << 24);

			uint32_t a = m_hash[0], a_ = m_hash[0];
			uint32_t b = m_hash[1], b_ = m_hash[1];
			uint32_t c = m_hash[2], c_ = m_hash[2];
			uint32_t d = m_hash[3], d_ = m_hash[3];
			uint32_t e = m_hash[4], e_ = m_hash[4];

			for (size_t j = 0; j < 80; j++)
			{
				uint32_t t = 0, t_ = 0;

				if (j >= 64)
				{
					t = bit::rotl(a + (b ^ (c | ~d)) + w[r[j]] + K[4], s[j]) + e;
					t_ = bit::rotl(a_ + (b_ ^ c_ ^ d_) + w[r_[j]] + K_[4], s_[j]) + e_;
				}
				else if (j >= 48)
				{
					t = bit::rotl(a + ((b & d) | (c & ~d)) + w[r[j]] + K[3], s[j]) + e;
					t_ = bit::rotl(a_ + ((b_ & c_) | (~b_ & d_)) + w[r_[j]] + K_[3], s_[j]) + e_;
				}
				else if (j >= 32)
				{
					t = bit::rotl(a + ((b | ~c) ^ d) + w[r[j]] + K[2], s[j]) + e;
					t_ = bit::rotl(a_ + ((b_ | ~c_) ^ d_) + w[r_[j]] + K_[2], s_[j]) + e_;
				}
				else if (j >= 16)
				{
					t = bit::rotl(a + ((b & c) | (~b & d)) + w[r[j]] + K[1], s[j]) + e;
					t_ = bit::rotl(a_ + ((b_ & d_) | (c_ & ~d_)) + w[r_[j]] + K_[1], s_[j]) + e_;
				}
				else
				{
					t = bit::rotl(a + (b ^ c ^ d) + w[r[j]] + K[0], s[j]) + e;
					t_ = bit::rotl(a_ + (b_ ^ (c_ | ~d_)) + w[r_[j]] + K_[0], s_[j]) + e_;
				}

				a = e;
				e = d;
				d = bit::rotl(c, 10);
				c = b;
				b = t;

				a_ = e_;
				e_ = d_;
				d_ = bit::rotl(c_, 10);
				c_ = b_;
				b_ = t_;
			}

			uint32_t t = m_hash[1] + c + d_;
			m_hash[1] = m_hash[2] + d + e_;
			m_hash[2] = m_hash[3] + e + a_;
			m_hash[3] = m_hash[4] + a + b_;
			m_hash[4] = m_hash[0] + b + c_;
			m_hash[0] = t;
		}
	}
	
	ripemd160::ripemd160() noexcept
	{
		clear();
	}
	
	ripemd160& ripemd160::write(const void* data, size_t size) noexcept
	{
		std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
		return *this;
	}
	
	ripemd160& ripemd160::write(const std::string& str) noexcept
	{
		std::copy(str.begin(), str.end(), std::back_inserter(m_data));
		return *this;
	}
	
	void ripemd160::digest(void* dest) noexcept
	{
		pad_data();
		process_data();

		uint32_t* dest_u32 = reinterpret_cast<uint32_t*>(dest);

		for (uint32_t h : m_hash)
			bit::write_le(dest_u32++, h);
	}

	std::string ripemd160::hex_digest() noexcept
	{
		pad_data();
		process_data();

		std::stringstream hex_digest;

		for (uint32_t h : m_hash)
		{
			hex_digest << std::setw(2) << std::setfill('0') << std::hex << (h & 0xFF);
			hex_digest << std::setw(2) << std::setfill('0') << std::hex << ((h >> 8) & 0xFF);
			hex_digest << std::setw(2) << std::setfill('0') << std::hex << ((h >> 16) & 0xFF);
			hex_digest << std::setw(2) << std::setfill('0') << std::hex << ((h >> 24) & 0xFF);
		}

		return hex_digest.str();
	}
	
	void ripemd160::clear() noexcept
	{
		m_data.clear();

		m_hash[0] = 0x67452301;
		m_hash[1] = 0xEFCDAB89;
		m_hash[2] = 0x98BADCFE;
		m_hash[3] = 0x10325476;
		m_hash[4] = 0xC3D2E1F0;
	}
	
	ripemd160& ripemd160::operator<<(const std::string& str) noexcept
	{
		write(str);
		return *this;
	}
	
	std::string ripemd160::operator()(const void* data, size_t size) noexcept
	{
		clear();
		write(data, size);
		return hex_digest();
	}
	
	std::string ripemd160::operator()(const std::string& str) noexcept
	{
		clear();
		write(str);
		return hex_digest();
	}
}
