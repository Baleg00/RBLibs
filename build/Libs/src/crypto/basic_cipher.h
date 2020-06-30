#pragma once

#include <string>

namespace rb::crypto
{
	enum class cipher_mode
	{
		ECB, CBC, PCBC, CFB, OFB, CTR
	};

	class basic_cipher
	{
	public:
		virtual ~basic_cipher() = default;

		virtual basic_cipher& write(const void* data, size_t size) noexcept = 0;
		
		virtual size_t encrypt(const void* key, size_t size, void* dest) noexcept = 0;
		virtual size_t decrypt(const void* key, size_t size, void* dest) noexcept = 0;

		virtual std::string hex_encrypt(const void* key, size_t size) noexcept = 0;
		virtual std::string hex_decrypt(const void* key, size_t size) noexcept = 0;

		virtual void clear() noexcept = 0;
		
		virtual basic_cipher& operator<<(char value) noexcept = 0;
		virtual basic_cipher& operator<<(unsigned char value) noexcept = 0;

		virtual basic_cipher& operator<<(short value) noexcept = 0;
		virtual basic_cipher& operator<<(unsigned short value) noexcept = 0;

		virtual basic_cipher& operator<<(int value) noexcept = 0;
		virtual basic_cipher& operator<<(unsigned int value) noexcept = 0;

		virtual basic_cipher& operator<<(long value) noexcept = 0;
		virtual basic_cipher& operator<<(unsigned long value) noexcept = 0;

		virtual basic_cipher& operator<<(long long value) noexcept = 0;
		virtual basic_cipher& operator<<(unsigned long long value) noexcept = 0;

		virtual basic_cipher& operator<<(const std::string& str) noexcept = 0;
	};
}
