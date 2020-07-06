#pragma once

#include "math/bigint_impl.h"
#include "math/biguint_impl.h"

namespace rb::math
{
	using int128 = bigint<128>;
	using int256 = bigint<256>;
	using int512 = bigint<512>;
	using int1024 = bigint<1024>;
	using int2048 = bigint<2048>;

	using uint128 = biguint<128>;
	using uint256 = biguint<256>;
	using uint512 = biguint<512>;
	using uint1024 = biguint<1024>;
	using uint2048 = biguint<2048>;

	inline namespace literals
	{
		[[nodiscard]] constexpr int128 operator""_i128(const char* str, size_t size)
		{
			return int128(std::string_view(str, size));
		}

		[[nodiscard]] constexpr int256 operator""_i256(const char* str, size_t size)
		{
			return int256(std::string_view(str, size));
		}

		[[nodiscard]] constexpr int512 operator""_i512(const char* str, size_t size)
		{
			return int512(std::string_view(str, size));
		}

		[[nodiscard]] constexpr int1024 operator""_i1024(const char* str, size_t size)
		{
			return int1024(std::string_view(str, size));
		}

		[[nodiscard]] constexpr int2048 operator""_i2048(const char* str, size_t size)
		{
			return int2048(std::string_view(str, size));
		}

		[[nodiscard]] constexpr uint128 operator""_u128(const char* str, size_t size)
		{
			return uint128(std::string_view(str, size));
		}

		[[nodiscard]] constexpr uint256 operator""_u256(const char* str, size_t size)
		{
			return uint256(std::string_view(str, size));
		}

		[[nodiscard]] constexpr uint512 operator""_u512(const char* str, size_t size)
		{
			return uint512(std::string_view(str, size));
		}

		[[nodiscard]] constexpr uint1024 operator""_u1024(const char* str, size_t size)
		{
			return uint1024(std::string_view(str, size));
		}

		[[nodiscard]] constexpr uint2048 operator""_u2048(const char* str, size_t size)
		{
			return uint2048(std::string_view(str, size));
		}
	}
}
