#pragma once

namespace rb::crypto
{
	class basic_keyed_hash
	{
	public:
		virtual ~basic_keyed_hash() noexcept = default;

		virtual basic_keyed_hash& write(const void* data, size_t size) noexcept = 0;

		virtual void digest(const void* key, size_t size, void* dest) noexcept = 0;
		virtual std::string hex_digest(const void* key, size_t size) noexcept = 0;

		virtual void clear() noexcept = 0;

		virtual basic_keyed_hash& operator<<(char value) noexcept = 0;
		virtual basic_keyed_hash& operator<<(unsigned char value) noexcept = 0;

		virtual basic_keyed_hash& operator<<(short value) noexcept = 0;
		virtual basic_keyed_hash& operator<<(unsigned short value) noexcept = 0;

		virtual basic_keyed_hash& operator<<(int value) noexcept = 0;
		virtual basic_keyed_hash& operator<<(unsigned int value) noexcept = 0;

		virtual basic_keyed_hash& operator<<(long value) noexcept = 0;
		virtual basic_keyed_hash& operator<<(unsigned long value) noexcept = 0;

		virtual basic_keyed_hash& operator<<(long long value) noexcept = 0;
		virtual basic_keyed_hash& operator<<(unsigned long long value) noexcept = 0;

		virtual basic_keyed_hash& operator<<(const std::string & str) noexcept = 0;
	};
}
