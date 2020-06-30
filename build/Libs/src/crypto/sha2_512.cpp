#include "pch.h"

#include "crypto/sha2.h"

namespace rb::crypto
{
	static const uint64_t K[80] = {
		0x428A2F98D728AE22ull, 0x7137449123EF65CDull, 0xB5C0FBCFEC4D3B2Full, 0xE9B5DBA58189DBBCull, 
		0x3956C25BF348B538ull, 0x59F111F1B605D019ull, 0x923F82A4AF194F9Bull, 0xAB1C5ED5DA6D8118ull, 
		0xD807AA98A3030242ull, 0x12835B0145706FBEull, 0x243185BE4EE4B28Cull, 0x550C7DC3D5FFB4E2ull, 
		0x72BE5D74F27B896Full, 0x80DEB1FE3B1696B1ull, 0x9BDC06A725C71235ull, 0xC19BF174CF692694ull, 
		0xE49B69C19EF14AD2ull, 0xEFBE4786384F25E3ull, 0x0FC19DC68B8CD5B5ull, 0x240CA1CC77AC9C65ull, 
		0x2DE92C6F592B0275ull, 0x4A7484AA6EA6E483ull, 0x5CB0A9DCBD41FBD4ull, 0x76F988DA831153B5ull, 
		0x983E5152EE66DFABull, 0xA831C66D2DB43210ull, 0xB00327C898FB213Full, 0xBF597FC7BEEF0EE4ull, 
		0xC6E00BF33DA88FC2ull, 0xD5A79147930AA725ull, 0x06CA6351E003826Full, 0x142929670A0E6E70ull, 
		0x27B70A8546D22FFCull, 0x2E1B21385C26C926ull, 0x4D2C6DFC5AC42AEDull, 0x53380D139D95B3DFull, 
		0x650A73548BAF63DEull, 0x766A0ABB3C77B2A8ull, 0x81C2C92E47EDAEE6ull, 0x92722C851482353Bull, 
		0xA2BFE8A14CF10364ull, 0xA81A664BBC423001ull, 0xC24B8B70D0F89791ull, 0xC76C51A30654BE30ull, 
		0xD192E819D6EF5218ull, 0xD69906245565A910ull, 0xF40E35855771202Aull, 0x106AA07032BBD1B8ull, 
		0x19A4C116B8D2D0C8ull, 0x1E376C085141AB53ull, 0x2748774CDF8EEB99ull, 0x34B0BCB5E19B48A8ull, 
		0x391C0CB3C5C95A63ull, 0x4ED8AA4AE3418ACBull, 0x5B9CCA4F7763E373ull, 0x682E6FF3D6B2B8A3ull, 
		0x748F82EE5DEFB2FCull, 0x78A5636F43172F60ull, 0x84C87814A1F0AB72ull, 0x8CC702081A6439ECull, 
		0x90BEFFFA23631E28ull, 0xA4506CEBDE82BDE9ull, 0xBEF9A3F7B2C67915ull, 0xC67178F2E372532Bull, 
		0xCA273ECEEA26619Cull, 0xD186B8C721C0C207ull, 0xEADA7DD6CDE0EB1Eull, 0xF57D4F7FEE6ED178ull, 
		0x06F067AA72176FBAull, 0x0A637DC5A2C898A6ull, 0x113F9804BEF90DAEull, 0x1B710B35131C471Bull, 
		0x28DB77F523047D84ull, 0x32CAAB7B40C72493ull, 0x3C9EBE0A15C9BEBCull, 0x431D67C49C100D4Cull, 
		0x4CC5D4BECB3E42B6ull, 0x597F299CFC657E2Aull, 0x5FCB6FAB3AD6FAECull, 0x6C44198C4A475817ull
	};

	void sha2_512::pad_data() noexcept
	{
		uint64_t size = m_data.size() * 8ull;
		
		m_data.push_back(0x80);

		while ((m_data.size() % 128) != 112)
			m_data.push_back(0);

		std::fill_n(std::back_inserter(m_data), 8, 0);
		m_data.push_back((size >> 56) & 0xFF);
		m_data.push_back((size >> 48) & 0xFF);
		m_data.push_back((size >> 40) & 0xFF);
		m_data.push_back((size >> 32) & 0xFF);
		m_data.push_back((size >> 24) & 0xFF);
		m_data.push_back((size >> 16) & 0xFF);
		m_data.push_back((size >> 8) & 0xFF);
		m_data.push_back(size & 0xFF);
	}
	
	void sha2_512::process_data() noexcept
	{
		for (size_t i = 0; i < m_data.size() / 128; i++)
		{
			uint64_t w[80] = { 0 };
			
			for (size_t j = 0; j < 16; j++)
				w[j] = rb::bit::read_be<uint64_t>(&m_data[i * 128 + j * 8]);

			for (size_t j = 16; j < 80; j++)
			{
				uint64_t s0 = bit::rotr(w[j - 15], 1) ^ bit::rotr(w[j - 15], 8) ^ (w[j - 15] >> 7);
				uint64_t s1 = bit::rotr(w[j - 2], 19) ^ bit::rotr(w[j - 2], 61) ^ (w[j - 2] >> 6);
				w[j] = w[j - 16] + s0 + w[j - 7] + s1;
			}

			uint64_t a = m_hash[0];
			uint64_t b = m_hash[1];
			uint64_t c = m_hash[2];
			uint64_t d = m_hash[3];
			uint64_t e = m_hash[4];
			uint64_t f = m_hash[5];
			uint64_t g = m_hash[6];
			uint64_t h = m_hash[7];

			for (size_t j = 0; j < 80; j++)
			{
				uint64_t S1 = bit::rotr(e, 14) ^ bit::rotr(e, 18) ^ bit::rotr(e, 41);
				uint64_t ch = (e & f) ^ (~e & g);
				uint64_t temp1 = h + S1 + ch + K[j] + w[j];
				uint64_t S0 = bit::rotr(a, 28) ^ bit::rotr(a, 34) ^ bit::rotr(a, 39);
				uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
				uint64_t temp2 = S0 + maj;

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
	
	sha2_512::sha2_512() noexcept
	{
		clear();
	}
	
	sha2_512& sha2_512::write(const void* data, size_t size) noexcept
	{
		std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
		return *this;
	}
	
	void sha2_512::digest(void* dest) noexcept
	{
		pad_data();
		process_data();

		uint64_t* dest_u64 = reinterpret_cast<uint64_t*>(dest);

		for (uint64_t h : m_hash)
			bit::write_be(dest_u64++, h);
	}

	std::string sha2_512::hex_digest() noexcept
	{
		pad_data();
		process_data();

		std::stringstream hex_digest;
		
		for (uint64_t h : m_hash)
			hex_digest << std::setw(16) << std::setfill('0') << std::hex << h;

		return hex_digest.str();
	}
	
	void sha2_512::clear() noexcept
	{
		m_data.clear();

		m_hash[0] = 0x6A09E667F3BCC908ull;
		m_hash[1] = 0xBB67AE8584CAA73Bull;
		m_hash[2] = 0x3C6EF372FE94F82Bull;
		m_hash[3] = 0xA54FF53A5F1D36F1ull;
		m_hash[4] = 0x510E527FADE682D1ull;
		m_hash[5] = 0x9B05688C2B3E6C1Full;
		m_hash[6] = 0x1F83D9ABFB41BD6Bull;
		m_hash[7] = 0x5BE0CD19137E2179ull;
	}
	
	sha2_512& sha2_512::operator<<(char value) noexcept
	{
		m_data.push_back(value);
		return *this;
	}

	sha2_512& sha2_512::operator<<(unsigned char value) noexcept
	{
		m_data.push_back(value);
		return *this;
	}

	sha2_512& sha2_512::operator<<(short value) noexcept
	{
		uint8_t bytes[sizeof(short)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(short));
		return *this;
	}

	sha2_512& sha2_512::operator<<(unsigned short value) noexcept
	{
		uint8_t bytes[sizeof(unsigned short)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned short));
		return *this;
	}

	sha2_512& sha2_512::operator<<(int value) noexcept
	{
		uint8_t bytes[sizeof(int)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(int));
		return *this;
	}

	sha2_512& sha2_512::operator<<(unsigned int value) noexcept
	{
		uint8_t bytes[sizeof(unsigned int)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned int));
		return *this;
	}

	sha2_512& sha2_512::operator<<(long value) noexcept
	{
		uint8_t bytes[sizeof(long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(long));
		return *this;
	}

	sha2_512& sha2_512::operator<<(unsigned long value) noexcept
	{
		uint8_t bytes[sizeof(unsigned long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned long));
		return *this;
	}

	sha2_512& sha2_512::operator<<(long long value) noexcept
	{
		uint8_t bytes[sizeof(long long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(long long));
		return *this;
	}

	sha2_512& sha2_512::operator<<(unsigned long long value) noexcept
	{
		uint8_t bytes[sizeof(unsigned long long)];
		bit::write_be(bytes, value);
		write(bytes, sizeof(unsigned long long));
		return *this;
	}

	sha2_512& sha2_512::operator<<(const std::string& str) noexcept
	{
		write(str.data(), str.size());
		return *this;
	}

	std::string sha2_512::operator()(const void* data, size_t size) noexcept
	{
		clear();
		write(data, size);
		return hex_digest();
	}

	std::string sha2_512::operator()(const std::string& str) noexcept
	{
		clear();
		write(str.data(), str.size());
		return hex_digest();
	}
}
