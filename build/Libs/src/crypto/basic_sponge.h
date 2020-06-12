#pragma once

#include <deque>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "crypto/basic_hash.h"

namespace rb::crypto
{
	typedef void(*perm_func_t)(uint8_t*, size_t);

	template<size_t R, size_t C, uint8_t D, size_t H, perm_func_t F>
	class basic_sponge : public basic_hash
	{
	public:
		static constexpr size_t BIT_RATE = R;
		static constexpr size_t BYTE_RATE = R / 8;
		static constexpr size_t CAPACITY = C;
		static constexpr size_t STATE_SIZE = R + C;

		static constexpr size_t BLOCK_SIZE = R;
		static constexpr size_t DIGEST_SIZE = H;

	private:
		static_assert(R + C == 1600 && R % 8 == 0);

		std::deque<uint8_t> m_data;
		uint8_t m_hash[DIGEST_SIZE / 8];

		void process_data() noexcept
		{
			uint8_t state[STATE_SIZE / 8] = { 0 };
			uint32_t blockSize = 0;

			while (m_data.size() > 0)
			{
				blockSize = std::min(m_data.size(), BYTE_RATE);

				for (size_t i = 0; i < blockSize; i++)
				{
					state[i] ^= m_data.front();
					m_data.pop_front();
				}

				if (blockSize == BYTE_RATE)
					(*F)(state, STATE_SIZE);
			}

			state[blockSize] ^= D;

			if ((D & 0x80) != 0 && blockSize == BYTE_RATE - 1)
				(*F)(state, STATE_SIZE);

			state[BYTE_RATE - 1] ^= 0x80;

			(*F)(state, STATE_SIZE);

			size_t out_len = DIGEST_SIZE / 8;

			while (out_len > 0)
			{
				blockSize = std::min(out_len, BYTE_RATE);

				std::memcpy(&m_hash[DIGEST_SIZE / 8 - out_len], state, blockSize);

				out_len -= blockSize;

				if (out_len > 0)
					(*F)(state, STATE_SIZE);
			}
		}

	public:
		basic_sponge() noexcept
		{
			clear();
		}

		basic_sponge& write(const void* data, size_t size) noexcept override
		{
			std::copy_n(reinterpret_cast<const uint8_t*>(data), size, std::back_inserter(m_data));
			return *this;
		}

		basic_sponge& write(const std::string& str) noexcept override
		{
			std::copy(str.begin(), str.end(), std::back_inserter(m_data));
			return *this;
		}

		void digest(void* dest) noexcept override
		{
			process_data();
			std::memcpy(dest, m_hash, DIGEST_SIZE / 8);
		}

		[[nodiscard]] std::string hex_digest() noexcept override
		{
			process_data();

			std::stringstream hex_digest;

			for (uint8_t b : m_hash)
				hex_digest << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(b);

			return hex_digest.str();
		}

		void clear() noexcept override
		{
			m_data.clear();
			std::memset(m_hash, 0, DIGEST_SIZE / 8);
		}

		basic_sponge& operator<<(const std::string& str) noexcept override
		{
			write(str);
			return *this;
		}

		[[nodiscard]] std::string operator()(const void* data, size_t size) noexcept override
		{
			clear();
			write(data, size);
			return hex_digest();
		}

		[[nodiscard]] std::string operator()(const std::string& str) noexcept override
		{
			clear();
			write(str);
			return hex_digest();
		}
	};
}
