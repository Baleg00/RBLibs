#pragma once

#include <string>

namespace rb::crypto
{
	class basic_hash
	{
	public:
		virtual ~basic_hash() noexcept = default;

		virtual basic_hash& write(const void* data, size_t size) noexcept = 0;
		virtual basic_hash& write(const std::string& str) noexcept = 0;

		virtual void digest(void* dest) noexcept = 0;
		virtual std::string hex_digest() noexcept = 0;

		virtual void clear() noexcept = 0;

		virtual basic_hash& operator<<(const std::string& str) noexcept = 0;

		virtual std::string operator()(const void* data, size_t size) noexcept = 0;
		virtual std::string operator()(const std::string& str) noexcept = 0;
	};
}
