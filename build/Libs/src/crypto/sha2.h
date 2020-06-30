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
	class sha2_224 : public basic_hash
	{
	public:
		static constexpr size_t BLOCK_SIZE = 512;
		static constexpr size_t DIGEST_SIZE = 224;

	public:
		sha2_224() noexcept;

		sha2_224& write(const void* data, size_t size) noexcept override;

		void digest(void* dest) noexcept override;
		[[nodiscard]] std::string hex_digest() noexcept override;

		void clear() noexcept override;

		sha2_224& operator<<(char value) noexcept override;
		sha2_224& operator<<(unsigned char value) noexcept override;

		sha2_224& operator<<(short value) noexcept override;
		sha2_224& operator<<(unsigned short value) noexcept override;

		sha2_224& operator<<(int value) noexcept override;
		sha2_224& operator<<(unsigned int value) noexcept override;

		sha2_224& operator<<(long value) noexcept override;
		sha2_224& operator<<(unsigned long value) noexcept override;

		sha2_224& operator<<(long long value) noexcept override;
		sha2_224& operator<<(unsigned long long value) noexcept override;

		sha2_224& operator<<(const std::string& str) noexcept override;

		[[nodiscard]] std::string operator()(const void* data, size_t size) noexcept override;
		[[nodiscard]] std::string operator()(const std::string& str) noexcept override;

	private:
		std::vector<uint8_t> m_data;
		uint32_t m_hash[8];

		void pad_data() noexcept;
		void process_data() noexcept;
	};

	class sha2_256 : public basic_hash
	{
	public:
		static constexpr size_t BLOCK_SIZE = 512;
		static constexpr size_t DIGEST_SIZE = 256;

	public:
		sha2_256() noexcept;

		sha2_256& write(const void* data, size_t size) noexcept override;

		void digest(void* dest) noexcept override;
		[[nodiscard]] std::string hex_digest() noexcept override;

		void clear() noexcept override;

		sha2_256& operator<<(char value) noexcept override;
		sha2_256& operator<<(unsigned char value) noexcept override;

		sha2_256& operator<<(short value) noexcept override;
		sha2_256& operator<<(unsigned short value) noexcept override;

		sha2_256& operator<<(int value) noexcept override;
		sha2_256& operator<<(unsigned int value) noexcept override;

		sha2_256& operator<<(long value) noexcept override;
		sha2_256& operator<<(unsigned long value) noexcept override;

		sha2_256& operator<<(long long value) noexcept override;
		sha2_256& operator<<(unsigned long long value) noexcept override;

		sha2_256& operator<<(const std::string& str) noexcept override;

		[[nodiscard]] std::string operator()(const void* data, size_t size) noexcept override;
		[[nodiscard]] std::string operator()(const std::string& str) noexcept override;

	private:
		std::vector<uint8_t> m_data;
		uint32_t m_hash[8];

		void pad_data() noexcept;
		void process_data() noexcept;
	};

	class sha2_384 : public basic_hash
	{
	public:
		static constexpr size_t BLOCK_SIZE = 1024;
		static constexpr size_t DIGEST_SIZE = 384;

	public:
		sha2_384() noexcept;

		sha2_384& write(const void* data, size_t size) noexcept override;

		void digest(void* dest) noexcept override;
		[[nodiscard]] std::string hex_digest() noexcept override;

		void clear() noexcept override;

		sha2_384& operator<<(char value) noexcept override;
		sha2_384& operator<<(unsigned char value) noexcept override;

		sha2_384& operator<<(short value) noexcept override;
		sha2_384& operator<<(unsigned short value) noexcept override;

		sha2_384& operator<<(int value) noexcept override;
		sha2_384& operator<<(unsigned int value) noexcept override;

		sha2_384& operator<<(long value) noexcept override;
		sha2_384& operator<<(unsigned long value) noexcept override;

		sha2_384& operator<<(long long value) noexcept override;
		sha2_384& operator<<(unsigned long long value) noexcept override;

		sha2_384& operator<<(const std::string& str) noexcept override;

		[[nodiscard]] std::string operator()(const void* data, size_t size) noexcept override;
		[[nodiscard]] std::string operator()(const std::string& str) noexcept override;

	private:
		std::vector<uint8_t> m_data;
		uint64_t m_hash[8];

		void pad_data() noexcept;
		void process_data() noexcept;
	};

	class sha2_512 : public basic_hash
	{
	public:
		static constexpr size_t BLOCK_SIZE = 1024;
		static constexpr size_t DIGEST_SIZE = 512;

	public:
		sha2_512() noexcept;

		sha2_512& write(const void* data, size_t size) noexcept override;

		void digest(void* dest) noexcept override;
		[[nodiscard]] std::string hex_digest() noexcept override;

		void clear() noexcept override;

		sha2_512& operator<<(char value) noexcept override;
		sha2_512& operator<<(unsigned char value) noexcept override;

		sha2_512& operator<<(short value) noexcept override;
		sha2_512& operator<<(unsigned short value) noexcept override;

		sha2_512& operator<<(int value) noexcept override;
		sha2_512& operator<<(unsigned int value) noexcept override;

		sha2_512& operator<<(long value) noexcept override;
		sha2_512& operator<<(unsigned long value) noexcept override;

		sha2_512& operator<<(long long value) noexcept override;
		sha2_512& operator<<(unsigned long long value) noexcept override;

		sha2_512& operator<<(const std::string& str) noexcept override;

		[[nodiscard]] std::string operator()(const void* data, size_t size) noexcept override;
		[[nodiscard]] std::string operator()(const std::string& str) noexcept override;

	private:
		std::vector<uint8_t> m_data;
		uint64_t m_hash[8];

		void pad_data() noexcept;
		void process_data() noexcept;
	};
}
