#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

namespace rb::bit
{
	template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
	[[nodiscard]] constexpr T rotl(const T& x, uint32_t s) noexcept
	{
		constexpr uint32_t N = std::numeric_limits<T>::digits;
		const uint32_t r = s % N;

		if (r == 0)
			return x;

		return (x << r) | (x >> (N - r));
	}

	template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
	[[nodiscard]] constexpr T rotr(const T& x, uint32_t s) noexcept
	{
		constexpr uint32_t N = std::numeric_limits<T>::digits;
		const uint32_t r = s % N;

		if (r == 0)
			return x;

		return (x >> r) | (x << (N - r));
	}
}
