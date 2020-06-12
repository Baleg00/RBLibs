#pragma once

#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

#include "crypto/basic_hash.h"

namespace rb::crypto
{
	template<class H>
	class hmac : public basic_hash
	{
	public:
		static constexpr size_t BLOCK_SIZE = H::BLOCK_SIZE;
		static constexpr size_t DIGEST_SIZE = H::DIGEST_SIZE;

	private:
		std::vector<uint8_t> m_data, m_key;

		uint8_t m_hash[DIGEST_SIZE / 8];

		void preprocess_key() noexcept
		{
			if (m_key.size() > BLOCK_SIZE / 8)
			{
				H hash;
				hash.write(m_key.data(), m_key.size());
				m_key.clear();
				m_key.reserve(DIGEST_SIZE / 8);
				hash.digest(m_key.data());
			}

			if (m_key.size() < BLOCK_SIZE / 8)
				m_key.insert(m_key.end(), BLOCK_SIZE / 8 - m_key.size(), 0);
		}

		void process_data() noexcept
		{
			uint8_t o_key_pad[BLOCK_SIZE / 8];
			uint8_t i_key_pad[BLOCK_SIZE / 8];

			for (size_t i = 0; i < BLOCK_SIZE / 8; i++)
			{
				o_key_pad[i] = m_key[i] ^ 0x5C;
				i_key_pad[i] = m_key[i] ^ 0x36;
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

	public:
		hmac() noexcept
		{
			clear();
		}

		void set_key(const void* key, size_t size) noexcept
		{
			m_key.clear();
			std::copy_n(reinterpret_cast<const uint8_t*>(key), size, std::back_inserter(m_key));
		}

		void set_key(const std::string& str) noexcept
		{
			m_key.clear();
			std::copy(str.begin(), str.end(), std::back_inserter(m_key));
		}

		hmac& write(const void* data, size_t size) noexcept override
		{
			std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
			return *this;
		}

		hmac& write(const std::string& str) noexcept override
		{
			std::copy(str.begin(), str.end(), std::back_inserter(m_data));
			return *this;
		}

		void digest(void* dest) noexcept override
		{
			preprocess_key();
			process_data();

			std::memcpy(dest, m_hash, DIGEST_SIZE / 8);
		}

		[[nodiscard]] std::string hex_digest() noexcept override
		{
			preprocess_key();
			process_data();

			std::stringstream hex_digest;

			for (uint8_t b : m_hash)
				hex_digest << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(b);

			return hex_digest.str();
		}

		void clear() noexcept override
		{
			m_data.clear();
		}

		basic_hash& operator<<(const std::string& str) noexcept override
		{
			write(str);
			return *this;
		}

		[[nodiscard]] std::string operator()(const void* data, size_t data_size) noexcept override
		{
			clear();
			write(data, data_size);
			return hex_digest();
		}

		[[nodiscard]] std::string operator()(const std::string& data) noexcept override
		{
			clear();
			write(data);
			return hex_digest();
		}
	};
}
