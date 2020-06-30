#pragma once

#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "bit/rotate.h"
#include "bit/endian.h"

#include "crypto/basic_hash.h"

namespace rb::crypto
{
	class sha1 : public basic_hash
	{
	public:
		static constexpr size_t BLOCK_SIZE = 512;
		static constexpr size_t DIGEST_SIZE = 160;

	public:
		sha1() noexcept;

		sha1& write(const void* data, size_t size) noexcept override;

		void digest(void* dest) noexcept override;
		[[nodiscard]] std::string hex_digest() noexcept override;

		void clear() noexcept override;

		sha1& operator<<(char value) noexcept override;
		sha1& operator<<(unsigned char value) noexcept override;

		sha1& operator<<(short value) noexcept override;
		sha1& operator<<(unsigned short value) noexcept override;

		sha1& operator<<(int value) noexcept override;
		sha1& operator<<(unsigned int value) noexcept override;

		sha1& operator<<(long value) noexcept override;
		sha1& operator<<(unsigned long value) noexcept override;

		sha1& operator<<(long long value) noexcept override;
		sha1& operator<<(unsigned long long value) noexcept override;

		sha1& operator<<(const std::string& str) noexcept override;

		[[nodiscard]] std::string operator()(const void* data, size_t size) noexcept override;
		[[nodiscard]] std::string operator()(const std::string& str) noexcept override;

	private:
		std::vector<uint8_t> m_data;
		uint32_t m_hash[5];

		void pad_data() noexcept;
		void process_data() noexcept;
	};
}
