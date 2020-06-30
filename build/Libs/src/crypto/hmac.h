#pragma once

#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

#include "bit/endian.h"

#include "crypto/basic_keyed_hash.h"

namespace rb::crypto
{
	template<class H>
	class hmac : public basic_keyed_hash
	{
	public:
		static constexpr size_t BLOCK_SIZE = H::BLOCK_SIZE;
		static constexpr size_t DIGEST_SIZE = H::DIGEST_SIZE;
	
	public:
		hmac() noexcept
		{
			clear();
		}

		hmac& write(const void* data, size_t size) noexcept override
		{
			std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
			return *this;
		}

		void digest(const void* key, size_t size, void* dest) noexcept override
		{
			process_data(reinterpret_cast<const uint8_t*>(key), size);

			std::memcpy(dest, m_hash, DIGEST_SIZE / 8);
		}

		[[nodiscard]] std::string hex_digest(const void* key, size_t size) noexcept override
		{
			process_data(reinterpret_cast<const uint8_t*>(key), size);

			std::stringstream hex_digest;

			for (uint8_t b : m_hash)
				hex_digest << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(b);

			return hex_digest.str();
		}

		void clear() noexcept override
		{
			m_data.clear();
		}

		hmac& operator<<(char value) noexcept override
		{
			m_data.push_back(value);
			return *this;
		}

		hmac& operator<<(unsigned char value) noexcept override
		{
			m_data.push_back(value);
			return *this;
		}

		hmac& operator<<(short value) noexcept override
		{
			uint8_t bytes[sizeof(short)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(short));
			return *this;
		}

		hmac& operator<<(unsigned short value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned short)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned short));
			return *this;
		}

		hmac& operator<<(int value) noexcept override
		{
			uint8_t bytes[sizeof(int)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(int));
			return *this;
		}

		hmac& operator<<(unsigned int value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned int)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned int));
			return *this;
		}

		hmac& operator<<(long value) noexcept override
		{
			uint8_t bytes[sizeof(long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(long));
			return *this;
		}

		hmac& operator<<(unsigned long value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned long));
			return *this;
		}

		hmac& operator<<(long long value) noexcept override
		{
			uint8_t bytes[sizeof(long long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(long long));
			return *this;
		}

		hmac& operator<<(unsigned long long value) noexcept override
		{
			uint8_t bytes[sizeof(unsigned long long)];
			bit::write_be(bytes, value);
			write(bytes, sizeof(unsigned long long));
			return *this;
		}

		hmac& operator<<(const std::string& str) noexcept override
		{
			write(str.data(), str.size());
			return *this;
		}

	private:
		std::vector<uint8_t> m_data;
		uint8_t m_hash[DIGEST_SIZE / 8];

		void process_data(const uint8_t* key, size_t size) noexcept
		{
			uint8_t real_key[BLOCK_SIZE / 8]{ 0 };

			if (size > BLOCK_SIZE / 8)
			{
				H hash;
				hash.write(key, size);
				hash.digest(real_key);
			}
			else if (size < BLOCK_SIZE / 8)
				std::memcpy(real_key, key, size);

			uint8_t o_key_pad[BLOCK_SIZE / 8];
			uint8_t i_key_pad[BLOCK_SIZE / 8];

			for (size_t i = 0; i < BLOCK_SIZE / 8; i++)
			{
				o_key_pad[i] = real_key[i] ^ 0x5C;
				i_key_pad[i] = real_key[i] ^ 0x36;
			}

			uint8_t i_hash[DIGEST_SIZE / 8];

			H hash;
			hash.write(i_key_pad, BLOCK_SIZE / 8);
			hash.write(m_data.data(), m_data.size());
			hash.digest(i_hash);
			hash.clear();

			hash.write(o_key_pad, BLOCK_SIZE / 8);
			hash.write(i_hash, DIGEST_SIZE / 8);
			hash.digest(m_hash);
		}
	};
}
