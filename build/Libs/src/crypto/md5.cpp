#include "pch.h"

#include "crypto/md5.h"

namespace rb::crypto
{
	static const uint32_t S[64] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};

	static const uint32_t K[64] = {
		0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501, 
		0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821, 
		0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8, 
		0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A, 
		0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70, 
		0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665, 
		0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1, 
		0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
	};

	void md5::pad_data() noexcept
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

	void md5::process_data() noexcept
	{
		for (size_t i = 0; i < m_data.size() / 64; i++)
		{
			uint32_t M[16] = { 0 };
			
			for (size_t j = 0; j < 16; j++)
				M[j] = m_data[j * 4] | (m_data[j * 4 + 1] << 8) | (m_data[j * 4 + 2] << 16) | (m_data[j * 4 + 3] << 24);

			uint32_t a = m_hash[0];
			uint32_t b = m_hash[1];
			uint32_t c = m_hash[2];
			uint32_t d = m_hash[3];
			uint32_t f = 0, g = 0;

			for (size_t j = 0; j < 64; j++)
			{
				if (j >= 48)
				{
					f = c ^ (b | ~d);
					g = (j * 7) % 16;
				}
				else if (j >= 32)
				{
					f = b ^ c ^ d;
					g = (j * 3 + 5) % 16;
				}
				else if (j >= 16)
				{
					f = (b & d) | (~d & c);
					g = (j * 5 + 1) % 16;
				}
				else
				{
					f = (b & c) | (~b & d);
					g = j;
				}

				f = f + a + K[j] + M[g];
				a = d;
				d = c;
				c = b;
				b = b + bit::rotl(f, S[j]);
			}

			m_hash[0] += a;
			m_hash[1] += b;
			m_hash[2] += c;
			m_hash[3] += d;
		}
	}

	md5::md5() noexcept
	{
		clear();
	}

	md5& md5::write(const void* data, size_t size) noexcept
	{
		std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
		return *this;
	}

	md5& md5::write(const std::string& str) noexcept
	{
		std::copy(str.begin(), str.end(), std::back_inserter(m_data));
		return *this;
	}

	void md5::digest(void* dest) noexcept
	{
		pad_data();
		process_data();

		uint32_t* dest_u32 = reinterpret_cast<uint32_t*>(dest);

		for (uint32_t h : m_hash)
			bit::write_le(dest_u32++, h);
	}

	std::string md5::hex_digest() noexcept
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

	void md5::clear() noexcept
	{
		m_data.clear();

		m_hash[0] = 0x67452301;
		m_hash[1] = 0xEFCDAB89;
		m_hash[2] = 0x98BADCFE;
		m_hash[3] = 0x10325476;
	}

	md5& md5::operator<<(const std::string& str) noexcept
	{
		write(str);
		return *this;
	}

	std::string md5::operator()(const void* data, size_t size) noexcept
	{
		clear();
		write(data, size);
		return hex_digest();
	}

	std::string md5::operator()(const std::string& str) noexcept
	{
		clear();
		write(str);
		return hex_digest();
	}
}
