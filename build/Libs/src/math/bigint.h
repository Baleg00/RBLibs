#pragma once

#include <array>

#include <string>
#include <cctype>

#include <cstdint>
#include <limits>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <utility>
#include <functional>
#include <initializer_list>
#include <exception>

#include "bit/endian.h"

namespace rb::math
{
	template<size_t B>
	class bigint
	{
	public:
		static_assert(B > 0, "number of bits cannot be 0");

		static constexpr size_t BIT_SIZE = B;
		static constexpr size_t BYTE_SIZE = (B + 7) / 8;

		using my_type = bigint<BIT_SIZE>;
		using data_type = std::array<uint8_t, BYTE_SIZE>;

	public:
		constexpr bigint() noexcept
			: m_data{ 0 }
		{
		}

		constexpr bigint(const my_type& other) noexcept
			: m_data(other.m_data)
		{
		}

		constexpr bigint(my_type&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

		bigint(std::initializer_list<uint8_t> il) noexcept
			: bigint()
		{
			std::copy(il.begin(), il.end(), m_data.data());
		}

		explicit bigint(const std::string& str)
			: bigint()
		{
			if (!std::all_of(str.begin(), str.end(), std::isxdigit))
				throw std::invalid_argument("string contains non-hexadecimal characters");

			std::string str_padded = str;

			if (str_padded.size() % 2)
				str_padded.insert(str_padded.begin(), '0');

			for (size_t i = 0; (i < str_padded.size()) && (i / 8 < m_data.size()); i += 2)
			{
				std::string hex_byte = str_padded.substr(str_padded.size() - 2 - i, 2);
				m_data[i / 2] = static_cast<uint8_t>(std::stoul(hex_byte, nullptr, 16));
			}
		}

		explicit bigint(const uint8_t* data, size_t size) noexcept
			: bigint()
		{
			std::copy_n(data, std::min(m_data.size(), size), m_data.data());
		}

		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		constexpr bigint(T value) noexcept
			: bigint()
		{
			std::make_unsigned_t<T> u_value = static_cast<std::make_unsigned_t<T>>(value);

			if (value < 0)
			{
				m_data[BYTE_SIZE - 1] = 0x80;
				u_value &= ~(1 << (sizeof(T) * 8 - 1));
			}

			for (size_t i = 0; i < std::min(BYTE_SIZE, sizeof(T)); i++)
				m_data[i] = static_cast<uint8_t>((u_value >> (i * 8)) & 0xFF);
		}

		template<size_t _B>
		explicit bigint(const bigint<_B>& other) noexcept
			: bigint()
		{
			if constexpr (bigint<_B>::BYTE_SIZE < BYTE_SIZE && other < 0)
			{
				bigint<_B> neg_other = -other;

				std::copy(neg_other.m_data.begin(), neg_other.m_data.end(), m_data.begin());

				*this = -(*this);
			}
			else
			{
				std::copy(other.m_data.begin(), other.m_data.begin() + std::min(bigint<_B>::BYTE_SIZE, BYTE_SIZE), m_data.begin());
			}
		}

	private:
		template<size_t... I>
		[[nodiscard]] inline my_type bitwise_and_impl(const my_type& rhs, std::index_sequence<I...>) const noexcept
		{
			my_type result = *this;
			((result.m_data[I] &= rhs.m_data[I]), ...);
			return result;
		}

		template<size_t... I>
		[[nodiscard]] inline my_type bitwise_or_impl(const my_type& rhs, std::index_sequence<I...>) const noexcept
		{
			my_type result = *this;
			((result.m_data[I] |= rhs.m_data[I]), ...);
			return result;
		}

		template<size_t... I>
		[[nodiscard]] inline my_type bitwise_xor_impl(const my_type& rhs, std::index_sequence<I...>) const noexcept
		{
			my_type result = *this;
			((result.m_data[I] ^= rhs.m_data[I]), ...);
			return result;
		}

		template<size_t... I>
		[[nodiscard]] inline my_type bitwise_not_impl(std::index_sequence<I...>) const noexcept
		{
			my_type result;
			((result.m_data[I] = ~m_data[I]), ...);
			return result;
		}

		[[nodiscard]] std::pair<my_type, my_type> division_impl(const my_type& rhs) const
		{
			if (rhs.is_zero())
				throw std::domain_error("division by 0");

			if (is_zero())
				return { 0, 0 };

			my_type n = *this, d = rhs;

			bool n_changed = false, d_changed = false;

			if (n.sign() < 0)
			{
				n = -n;
				n_changed = true;
			}

			if (d.sign() < 0)
			{
				d = -d;
				d_changed = true;
			}

			my_type q, r;

			for (int32_t i = n.bits() - 1; i >= 0; i--)
			{
				r <<= 1;
				r.m_data[0] = (r.m_data[0] & 0xFE) | ((n.m_data[i / 8] & (1 << (i % 8))) >> (i % 8));

				if (r >= d)
				{
					r -= d;
					q.m_data[i / 8] |= 1 << (i % 8);
				}
			}

			if (n_changed && d_changed)
				return { q, -r };
			else if (n_changed && !d_changed)
				return { -q, rhs - r };
			else if (!n_changed && d_changed)
				return { -q, rhs + r };

			return { q, r };
		}

	public:
		[[nodiscard]] int sign() const noexcept
		{
			return (m_data[BYTE_SIZE - 1] & 0x80) == 0 ? 1 : -1;
		}

		[[nodiscard]] bool is_zero() const noexcept
		{
			return std::all_of(m_data.begin(), m_data.end(), [](uint8_t v) { return v == 0; });
		}

		[[nodiscard]] size_t bits() const noexcept
		{
			data_type::const_reverse_iterator it = m_data.rbegin();

			while (*it == 0 && it != m_data.rend())
				it++;

			size_t num_bits = (std::distance(it, m_data.rend()) - 1) * 8;

			if (it != m_data.rend())
			{
				uint8_t byte = *it;

				while (byte != 0)
				{
					byte >>= 1;
					num_bits++;
				}
			}

			return num_bits;
		}

		[[nodiscard]] my_type sqrt() const
		{
			if (*this < 0)
				throw std::domain_error("cannot take square root of negative");

			if (*this < 2)
				return *this;

			my_type small_candidate = (*this >> 2).sqrt() << 1;
			my_type large_candidate = small_candidate + 1;

			if (large_candidate * large_candidate > * this)
				return small_candidate;

			return large_candidate;
		}

		[[nodiscard]] my_type pow(my_type n) const
		{
			if (n < 0)
				throw std::domain_error("exponent cannot be negative");

			if (n.is_zero())
				return 1;

			my_type x = *this, y = 1;

			while (n > 1)
			{
				if (n & 1)
				{
					y = x * y;
					x *= x;
					n = (n - 1) / 2;
				}
				else
				{
					x *= x;
					n /= 2;
				}
			}

			return x * y;
		}

	public:
		[[nodiscard]] uint8_t* data() noexcept
		{
			return m_data.data();
		}

		[[nodiscard]] const uint8_t* data() const noexcept
		{
			return m_data.data();
		}

	public:
		my_type& operator=(const my_type& rhs) noexcept
		{
			m_data = rhs.m_data;
			return *this;
		}

		my_type& operator=(my_type&& rhs) noexcept
		{
			m_data = std::move(rhs.m_data);
			return *this;
		}

		my_type& operator=(uint64_t rhs) noexcept
		{
			*this = my_type(rhs);
			return *this;
		}

		my_type& operator=(std::initializer_list<uint8_t> rhs) noexcept
		{
			*this = my_type(rhs);
			return *this;
		}

		my_type& operator=(const std::string& rhs) noexcept
		{
			*this = my_type(rhs);
			return *this;
		}

	public:
		[[nodiscard]] my_type operator+(const my_type& rhs) const noexcept
		{
			if (is_zero())
				return rhs;

			if (rhs.is_zero())
				return *this;

			my_type result = rhs;

			for (size_t i = 0, carry = 0; i < BYTE_SIZE; i++)
			{
				uint32_t temp = carry + m_data[i] + result.m_data[i];
				result.m_data[i] = temp % 256;
				carry = temp / 256;
			}

			return result;
		}

		[[nodiscard]] my_type operator-(const my_type& rhs) const noexcept
		{
			return *this + (-rhs);
		}

		my_type operator+() const noexcept
		{
			return *this;
		}

		my_type operator-() const noexcept
		{
			return ~(*this) + 1;
		}

		my_type operator++(int) noexcept
		{
			my_type temp = *this;
			*this += 1;
			return temp;
		}

		my_type operator--(int) noexcept
		{
			my_type temp = *this;
			*this -= 1;
			return temp;
		}

		my_type& operator++() noexcept
		{
			*this += 1;
			return *this;
		}

		my_type& operator--() noexcept
		{
			*this -= 1;
			return *this;
		}

		[[nodiscard]] my_type operator*(const my_type& rhs) const noexcept
		{
			if (is_zero() || rhs.is_zero())
				return 0;

			my_type a = *this, b = rhs;

			bool change_sign = false;

			if (a.sign() * b.sign() < 0)
				change_sign = true;

			if (a.sign() < 0)
				a = -a;

			if (b.sign() < 0)
				b = -b;

			my_type result;

			for (size_t i = 0; i < BYTE_SIZE; i++)
			{
				for (size_t j = 0, carry = 0; j < BYTE_SIZE - i; j++)
				{
					uint32_t temp = carry + a.m_data[i] * b.m_data[j] + result.m_data[i + j];
					result.m_data[i + j] = temp % 256;
					carry = temp / 256;
				}
			}

			if (change_sign)
				return -result;

			return result;
		}

		[[nodiscard]] my_type operator/(const my_type& rhs) const
		{
			return division_impl(rhs).first;
		}

		[[nodiscard]] my_type operator%(const my_type& rhs) const
		{
			return division_impl(rhs).second;
		}

		my_type& operator+=(const my_type& rhs) noexcept
		{
			*this = *this + rhs;
			return *this;
		}

		my_type& operator-=(const my_type& rhs) noexcept
		{
			*this = *this - rhs;
			return *this;
		}

		my_type& operator*=(const my_type& rhs) noexcept
		{
			*this = *this * rhs;
			return *this;
		}

		my_type& operator/=(const my_type& rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		my_type& operator%=(const my_type& rhs)
		{
			*this = *this % rhs;
			return *this;
		}

	public:
		[[nodiscard]] my_type operator&(const my_type& rhs) const noexcept
		{
			return bitwise_and_impl(rhs, std::make_index_sequence<BYTE_SIZE>{});
		}

		[[nodiscard]] my_type operator|(const my_type& rhs) const noexcept
		{
			return bitwise_or_impl(rhs, std::make_index_sequence<BYTE_SIZE>{});
		}

		[[nodiscard]] my_type operator^(const my_type& rhs) const noexcept
		{
			return bitwise_xor_impl(rhs, std::make_index_sequence<BYTE_SIZE>{});
		}

		[[nodiscard]] my_type operator<<(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (rhs >= BIT_SIZE)
				return my_type();

			my_type result;

			const uint32_t byte_pos = rhs / 8;
			const uint32_t bit_offs = rhs % 8;

			for (uint32_t i = byte_pos; i < BYTE_SIZE; i++)
				result.m_data[i] |= (m_data[i - byte_pos] << bit_offs) | (i - byte_pos > 0 ? (m_data[i - byte_pos - 1] >> (8 - bit_offs)) : 0);

			return result;
		}

		[[nodiscard]] my_type operator<<(const my_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			if (rhs.m_data[0] >= BIT_SIZE)
				return my_type();

			uint32_t shift = rb::bit::read_le<uint32_t>(rhs.m_data.data());

			return *this << shift;
		}

		[[nodiscard]] my_type operator>>(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (rhs >= BIT_SIZE)
				return my_type();

			my_type result;

			const uint32_t byte_pos = rhs / 8;
			const uint32_t bit_offs = rhs % 8;

			for (uint32_t i = 0; i < BYTE_SIZE - byte_pos; i++)
				result.m_data[i] |= (m_data[byte_pos + i] >> bit_offs) | (byte_pos + i + 1 < BYTE_SIZE ? (m_data[byte_pos + i + 1] << (8 - bit_offs)) : 0);

			return result;
		}

		[[nodiscard]] my_type operator>>(const my_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			if (rhs.m_data[0] >= BIT_SIZE)
				return my_type();

			uint32_t shift = rb::bit::read_le<uint32_t>(rhs.m_data.data());

			return *this >> shift;
		}

		[[nodiscard]] my_type operator~() const noexcept
		{
			return bitwise_not_impl(std::make_index_sequence<BYTE_SIZE>{});
		}

		my_type& operator&=(const my_type& rhs) noexcept
		{
			*this = *this & rhs;
			return *this;
		}

		my_type& operator|=(const my_type& rhs) noexcept
		{
			*this = *this | rhs;
			return *this;
		}

		my_type& operator^=(const my_type& rhs) noexcept
		{
			*this = *this ^ rhs;
			return *this;
		}

		my_type& operator>>=(uint32_t rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		my_type& operator>>=(const my_type& rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		my_type& operator<<=(uint32_t rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

		my_type& operator<<=(const my_type& rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

	public:
		[[nodiscard]] bool operator<(const my_type& rhs) const noexcept
		{
			if (sign() < rhs.sign())
				return true;

			if (sign() > rhs.sign())
				return false;

			if (sign() < 0 && rhs.sign() < 0)
				return -(*this) > -rhs;

			for (int32_t i = m_data.size() - 1; i >= 0; i--)
				if (m_data[i] != rhs.m_data[i])
					return m_data[i] < rhs.m_data[i];

			return false;
		}

		[[nodiscard]] bool operator<=(const my_type& rhs) const noexcept
		{
			return !(*this > rhs);
		}

		[[nodiscard]] bool operator>(const my_type& rhs) const noexcept
		{
			if (sign() > rhs.sign())
				return true;

			if (sign() < rhs.sign())
				return false;

			if (sign() < 0 && rhs.sign() < 0)
				return -(*this) < -rhs;

			for (int32_t i = m_data.size() - 1; i >= 0; i--)
				if (m_data[i] != rhs.m_data[i])
					return m_data[i] > rhs.m_data[i];

			return false;
		}

		[[nodiscard]] bool operator>=(const my_type& rhs) const noexcept
		{
			return !(*this < rhs);
		}

		[[nodiscard]] bool operator==(const my_type& rhs) const noexcept
		{
			for (size_t i = 0; i < m_data.size(); i++)
				if (m_data[i] != rhs.m_data[i])
					return false;

			return true;
		}

		[[nodiscard]] bool operator!=(const my_type& rhs) const noexcept
		{
			return !(*this == rhs);
		}

	public:
		friend std::ostream& operator<<(std::ostream& os, my_type v)
		{
			if (v.sign() < 0)
			{
				os << "-";
				v = -v;
			}

			data_type::const_reverse_iterator it = v.m_data.rbegin();

			while (*it == 0 && it != v.m_data.rend())
				it++;

			if (it == v.m_data.rend())
				os << "0";

			while (it != v.m_data.rend())
				os << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(*(it++));

			return os;
		}

		friend std::istream& operator>>(std::istream& is, my_type& v)
		{
			std::string hex_str;
			is >> hex_str;

			if (hex_str[0] == '-')
			{
				hex_str.erase(hex_str.begin());
				v = -my_type(hex_str);
			}
			else
			{
				v = my_type(hex_str);
			}

			return is;
		}

	public:
		[[nodiscard]] explicit operator bool() const noexcept
		{
			return !is_zero();
		}

		[[nodiscard]] explicit operator std::string() const noexcept
		{
			std::stringstream strm;
			strm << *this;
			return strm.str();
		}

	private:
		data_type m_data;
	};

	using int128 = bigint<128>;
	using int256 = bigint<256>;
	using int512 = bigint<512>;
	using int1024 = bigint<1024>;
	using int2048 = bigint<2048>;

	inline namespace literals
	{
		int128 operator""_i128(const char* str, size_t size)
		{
			return int128(std::string(str, size));
		}

		int256 operator""_i256(const char* str, size_t size)
		{
			return int256(std::string(str, size));
		}

		int512 operator""_i512(const char* str, size_t size)
		{
			return int512(std::string(str, size));
		}

		int1024 operator""_i1024(const char* str, size_t size)
		{
			return int1024(std::string(str, size));
		}

		int2048 operator""_i2048(const char* str, size_t size)
		{
			return int2048(std::string(str, size));
		}
	}
}
