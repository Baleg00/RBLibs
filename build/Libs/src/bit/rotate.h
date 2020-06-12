#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

namespace rb::bit
{
	/**
	 * \brief Bitwise left-rotate operation.
	 * 
	 * \param value Value to be rotated.
	 * \param r Rotation amount.
	 * \return Rotated value.
	 */
	template<typename T>
	[[nodiscard]] constexpr T rotl(T value, uint32_t r) noexcept
	{
		static_assert(std::is_integral_v<T>, "value must be an integral");

		constexpr uint32_t N = std::numeric_limits<T>::digits;
		r %= N;

		return (value << r) | (value >> (N - r));
	}

	/**
	 * \brief Bitwise right-rotate operation.
	 *
	 * \param value Value to be rotated.
	 * \param r Rotation amount.
	 * \return Rotated value.
	 */
	template<typename T>
	[[nodiscard]] constexpr T rotr(T value, uint32_t r) noexcept
	{
		static_assert(std::is_integral_v<T>, "value must be an integral");

		constexpr uint32_t N = std::numeric_limits<T>::digits;
		r %= N;

		return (value >> r) | (value << (N - r));
	}
}
