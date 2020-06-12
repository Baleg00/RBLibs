#pragma once

#include <cstdint>
#include <cmath>
#include <numeric>

#include <string>
#include <array>
#include <algorithm>
#include <iterator>

#include <memory>

#include <exception>

namespace rb::crypto
{
	template<uint32_t R, char P, char... C>
	class base_transcoder final
	{
	public:
		static_assert(R < 256, "radix must be less than 256");
		static_assert(R != 0, "radix must be greater than 0");
		static_assert((R & (R - 1)) == 0, "radix must be a power of 2");

		static constexpr uint32_t RADIX = R;
		static constexpr char PAD_CHAR = P;
		inline static const std::array<char, sizeof...(C) + 1> CHARS{ C..., P };

	private:
		inline static const uint32_t BITS_PER_CHAR = static_cast<uint32_t>(std::floor(std::log2(RADIX)));
		inline static const uint32_t GROUP_SIZE = static_cast<uint32_t>(std::lcm(BITS_PER_CHAR, 8) / BITS_PER_CHAR);

	public:
		base_transcoder() = delete;
		base_transcoder(const base_transcoder&) = delete;
		base_transcoder(base_transcoder&&) = delete;

		void operator=(const base_transcoder&) = delete;
		void operator=(base_transcoder&&) = delete;

	public:
		static [[nodiscard]] bool check(const std::string& str) noexcept
		{
			return std::all_of(str.begin(), str.end(), [&](char c) { return std::find(CHARS.begin(), CHARS.end(), c) != CHARS.end(); });
		}

		static [[nodiscard]] std::string encode(const void* src, size_t size)
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(src);

			std::string encoded_str;

			uint32_t byte_pos = 0, bit_offs = 0;

			while (data[byte_pos] == 0 && byte_pos < size)
				byte_pos++;

			while (byte_pos < size)
			{
				uint32_t char_value;

				if (bit_offs + BITS_PER_CHAR > 8)
				{
					char_value = ((data[byte_pos] << bit_offs) & 0xFF) >> (8 - BITS_PER_CHAR);

					if (byte_pos + 1 < size)
						char_value |= data[byte_pos + 1] >> (16 - BITS_PER_CHAR - bit_offs);
				}
				else
				{
					char_value = ((data[byte_pos] << bit_offs) & 0xFF) >> (8 - BITS_PER_CHAR);
				}

				encoded_str += CHARS[char_value];

				byte_pos += (bit_offs + BITS_PER_CHAR >= 8) ? 1 : 0;
				bit_offs = (bit_offs + BITS_PER_CHAR) % 8;
			}

			const uint32_t padding = GROUP_SIZE - ((size * 8 + BITS_PER_CHAR - 1) / BITS_PER_CHAR) % GROUP_SIZE;

			if (padding < GROUP_SIZE)
				for (uint32_t i = 0; i < padding; i++)
					encoded_str += PAD_CHAR;

			return encoded_str;
		}

		static void decode(const std::string& str, void* dest, size_t size)
		{
			if (!check(str))
				throw std::invalid_argument("string contains invalid characters");

			uint8_t* data = reinterpret_cast<uint8_t*>(dest);

			for (uint32_t char_pos = 0; (char_pos < str.size()) && (char_pos * BITS_PER_CHAR < size * 8) && (str[char_pos] != PAD_CHAR); char_pos++)
			{
				const auto char_it = std::find(CHARS.begin(), CHARS.end(), str[char_pos]);
				const uint8_t char_value = static_cast<uint8_t>(std::distance(CHARS.begin(), char_it));
				
				uint32_t bit_pos = char_pos * BITS_PER_CHAR;
				
				if ((bit_pos % 8) + BITS_PER_CHAR > 8)
				{
					data[bit_pos / 8] |= char_value >> (BITS_PER_CHAR - 8 + (bit_pos % 8));
					data[bit_pos / 8 + 1] |= char_value << (16 - BITS_PER_CHAR - (bit_pos % 8));
				}
				else
				{
					data[bit_pos / 8] |= char_value << (8 - BITS_PER_CHAR - (bit_pos % 8));
				}
			}
		}
	};

	using base16 = base_transcoder<16, '=', 
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	>;

	using base32 = base_transcoder<32, '=',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
		'Y', 'Z', '2', '3', '4', '5', '6', '7'
	>;

	using base32hex = base_transcoder<32, '=',
		'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 
		'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
		'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'
	>;

	using base64 = base_transcoder<64, '=',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
		'w', 'x', 'y', 'z', '0', '1', '2', '3', 
		'4', '5', '6', '7', '8', '9', '+', '/'
	>;

	using base64url = base_transcoder<64, '=',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '-', '_'
	>;

	/*
	using base58 = base_transcoder<58, '=',
		'1', '2', '3', '4', '5', '6', '7', '8',
		'9', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
		'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
		'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
		'h', 'i', 'j', 'k', 'm', 'n', 'o', 'p',
		'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
		'y', 'z'
	>;
	*/

	inline namespace literals
	{
		[[nodiscard]] std::string operator""_base16(const char* str, size_t size)
		{
			return base16::encode(str, size);
		}

		[[nodiscard]] std::string operator""_base32(const char* str, size_t size)
		{
			return base32::encode(str, size);
		}

		[[nodiscard]] std::string operator""_base32hex(const char* str, size_t size)
		{
			return base32hex::encode(str, size);
		}

		[[nodiscard]] std::string operator""_base64(const char* str, size_t size)
		{
			return base64::encode(str, size);
		}

		[[nodiscard]] std::string operator""_base64url(const char* str, size_t size)
		{
			return base64url::encode(str, size);
		}
	}
}
