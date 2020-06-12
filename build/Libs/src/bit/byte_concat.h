#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>

namespace rb::bit
{
	/**
	 * \brief The implementation for the big-endian byte-concatenation algorithm.
	 */
	template<typename T, typename U, size_t... N>
	[[nodiscard]] T _bconcat_be_impl(const U* values, std::index_sequence<N...>) noexcept
	{
		/*
		 * The template parameter pack N is an index sequence from 0 up to the number of times U fits into T.
		 * We cast the current value to T and shift it left to the appropriate position.
		 * Repeat this for every value and bitwise-or them together.
		 */

		return ((static_cast<T>(values[N]) << ((sizeof...(N) - 1 - N) * 8)) | ...);
	}

	/**
	 * \brief The implementation for the little-endian byte-concatenation algorithm.
	 */
	template<typename U, typename T, size_t... N>
	[[nodiscard]] U _bconcat_le_impl(const T* values, std::index_sequence<N...>) noexcept
	{
		return ((static_cast<U>(values[N]) << (N * 8)) | ...);
	}

	/**
	 * \brief Concatenates integrals in big-endian order.
	 * 
	 * \param values The memory address to read from.
	 * \return Concatenated big-endian integer.
	 */
	template<typename T, typename U>
	[[nodiscard]] T bconcat_be(const U* values) noexcept
	{
		static_assert(std::is_integral_v<T>, "return type must be integral");
		static_assert(std::is_integral_v<U>, "value type must be integral");
		static_assert(sizeof(T) >= sizeof(U), "return type cannot be smaller than value type");

		/*
		 * We make T and U unsigned in order to avoid undefined behaviour when performing bitwise operations.
		 * The template parameter pack N is implicitly deduced from the second unnamed argument
		 * which is and index sequence from 0 up to the number of times U fits into T.
		 */
		return static_cast<T>(_bconcat_be_impl<std::make_unsigned_t<T>>(reinterpret_cast<const std::make_unsigned_t<U>*>(values), std::make_index_sequence<sizeof(T) / sizeof(U)>{}));
	}

	/**
	 * \brief Concatenates integrals in little-endian order.
	 *
	 * \param values The memory address to read from.
	 * \return Concatenated little-endian integer.
	 */
	template<typename T, typename U>
	[[nodiscard]] T bconcat_le(const U* values) noexcept
	{
		static_assert(std::is_integral_v<T>, "return type must be integral");
		static_assert(std::is_integral_v<U>, "value type must be integral");
		static_assert(sizeof(T) >= sizeof(U), "return type cannot be smaller than value type");

		return static_cast<T>(_bconcat_le_impl<std::make_unsigned_t<T>>(reinterpret_cast<const std::make_unsigned_t<U>*>(values), std::make_index_sequence<sizeof(T) / sizeof(U)>{}));
	}
}
