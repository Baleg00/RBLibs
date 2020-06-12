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
	/**
	 * \brief Template class for base transcoding.
	 * 
	 * \tparam R Radix (base). Must be one of 1, 2, 4, 8, 16, 32, 64 or 128.
	 * \tparam P Padding character.
	 * \tparam C Base characters (digits) in increasing order.
	 */
	template<uint32_t R, char P, char... C>
	class base_transcoder final
	{
	public:
		static_assert(R < 256, "radix must be less than 256");
		static_assert(R != 0, "radix must be greater than 0");
		static_assert((R & (R - 1)) == 0, "radix must be a power of 2");
		static_assert(sizeof...(C) == R, "number of characters must be equal to radix");

		static constexpr uint32_t RADIX = R;
		static constexpr char PAD_CHAR = P;
		inline static const std::array<char, sizeof...(C) + 1> CHARS{ C..., P };

	private:
		inline static const uint32_t BITS_PER_CHAR = static_cast<uint32_t>(std::floor(std::log2(RADIX)));
		inline static const uint32_t GROUP_SIZE = static_cast<uint32_t>(std::lcm(BITS_PER_CHAR, 8) / BITS_PER_CHAR); // Number of characters that line up perfectly with bytes.

	public:
		// There can be no instances of base transcoders because there is no point in doing that.

		base_transcoder() = delete;
		base_transcoder(const base_transcoder&) = delete;
		base_transcoder(base_transcoder&&) = delete;

		void operator=(const base_transcoder&) = delete;
		void operator=(base_transcoder&&) = delete;

	public:
		/**
		 * \brief Checks if all characters of a string are valid for this base.
		 * 
		 * \param str String to be checked.
		 * \return True if string is valid, false otherwise.
		 */
		static [[nodiscard]] bool check(const std::string& str) noexcept
		{
			return std::all_of(str.begin(), str.end(), [&](char c) { return std::find(CHARS.begin(), CHARS.end(), c) != CHARS.end(); });
		}

		/**
		 * \brief Encodes binary data into base string.
		 * 
		 * \param src Memory to read from.
		 * \param size Size of memory range to be encoded.
		 * \return Encoded string.
		 */
		static [[nodiscard]] std::string encode(const void* src, size_t size) noexcept
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(src);

			std::string encoded_str;

			uint32_t byte_pos = 0; // Position inside data to be encoded
			uint32_t bit_offs = 0; // Offset inside current byte

			// Skip leading zeroes
			while (data[byte_pos] == 0 && byte_pos < size)
				byte_pos++;

			// Loop while there are bytes to process
			while (byte_pos < size)
			{
				uint32_t char_value; // Value of current encoded character
				
				char_value = ((data[byte_pos] << bit_offs) & 0xFF) >> (8 - BITS_PER_CHAR); // Extract bits from current byte

				if (bit_offs + BITS_PER_CHAR > 8 && byte_pos + 1 < size) // We need bits from the next byte
					char_value |= data[byte_pos + 1] >> (16 - BITS_PER_CHAR - bit_offs);

				encoded_str += CHARS[char_value];

				byte_pos += (bit_offs + BITS_PER_CHAR >= 8) ? 1 : 0; // Step to next byte of more bits are required
				bit_offs = (bit_offs + BITS_PER_CHAR) % 8; // Update offset
			}

			const uint32_t padding = GROUP_SIZE - ((size * 8 + BITS_PER_CHAR - 1) / BITS_PER_CHAR) % GROUP_SIZE; // Number of padding characters

			// Add padding if needed
			if (padding < GROUP_SIZE)
				for (uint32_t i = 0; i < padding; i++)
					encoded_str += PAD_CHAR;

			return encoded_str;
		}

		/**
		 * \brief Decodes base string into binary data.
		 *
		 * \param str String to decode.
		 * \param dest Memory to write to.
		 * \param size Size of available memory.
		 */
		static void decode(const std::string& str, void* dest, size_t size)
		{
			// Check if string contains only valid characters
			if (!check(str))
				throw std::invalid_argument("string contains invalid characters");

			uint8_t* data = reinterpret_cast<uint8_t*>(dest);

			/*
			 * Loop through all characters until one of the following happens:
			 * 1. There are no more characters.
			 * 2. There is no more memory to write to.
			 * 3. A padding character is encountered.
			 */
			for (uint32_t char_pos = 0; (char_pos < str.size()) && (char_pos * BITS_PER_CHAR < size * 8) && (str[char_pos] != PAD_CHAR); char_pos++)
			{
				const auto char_it = std::find(CHARS.begin(), CHARS.end(), str[char_pos]);
				const uint8_t char_value = static_cast<uint8_t>(std::distance(CHARS.begin(), char_it)); // Value of current character is equal to its index
				
				const uint32_t bit_pos = char_pos * BITS_PER_CHAR; // Number of bits processed
				
				if ((bit_pos % 8) + BITS_PER_CHAR > 8) // Character leans over to next byte
				{
					data[bit_pos / 8] |= char_value >> (BITS_PER_CHAR - 8 + (bit_pos % 8));
					data[bit_pos / 8 + 1] |= char_value << (16 - BITS_PER_CHAR - (bit_pos % 8));
				}
				else // Character is inside only current byte
				{
					data[bit_pos / 8] |= char_value << (8 - BITS_PER_CHAR - (bit_pos % 8));
				}
			}
		}
	};

	// Standard base16 transcoder
	using base16 = base_transcoder<16, '=', 
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	>;

	// Standard base32 transcoder
	using base32 = base_transcoder<32, '=',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
		'Y', 'Z', '2', '3', '4', '5', '6', '7'
	>;

	// Standard base32 transcoder with extended hex alphabet
	using base32hex = base_transcoder<32, '=',
		'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 
		'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
		'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'
	>;

	// Standard base64 transcoder
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

	// Standard base64 transcoder with url and filename safe alphabet
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
