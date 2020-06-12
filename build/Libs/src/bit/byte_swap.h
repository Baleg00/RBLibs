#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>

namespace rb::bit
{
	/**
	 * \brief The implementation of the byte-swap algorithm.
	 */
	template<typename T, size_t... N>
	[[nodiscard]] constexpr T _bswap_impl(T value, std::index_sequence<N...>) noexcept
	{
		/*
		 * This function uses fold expressions to enable compile-time evaluation.
		 * The template parameter pack N is an index sequence from 0 up to the number of bytes in type T.
		 * First we extract a single byte by shifting it to the rightmost place (the place of the least-significant byte) 
		 * and bitwise-and it with 0xFF to cut off any remaining bits on the left. Then we shift the byte to the swapped place. 
		 * Repeat this for every byte and bitwise-or the results together.
		 */

		return ((((value >> (N * 8)) & 0xFF) << (sizeof(T) - 1 - N) * 8) | ...);
	}

	/**
	 * \brief Swaps the bytes of an integer.
	 * 
	 * \param value The integer to swap the bytes of.
	 * \return The integer with swapped bytes.
	 */
	template<typename T>
	[[nodiscard]] constexpr T bswap(T value) noexcept
	{
		static_assert(std::is_integral_v<T>, "value must be an integral");

		/*
		 * Make T unsigned in order to avoid undefined behaviour when performing bitwise operations.
		 * The template parameter pack N is implicitly deduced from the second unnamed argument
		 * which is and index sequence from 0 up to the number of bytes in type T.
		 */

		return static_cast<T>(_bswap_impl<std::make_unsigned_t<T>>(value, std::make_index_sequence<sizeof(T)>{}));
	}
}
