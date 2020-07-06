#pragma once

#include <array>

#include <string_view>

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

		explicit constexpr bigint(const std::string_view& str)
			: bigint()
		{
			for (std::string_view::const_iterator it = str.begin(); it != str.end(); it++)
				if (!((*it >= '0' && *it <= '9') || (*it >= 'a' && *it <= 'f') || (*it >= 'A' && *it <= 'F')))
					throw std::invalid_argument("string contains non-hexadecimal characters");

			size_t i = 0, offs = str.size() % 2;

			if (offs)
			{
				char c = str[i++];
				m_data[str.size() / 2] = (c >= 'a') ? (c - 'a' + 10) : ((c >= 'A') ? (c - 'A' + 10) : (c - '0'));
			}

			for (; (i < str.size()) && (i / 2 + offs < BYTE_SIZE); i += 2)
			{
				char c = str[i];
				uint8_t byte = 0;

				byte |= ((c >= 'a') ? (c - 'a' + 10) : ((c >= 'A') ? (c - 'A' + 10) : (c - '0'))) << 4;
				c = str[i + 1];
				byte |= (c >= 'a') ? (c - 'a' + 10) : ((c >= 'A') ? (c - 'A' + 10) : (c - '0'));

				m_data[(str.size() - i - 1) / 2] = byte;
			}
		}

		explicit bigint(const uint8_t* data, size_t size) noexcept
			: bigint()
		{
			std::copy_n(data, std::min(BYTE_SIZE, size), m_data.data());
		}

		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		constexpr bigint(T value) noexcept
			: bigint()
		{
			using U = std::make_unsigned_t<T>;
			U u_value = static_cast<U>(value);

			if (value < 0)
				*this = ~(*this);

			for (size_t i = 0; i < std::min(BYTE_SIZE, sizeof(T)); i++)
				m_data[i] = static_cast<uint8_t>((u_value >> (i * 8)) & 0xFF);
		}

	private:
		[[nodiscard]] constexpr std::pair<my_type, my_type> division_impl(const my_type& rhs) const
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
		[[nodiscard]] static constexpr my_type ZERO() noexcept
		{
			return {};
		}

		[[nodiscard]] static constexpr my_type ONE() noexcept
		{
			return { 1 };
		}

		[[nodiscard]] static constexpr my_type MIN() noexcept
		{
			return ONE() << (B - 1);
		}

		[[nodiscard]] static constexpr my_type MAX() noexcept
		{
			return MIN() ^ ~ZERO();
		}

	public:
		[[nodiscard]] constexpr int sign() const noexcept
		{
			return (m_data[BYTE_SIZE - 1] & 0x80) == 0 ? 1 : -1;
		}

		[[nodiscard]] constexpr bool is_zero() const noexcept
		{
			for (size_t i = 0; i < BYTE_SIZE; i++)
				if (m_data[i] != 0)
					return false;

			return true;
		}

		[[nodiscard]] constexpr my_type abs() const noexcept
		{
			return sign() < 0 ? -(*this) : *this;
		}

		[[nodiscard]] constexpr size_t bits() const noexcept
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

		[[nodiscard]] constexpr my_type sqrt() const
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

		[[nodiscard]] constexpr my_type pow(my_type n) const
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

		[[nodiscard]] constexpr my_type log(const my_type& b) const
		{
			if (b <= 0)
				throw std::domain_error("logarithm base must be positive");

			if (*this <= 0)
				throw std::domain_error("cannot take logarithm of non-positive");

			my_type result;
			my_type x = *this;

			while (x >= b)
			{
				result++;
				x /= b;
			}

			return result;
		}

		[[nodiscard]] constexpr my_type log2() const
		{
			if (*this <= 0)
				throw std::domain_error("cannot take logarithm of non-positive");

			return bits() - 1;
		}

		[[nodiscard]] constexpr my_type gcd(my_type a) const noexcept
		{
			my_type b = *this;

			while (!b.is_zero())
			{
				my_type temp = b;
				b = a % b;
				a = temp;
			}

			return a;
		}

		[[nodiscard]] constexpr my_type lcm(my_type a) const noexcept
		{
			return (*this * a).abs() / gcd(a);
		}

	public:
		[[nodiscard]] constexpr uint8_t* data() noexcept
		{
			return m_data.data();
		}

		[[nodiscard]] constexpr const uint8_t* data() const noexcept
		{
			return m_data.data();
		}

	public:
		constexpr my_type& operator=(const my_type& rhs) noexcept
		{
			m_data = rhs.m_data;
			return *this;
		}

		constexpr my_type& operator=(my_type&& rhs) noexcept
		{
			m_data = std::move(rhs.m_data);
			return *this;
		}

		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		constexpr my_type& operator=(T rhs) noexcept
		{
			*this = my_type(rhs);
			return *this;
		}

	public:
		[[nodiscard]] constexpr my_type operator+(const my_type& rhs) const noexcept
		{
			my_type result;

			for (size_t i = 0, carry = 0; i < BYTE_SIZE; i++)
			{
				uint32_t temp = carry + m_data[i] + rhs.m_data[i];
				result.m_data[i] = temp % 256;
				carry = temp / 256;
			}

			return result;
		}

		[[nodiscard]] constexpr my_type operator-(const my_type& rhs) const noexcept
		{
			return *this + (-rhs);
		}

		[[nodiscard]] constexpr my_type operator+() const noexcept
		{
			return *this;
		}

		[[nodiscard]] constexpr my_type operator-() const noexcept
		{
			return ~(*this) + 1;
		}

		constexpr my_type operator++(int) noexcept
		{
			my_type temp = *this;
			*this += 1;
			return temp;
		}

		constexpr my_type operator--(int) noexcept
		{
			my_type temp = *this;
			*this -= 1;
			return temp;
		}

		constexpr my_type& operator++() noexcept
		{
			*this += 1;
			return *this;
		}

		constexpr my_type& operator--() noexcept
		{
			*this -= 1;
			return *this;
		}

		[[nodiscard]] constexpr my_type operator*(const my_type& rhs) const noexcept
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

		[[nodiscard]] constexpr my_type operator/(const my_type& rhs) const
		{
			return division_impl(rhs).first;
		}

		[[nodiscard]] constexpr my_type operator%(const my_type& rhs) const
		{
			return division_impl(rhs).second;
		}

		constexpr my_type& operator+=(const my_type& rhs) noexcept
		{
			*this = *this + rhs;
			return *this;
		}

		constexpr my_type& operator-=(const my_type& rhs) noexcept
		{
			*this = *this - rhs;
			return *this;
		}

		constexpr my_type& operator*=(const my_type& rhs) noexcept
		{
			*this = *this * rhs;
			return *this;
		}

		constexpr my_type& operator/=(const my_type& rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		constexpr my_type& operator%=(const my_type& rhs)
		{
			*this = *this % rhs;
			return *this;
		}

	public:
		[[nodiscard]] constexpr my_type operator&(const my_type& rhs) const noexcept
		{
			my_type result = *this;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] &= rhs.m_data[i];

			return result;
		}

		[[nodiscard]] constexpr my_type operator|(const my_type& rhs) const noexcept
		{
			my_type result = *this;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] |= rhs.m_data[i];

			return result;
		}

		[[nodiscard]] constexpr my_type operator^(const my_type& rhs) const noexcept
		{
			my_type result = *this;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] ^= rhs.m_data[i];

			return result;
		}

		[[nodiscard]] constexpr my_type operator<<(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (rhs >= BIT_SIZE)
				return 0;

			const uint32_t byte_pos = rhs / 8;
			const uint32_t bit_offs = rhs % 8;

			my_type result;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				if (i >= byte_pos)
					result.m_data[i] |= (m_data[i - byte_pos] << bit_offs) | (i - byte_pos > 0 ? (m_data[i - byte_pos - 1] >> (8 - bit_offs)) : 0);

			return result;
		}

		[[nodiscard]] constexpr my_type operator<<(const my_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			if (rhs.m_data[0] >= BIT_SIZE)
				return 0;

			uint32_t shift = rb::bit::read_le<uint32_t>(rhs.m_data.data());

			return *this << shift;
		}

		[[nodiscard]] constexpr my_type operator>>(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (rhs >= BIT_SIZE)
				return 0;

			const uint32_t byte_pos = rhs / 8;
			const uint32_t bit_offs = rhs % 8;

			my_type result;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				if (i < BYTE_SIZE - byte_pos)
					result.m_data[i] |= (m_data[i + byte_pos] >> bit_offs) | (byte_pos + i + 1 < BYTE_SIZE ? (m_data[byte_pos + i + 1] << (8 - bit_offs)) : 0);

			return result;
		}

		[[nodiscard]] constexpr my_type operator>>(const my_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			if (rhs.m_data[0] >= BIT_SIZE)
				return 0;

			uint32_t shift = rb::bit::read_le<uint32_t>(rhs.m_data.data());

			return *this >> shift;
		}

		[[nodiscard]] constexpr my_type operator~() const noexcept
		{
			my_type result;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] = ~m_data[i];

			return result;
		}

		constexpr my_type& operator&=(const my_type& rhs) noexcept
		{
			*this = *this & rhs;
			return *this;
		}

		constexpr my_type& operator|=(const my_type& rhs) noexcept
		{
			*this = *this | rhs;
			return *this;
		}

		constexpr my_type& operator^=(const my_type& rhs) noexcept
		{
			*this = *this ^ rhs;
			return *this;
		}

		constexpr my_type& operator>>=(uint32_t rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		constexpr my_type& operator>>=(const my_type& rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		constexpr my_type& operator<<=(uint32_t rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

		constexpr my_type& operator<<=(const my_type& rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

	public:
		[[nodiscard]] constexpr bool operator<(const my_type& rhs) const noexcept
		{
			if (sign() < rhs.sign())
				return true;

			if (sign() > rhs.sign())
				return false;

			if (sign() < 0 && rhs.sign() < 0)
				return -rhs < -(*this);

			for (int i = BYTE_SIZE - 1; i >= 0; i--)
				if (m_data[i] != rhs.m_data[i])
					return m_data[i] < rhs.m_data[i];

			return false;
		}

		[[nodiscard]] constexpr bool operator<=(const my_type& rhs) const noexcept
		{
			return !(rhs < *this);
		}

		[[nodiscard]] constexpr bool operator>(const my_type& rhs) const noexcept
		{
			return rhs < *this;
		}

		[[nodiscard]] constexpr bool operator>=(const my_type& rhs) const noexcept
		{
			return !(*this < rhs);
		}

		[[nodiscard]] constexpr bool operator==(const my_type& rhs) const noexcept
		{
			for (size_t i = 0; i < BYTE_SIZE; i++)
				if (m_data[i] != rhs.m_data[i])
					return false;

			return true;
		}

		[[nodiscard]] constexpr bool operator!=(const my_type& rhs) const noexcept
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

			while (it != v.m_data.rend() && *it == 0)
				it++;

			if (it == v.m_data.rend())
				os << "0";
			else
				os << std::hex << static_cast<uint32_t>(*(it++));

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
				v = my_type(hex_str);

			return is;
		}

	public:
		[[nodiscard]] constexpr explicit operator bool() const noexcept
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
}
