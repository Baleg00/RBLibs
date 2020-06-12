#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>

namespace rb::bit
{
	/**
	 * \brief Implementation for big-endian write function.
	 */
	template<typename T, size_t... N>
	constexpr void _write_be_impl(void* dest, T value, std::index_sequence<N...>) noexcept
	{
		/*
		 * This function uses fold expressions to enable compile-time evaluation.
		 * The template parameter pack N is an index sequence from 0 up to the number of bytes in type T.
		 * Extract bytes from value in big-endian order.
		 * Reinterpret dest as a byte array and write values to the appropriate index.
		 */

		((reinterpret_cast<uint8_t*>(dest)[N] = ((value >> ((sizeof(T) - 1 - N) * 8)) & 0xFF)), ...);
	}

	/**
	 * \brief Implementation for little-endian write function.
	 */
	template<typename T, size_t... N>
	constexpr void _write_le_impl(void* dest, T value, std::index_sequence<N...>) noexcept
	{
		((reinterpret_cast<uint8_t*>(dest)[N] = ((value >> (N * 8)) & 0xFF)), ...);
	}

	/**
	 * \brief Writes integer into memory in big-endian byte order.
	 *
	 * \param dest Memory address to write to.
	 * \param value Value to write to memory.
	 */
	template<typename T>
	constexpr void write_be(void* dest, T value) noexcept
	{
		static_assert(std::is_integral_v<T>, "value must be an integral");

		/*
		 * Make T unsigned in order to avoid undefined behaviour when performing bitwise operations.
		 * The template parameter pack N is implicitly deduced from the third unnamed argument
		 * which is and index sequence from 0 up to the size of type T.
		 */

		_write_be_impl<std::make_unsigned_t<T>>(dest, value, std::make_index_sequence<sizeof(T)>{});
	}

	/**
	 * \brief Writes integer into memory in little-endian byte order.
	 *
	 * \param dest Memory address to write to.
	 * \param value Value to write to memory.
	 */
	template<typename T>
	constexpr void write_le(void* dest, T value) noexcept
	{
		static_assert(std::is_integral_v<T>, "value must be an integral");

		_write_le_impl<std::make_unsigned_t<T>>(dest, value, std::make_index_sequence<sizeof(T)>{});
	}

	/**
	 * \brief Implementation for big-endian read function.
	 */
	template<typename T, size_t... N>
	[[nodiscard]] constexpr T _read_be_impl(const void* src, std::index_sequence<N...>) noexcept
	{
		/*
		 * Reinterpret src as a byte array. 
		 * Read bytes, shift them into the appropriate big-endian poistion and bitwise-or them together.
		 */

		return ((static_cast<T>(reinterpret_cast<const uint8_t*>(src)[N]) << ((sizeof(T) - 1 - N)) * 8) | ...);
	}

	/**
	 * \brief Implementation for little-endian read function.
	 */
	template<typename T, size_t... N>
	[[nodiscard]] constexpr T _read_le_impl(const void* src, std::index_sequence<N...>) noexcept
	{
		return ((static_cast<T>(reinterpret_cast<const uint8_t*>(src)[N]) << (N * 8)) | ...);
	}

	/**
	 * \brief Reads big-endian integer from memory.
	 *
	 * \param src Memory address to read from.
	 * \return Value read from memory.
	 */
	template<typename T>
	[[nodiscard]] constexpr T read_be(const void* src) noexcept
	{
		static_assert(std::is_integral_v<T>, "return type must be integral");

		/*
		 * Make T unsigned in order to avoid undefined behaviour when performing bitwise operations.
		 * The template parameter pack N is implicitly deduced from the second unnamed argument
		 * which is and index sequence from 0 up to the size of type T.
		 */

		return static_cast<T>(_read_be_impl<std::make_unsigned_t<T>>(src, std::make_index_sequence<sizeof(T)>{}));
	}

	/**
	 * \brief Reads little-endian integer from memory.
	 *
	 * \param src Memory address to read from.
	 * \return Value read from memory.
	 */
	template<typename T>
	[[nodiscard]] constexpr T read_le(const void* src) noexcept
	{
		static_assert(std::is_integral_v<T>, "return type must be integral");

		return static_cast<T>(_read_le_impl<std::make_unsigned_t<T>>(src, std::make_index_sequence<sizeof(T)>{}));
	}
}
