#pragma once

#include <array>
#include <vector>

#include <string_view>

#include <cstdint>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <utility>
#include <algorithm>
#include <functional>
#include <exception>

#include "bit/endian.h"

namespace rb::math
{
	template<size_t B>
	class bigint_impl
	{
	public:
		static inline constexpr size_t BIT_SIZE = B;
		static inline constexpr size_t BYTE_SIZE = (B + 7) / 8;

	private:
		using this_type = bigint_impl<BIT_SIZE>;
		using data_type = std::array<uint8_t, BYTE_SIZE>;

	public:
		constexpr bigint_impl() noexcept
			: m_data{ 0 }
		{
		}

		constexpr bigint_impl(const this_type& other) noexcept
			: m_data(other.m_data)
		{
		}

		constexpr bigint_impl(this_type&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

		explicit constexpr bigint_impl(const std::string_view& str)
			: bigint_impl()
		{
			for (const char& c : str)
				if ((c < '0' || c > '9') && (c < 'a' || c > 'f') && (c < 'A' || c > 'F'))
					throw std::invalid_argument("`" __FUNCSIG__ "`: argument contains non-hexadecimal characters");

			for (size_t i = 0; i < (str.size() >> 1); i++)
				m_data[i] = _hex_char_to_int(str[str.size() - 1 - (i << 1)]) | (_hex_char_to_int(str[str.size() - 2 - (i << 1)]) << 4);

			if (str.size() & 1)
				m_data[str.size() >> 1] = _hex_char_to_int(str[0]);
		}

		explicit constexpr bigint_impl(const uint8_t* data, size_t size) noexcept
			: bigint_impl()
		{
			for (size_t i = 0; i < std::min(size, BYTE_SIZE); i++)
				m_data[i] = data[i];
		}

		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		constexpr bigint_impl(T value) noexcept
			: bigint_impl()
		{
			using U = std::make_unsigned_t<T>;
			U u_value = static_cast<U>(value);

			if (value < 0)
				*this = ~(*this);

			for (size_t i = 0; i < std::min(BYTE_SIZE, sizeof(T)); i++)
				m_data[i] = static_cast<uint8_t>((u_value >> (i * 8)) & 0xFF);
		}

	private:
		[[nodiscard]] static constexpr uint8_t _hex_char_to_int(char c)
		{
			if (c >= '0' && c <= '9')
				return c - '0';

			if (c >= 'a' && c <= 'f')
				return 10 + c - 'a';

			if (c >= 'A' && c <= 'F')
				return 10 + c - 'A';

			return -1;
		}

		[[nodiscard]] constexpr std::pair<this_type, this_type> _division_impl(const this_type& rhs) const
		{
			if (rhs.is_zero())
				throw std::domain_error("`" __FUNCSIG__ "`: cannot divide by 0");

			if (is_zero())
				return { 0, 0 };

			this_type n = *this, d = rhs;

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

			this_type q, r;

			for (int32_t i = n.bits() - 1; i >= 0; i--)
			{
				r <<= 1;
				r.m_data[0] = (r.m_data[0] & 0xFE) | ((n.m_data[i / 8] >> (i % 8)) & 1);

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
		[[nodiscard]] static constexpr this_type ZERO() noexcept
		{
			return 0;
		}

		[[nodiscard]] static constexpr this_type ONE() noexcept
		{
			return 1;
		}

		[[nodiscard]] static constexpr this_type MIN() noexcept
		{
			return ONE() << (B - 1);
		}

		[[nodiscard]] static constexpr this_type MAX() noexcept
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

		[[nodiscard]] constexpr this_type abs() const noexcept
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

				while (byte)
				{
					byte >>= 1;
					num_bits++;
				}
			}

			return num_bits;
		}

		[[nodiscard]] constexpr this_type sqrt() const
		{
			if (sign() < 0)
				throw std::domain_error("`" __FUNCSIG__ "`: cannot take square root of a negative number");

			if (*this < 2)
				return *this;

			this_type small_candidate = (*this >> 2).sqrt() << 1;
			this_type large_candidate = small_candidate + 1;

			if (large_candidate * large_candidate > * this)
				return small_candidate;

			return large_candidate;
		}

		[[nodiscard]] constexpr this_type pow(this_type n) const
		{
			if (n < 0)
				throw std::domain_error("`" __FUNCSIG__ "`: exponent `n` cannot be negative");

			if (n.is_zero())
				return 1;

			this_type x = *this, y = 1;

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

		[[nodiscard]] constexpr this_type log(const this_type& b) const
		{
			if (b <= 0)
				throw std::domain_error("`" __FUNCSIG__ "`: logarithm base `b` must be positive");

			if (*this <= 0)
				throw std::domain_error("`" __FUNCSIG__ "`: cannot take logarithm of non-positive number");

			this_type result;
			this_type x = *this;

			while (x >= b)
			{
				result++;
				x /= b;
			}

			return result;
		}

		[[nodiscard]] constexpr this_type log2() const
		{
			if (*this <= 0)
				throw std::domain_error("`" __FUNCSIG__ "`: cannot take logarithm of non-positive number");

			return bits() - 1;
		}

		// Binary GCD Algorithm
		// https://en.wikipedia.org/wiki/Binary_GCD_algorithm
		[[nodiscard]] constexpr this_type gcd(this_type a) const noexcept
		{
			this_type b = (*this).abs();
			a = a.abs();
			uint32_t d = 0;

			while (a != b)
			{
				if ((a.m_data[0] & 1) == 0 && (b.m_data[0] & 1) == 0)
				{
					a >>= 1;
					b >>= 1;
					d++;
				}
				else if ((a.m_data[0] & 1) == 0)
					a >>= 1;
				else if ((b.m_data[0] & 1) == 0)
					b >>= 1;
				else if (a > b)
					a -= b;
				else
					b -= a;
			}

			return a << d;
		}

		[[nodiscard]] constexpr this_type lcm(this_type a) const noexcept
		{
			return abs() / gcd(a) * a.abs();
		}

	public:
		[[nodiscard]] constexpr size_t size() const noexcept
		{
			return m_data.size();
		}

		[[nodiscard]] constexpr uint8_t* data() noexcept
		{
			return m_data.data();
		}

		[[nodiscard]] constexpr const uint8_t* data() const noexcept
		{
			return m_data.data();
		}

	public:
		constexpr this_type& operator=(const this_type& rhs) noexcept
		{
			m_data = rhs.m_data;
			return *this;
		}

		constexpr this_type& operator=(this_type&& rhs) noexcept
		{
			m_data = std::move(rhs.m_data);
			return *this;
		}

		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		constexpr this_type& operator=(T rhs) noexcept
		{
			*this = this_type(rhs);
			return *this;
		}

	public:
		[[nodiscard]] constexpr this_type operator+(const this_type& rhs) const noexcept
		{
			this_type result;

			for (size_t i = 0, carry = 0; i < BYTE_SIZE; i++)
			{
				uint32_t temp = carry + m_data[i] + rhs.m_data[i];
				result.m_data[i] = temp & 0xFF;
				carry = temp >> 8;
			}

			return result;
		}

		[[nodiscard]] constexpr this_type operator-(const this_type& rhs) const noexcept
		{
			return *this + (-rhs);
		}

		[[nodiscard]] constexpr this_type operator+() const noexcept
		{
			return *this;
		}

		[[nodiscard]] constexpr this_type operator-() const noexcept
		{
			return ~(*this) + 1;
		}

		constexpr this_type operator++(int) noexcept
		{
			this_type temp = *this;
			*this += 1;
			return temp;
		}

		constexpr this_type operator--(int) noexcept
		{
			this_type temp = *this;
			*this -= 1;
			return temp;
		}

		constexpr this_type& operator++() noexcept
		{
			*this += 1;
			return *this;
		}

		constexpr this_type& operator--() noexcept
		{
			*this -= 1;
			return *this;
		}

		[[nodiscard]] constexpr this_type operator*(const this_type& rhs) const noexcept
		{
			if (is_zero() || rhs.is_zero())
				return 0;

			this_type a = *this, b = rhs;

			bool change_sign = false;

			if (a.sign() * b.sign() < 0)
				change_sign = true;

			if (a.sign() < 0)
				a = -a;

			if (b.sign() < 0)
				b = -b;

			this_type result;

			for (size_t i = 0; i < BYTE_SIZE; i++)
			{
				for (size_t j = 0, carry = 0; j < BYTE_SIZE - i; j++)
				{
					uint32_t temp = carry + a.m_data[i] * b.m_data[j] + result.m_data[i + j];
					result.m_data[i + j] = temp & 0xFF;
					carry = temp >> 8;
				}
			}

			if (change_sign)
				return -result;

			return result;
		}

		[[nodiscard]] constexpr this_type operator/(const this_type& rhs) const
		{
			return _division_impl(rhs).first;
		}

		[[nodiscard]] constexpr this_type operator%(const this_type& rhs) const
		{
			return _division_impl(rhs).second;
		}

		constexpr this_type& operator+=(const this_type& rhs) noexcept
		{
			*this = *this + rhs;
			return *this;
		}

		constexpr this_type& operator-=(const this_type& rhs) noexcept
		{
			*this = *this - rhs;
			return *this;
		}

		constexpr this_type& operator*=(const this_type& rhs) noexcept
		{
			*this = *this * rhs;
			return *this;
		}

		constexpr this_type& operator/=(const this_type& rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		constexpr this_type& operator%=(const this_type& rhs)
		{
			*this = *this % rhs;
			return *this;
		}

	public:
		[[nodiscard]] constexpr this_type operator&(const this_type& rhs) const noexcept
		{
			this_type result = *this;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] &= rhs.m_data[i];

			return result;
		}

		[[nodiscard]] constexpr this_type operator|(const this_type& rhs) const noexcept
		{
			this_type result = *this;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] |= rhs.m_data[i];

			return result;
		}

		[[nodiscard]] constexpr this_type operator^(const this_type& rhs) const noexcept
		{
			this_type result = *this;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] ^= rhs.m_data[i];

			return result;
		}

		[[nodiscard]] constexpr this_type operator<<(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (rhs >= BIT_SIZE)
				return 0;

			const uint32_t byte_pos = rhs / 8;
			const uint32_t bit_offs = rhs % 8;

			this_type result;

			for (size_t i = byte_pos; i < BYTE_SIZE; i++)
				result.m_data[i] |= (m_data[i - byte_pos] << bit_offs) | (i - byte_pos > 0 ? (m_data[i - byte_pos - 1] >> (8 - bit_offs)) : 0);

			return result;
		}

		[[nodiscard]] constexpr this_type operator<<(const this_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			if (rhs.m_data[0] >= BIT_SIZE)
				return 0;

			uint32_t shift = rb::bit::read_le<uint32_t>(rhs.m_data.data());

			return *this << shift;
		}

		[[nodiscard]] constexpr this_type operator>>(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (rhs >= BIT_SIZE)
				return 0;

			const uint32_t byte_pos = rhs >> 3;
			const uint32_t bit_offs = rhs & 0b111;

			this_type result;

			for (size_t i = 0; i < BYTE_SIZE - byte_pos; i++)
				result.m_data[i] |= (m_data[i + byte_pos] >> bit_offs) | (byte_pos + i + 1 < BYTE_SIZE ? (m_data[byte_pos + i + 1] << (8 - bit_offs)) : 0);

			return result;
		}

		[[nodiscard]] constexpr this_type operator>>(const this_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			if (rhs.m_data[0] >= BIT_SIZE)
				return 0;

			uint32_t shift = rb::bit::read_le<uint32_t>(rhs.m_data.data());

			return *this >> shift;
		}

		[[nodiscard]] constexpr this_type operator~() const noexcept
		{
			this_type result;

			for (size_t i = 0; i < BYTE_SIZE; i++)
				result.m_data[i] = ~m_data[i];

			return result;
		}

		constexpr this_type& operator&=(const this_type& rhs) noexcept
		{
			*this = *this & rhs;
			return *this;
		}

		constexpr this_type& operator|=(const this_type& rhs) noexcept
		{
			*this = *this | rhs;
			return *this;
		}

		constexpr this_type& operator^=(const this_type& rhs) noexcept
		{
			*this = *this ^ rhs;
			return *this;
		}

		constexpr this_type& operator>>=(uint32_t rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		constexpr this_type& operator>>=(const this_type& rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		constexpr this_type& operator<<=(uint32_t rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

		constexpr this_type& operator<<=(const this_type& rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

	public:
		[[nodiscard]] constexpr bool operator<(const this_type& rhs) const noexcept
		{
			if (sign() < rhs.sign())
				return true;

			if (sign() > rhs.sign())
				return false;

			if (sign() < 0 && rhs.sign() < 0)
				return -rhs < -(*this);

			for (int32_t i = BYTE_SIZE - 1; i >= 0; i--)
				if (m_data[i] != rhs.m_data[i])
					return m_data[i] < rhs.m_data[i];

			return false;
		}

		[[nodiscard]] constexpr bool operator<=(const this_type& rhs) const noexcept
		{
			return !(rhs < *this);
		}

		[[nodiscard]] constexpr bool operator>(const this_type& rhs) const noexcept
		{
			return rhs < *this;
		}

		[[nodiscard]] constexpr bool operator>=(const this_type& rhs) const noexcept
		{
			return !(*this < rhs);
		}

		[[nodiscard]] constexpr bool operator==(const this_type& rhs) const noexcept
		{
			for (size_t i = 0; i < BYTE_SIZE; i++)
				if (m_data[i] != rhs.m_data[i])
					return false;

			return true;
		}

		[[nodiscard]] constexpr bool operator!=(const this_type& rhs) const noexcept
		{
			return !(*this == rhs);
		}

	public:
		friend std::ostream& operator<<(std::ostream& os, this_type v)
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
			{
				os << std::hex << static_cast<uint32_t>(*(it++));

				while (it != v.m_data.rend())
					os << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(*(it++));
			}

			return os;
		}

		friend std::istream& operator>>(std::istream& is, this_type& v)
		{
			std::string hex_str;
			is >> hex_str;

			if (hex_str[0] == '-')
			{
				hex_str.erase(hex_str.begin());
				v = -this_type(hex_str);
			}
			else
				v = this_type(hex_str);

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

	template<>
	class bigint_impl<0>
	{
	private:
		using this_type = bigint_impl<0>;
		using data_type = std::vector<uint8_t>;

	public:
		bigint_impl() noexcept
			: m_sign(1), m_data({ 0 })
		{
		}

		bigint_impl(const this_type& other) noexcept
			: m_sign(other.m_sign), m_data(other.m_data)
		{
		}

		bigint_impl(this_type&& other) noexcept
			: m_sign(std::move(other.m_sign)), m_data(std::move(other.m_data))
		{
		}

		explicit bigint_impl(const std::string_view& str)
			: bigint_impl()
		{
			for (const char& c : str)
				if ((c < '0' || c > '9') && (c < 'a' || c > 'f') && (c < 'A' || c > 'F'))
					throw std::invalid_argument("`" __FUNCSIG__ "`: argument contains non-hexadecimal characters");

			m_data.resize((str.size() + 1) >> 1);

			for (size_t i = 0; i < (str.size() >> 1); i++)
				m_data[i] = _hex_char_to_int(str[str.size() - 1 - (i << 1)]) | (_hex_char_to_int(str[str.size() - 2 - (i << 1)]) << 4);

			if (str.size() & 1)
				m_data[str.size() >> 1] = _hex_char_to_int(str[0]);

			fit();
		}

		explicit bigint_impl(const uint8_t* data, size_t size) noexcept
			: bigint_impl()
		{
			m_data.resize(size);

			for (size_t i = 0; i < size; i++)
				m_data[i] = data[i];

			fit();
		}

		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		bigint_impl(T value) noexcept
			: bigint_impl()
		{
			if constexpr (std::is_signed_v<T>)
			{
				if (value < 0)
				{
					m_sign = -1;
					value = -value;
				}
			}

			using U = std::make_unsigned_t<T>;
			U u_value = static_cast<U>(value);

			m_data.resize(sizeof(T));

			for (size_t i = 0; i < sizeof(T); i++)
				m_data[i] = static_cast<uint8_t>((u_value >> (i * 8)) & 0xFF);

			fit();
		}

	private:
		[[nodiscard]] static constexpr uint8_t _hex_char_to_int(char c)
		{
			if (c >= '0' && c <= '9')
				return c - '0';

			if (c >= 'a' && c <= 'f')
				return 10 + c - 'a';

			if (c >= 'A' && c <= 'F')
				return 10 + c - 'A';

			return -1;
		}

		void resize(size_t size)
		{
			m_data.resize(size);
		}

		void fit()
		{
			size_t n = 0;
			for (data_type::reverse_iterator it = m_data.rbegin(); it != m_data.rend() && *it == 0; it++)
				n++;
			resize(std::max(size() - n, 1U));
		}

		// Binary Long Division
		// https://en.wikipedia.org/wiki/Division_algorithm#Long_division
		[[nodiscard]] std::pair<this_type, this_type> _division_impl(const this_type& rhs) const
		{
			if (rhs.is_zero())
				throw std::domain_error("`" __FUNCSIG__ "`: cannot divide by 0");

			if (is_zero())
				return { 0, 0 };

			this_type n = *this, d = rhs;

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

			this_type q, r;

			size_t max_size = std::max(n.size(), d.size());
			n.resize(max_size);
			d.resize(max_size);
			q.resize(max_size);

			for (int32_t i = n.bits() - 1; i >= 0; i--)
			{
				r <<= 1;
				r.m_data[0] = (r.m_data[0] & 0xFE) | ((n.m_data[i >> 3] >> (i & 0b111)) & 1);

				if (r >= d)
				{
					r -= d;
					q.m_data[i >> 3] |= 1 << (i % 8);
				}
			}

			q.fit();
			r.fit();

			if (n_changed && d_changed)
				return { q, -r };
			else if (n_changed && !d_changed)
				return { -q, rhs - r };
			else if (!n_changed && d_changed)
				return { -q, rhs + r };

			return { q, r };
		}

	public:
		[[nodiscard]] static this_type ZERO() noexcept
		{
			static const this_type zero = 0;
			return zero;
		}

		[[nodiscard]] static this_type ONE() noexcept
		{
			static const this_type one = 1;
			return one;
		}

	public:
		[[nodiscard]] int sign() const noexcept
		{
			return m_sign;
		}

		[[nodiscard]] bool is_zero() const noexcept
		{
			return size() == 1 && m_data[0] == 0;
		}

		[[nodiscard]] this_type abs() const noexcept
		{
			return sign() < 0 ? -(*this) : *this;
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

				while (byte)
				{
					byte >>= 1;
					num_bits++;
				}
			}

			return num_bits;
		}

		[[nodiscard]] this_type sqrt() const
		{
			if (sign() < 0)
				throw std::domain_error("`" __FUNCSIG__ "`: cannot take square root of a negative number");

			if (*this < 2)
				return *this;

			this_type small_candidate = (*this >> 2).sqrt() << 1;
			this_type large_candidate = small_candidate + 1;

			if (large_candidate * large_candidate > *this)
				return small_candidate;

			return large_candidate;
		}

		[[nodiscard]] this_type pow(this_type n) const
		{
			if (n.sign() < 0)
				throw std::domain_error("`" __FUNCSIG__ "`: exponent `n` cannot be negative");

			if (n.is_zero())
				return 1;

			this_type x = *this, y = 1;

			while (n > 1)
			{
				if (n.m_data.front() & 1)
				{
					y = x * y;
					x *= x;
					n = (n - 1) >> 1;
				}
				else
				{
					x *= x;
					n >>= 1;
				}
			}

			return x * y;
		}

		[[nodiscard]] this_type log(const this_type& b) const
		{
			if (b <= 0)
				throw std::domain_error("`" __FUNCSIG__ "`: logarithm base `b` must be positive");

			if (*this <= 0)
				throw std::domain_error("`" __FUNCSIG__ "`: cannot take logarithm of non-positive number");

			this_type result;
			this_type x = *this;

			while (x >= b)
			{
				result++;
				x /= b;
			}

			return result;
		}

		[[nodiscard]] this_type log2() const
		{
			if (*this <= 0)
				throw std::domain_error("`" __FUNCSIG__ "`: cannot take logarithm of non-positive number");

			return bits() - 1;
		}

		// Binary GCD Algorithm
		// https://en.wikipedia.org/wiki/Binary_GCD_algorithm
		[[nodiscard]] this_type gcd(this_type a) const noexcept
		{
			this_type b = *this;
			uint32_t d = 0;

			while (a != b)
			{
				if ((a.m_data[0] & 1) == 0 && (b.m_data[0] & 1) == 0)
				{
					a >>= 1;
					b >>= 1;
					d++;
				}
				else if ((a.m_data[0] & 1) == 0)
					a >>= 1;
				else if ((b.m_data[0] & 1) == 0)
					b >>= 1;
				else if (a > b)
					a -= b;
				else
					b -= a;
			}

			return a << d;
		}

		[[nodiscard]] this_type lcm(this_type a) const noexcept
		{
			return (*this * a).abs() / gcd(a);
		}

	public:
		[[nodiscard]] size_t size() const noexcept
		{
			return m_data.size();
		}

		[[nodiscard]] uint8_t* data() noexcept
		{
			return m_data.data();
		}

		[[nodiscard]] const uint8_t* data() const noexcept
		{
			return m_data.data();
		}

		public:
			this_type& operator=(const this_type& rhs) noexcept
			{
				m_sign = rhs.m_sign;
				m_data = rhs.m_data;
				return *this;
			}

			this_type& operator=(this_type&& rhs) noexcept
			{
				m_sign = std::move(rhs.m_sign);
				m_data = std::move(rhs.m_data);
				return *this;
			}

			template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
			this_type& operator=(T rhs) noexcept
			{
				*this = this_type(rhs);
				return *this;
			}

	public:
		[[nodiscard]] this_type operator+(const this_type& rhs) const noexcept
		{
			if (sign() > 0 && rhs.sign() < 0)
				return *this - -rhs;

			if (sign() < 0 && rhs.sign() > 0)
				return -(-(*this) - rhs);

			const this_type& longer = size() >= rhs.size() ? *this : rhs;
			const this_type& shorter = size() < rhs.size() ? *this : rhs;

			this_type result;
			result.resize(longer.size() + 1);

			uint32_t carry = 0;
			for (size_t i = 0; i < shorter.size(); i++)
			{
				uint32_t temp = carry + longer.m_data[i] + shorter.m_data[i];
				result.m_data[i] = temp & 0xFF;
				carry = temp >> 8;
			}

			for (size_t i = shorter.size(); i < longer.size(); i++)
			{
				uint32_t temp = carry + longer.m_data[i];
				result.m_data[i] = temp & 0xFF;
				carry = temp >> 8;
			}

			if (carry)
				result.m_data.back() = carry;

			result.fit();

			return result;
		}

		[[nodiscard]] this_type operator-(const this_type& rhs) const noexcept
		{
			if (sign() > 0 && rhs.sign() < 0)
				return *this + -rhs;

			if (sign() < 0 && rhs.sign() > 0)
				return -(-(*this) + rhs);

			this_type result, greater, lesser;

			if (*this >= rhs)
			{
				greater = *this;
				lesser = rhs;
			}
			else
			{
				greater = rhs;
				lesser = *this;
				result.m_sign = -1;
			}

			lesser.resize(greater.size());
			result.resize(greater.size() + 1);
			
			int32_t borrow = 0;
			for (size_t i = 0; i < greater.size(); i++)
			{
				int32_t temp = borrow + static_cast<int32_t>(greater.m_data[i]) - static_cast<int32_t>(lesser.m_data[i]);
				
				if (temp < 0)
				{
					result.m_data[i] = (256 + (temp % 256)) & 0xFF;
					borrow = temp >> 8;
				}
				else
				{
					result.m_data[i] = temp;
					borrow = 0;
				}
			}

			result.fit();

			return result;
		}

		[[nodiscard]] this_type operator+() const noexcept
		{
			return *this;
		}

		[[nodiscard]] this_type operator-() const noexcept
		{
			this_type temp = *this;
			temp.m_sign = -temp.m_sign;
			return temp;
		}

		this_type operator++(int) noexcept
		{
			this_type temp = *this;
			*this += 1;
			return temp;
		}

		this_type operator--(int) noexcept
		{
			this_type temp = *this;
			*this -= 1;
			return temp;
		}

		this_type& operator++() noexcept
		{
			*this += 1;
			return *this;
		}

		this_type& operator--() noexcept
		{
			*this -= 1;
			return *this;
		}

		[[nodiscard]] this_type operator*(const this_type& rhs) const noexcept
		{
			if (is_zero() || rhs.is_zero())
				return 0;

			this_type a = *this, b = rhs;

			bool change_sign = false;

			if (a.sign() * b.sign() < 0)
				change_sign = true;

			if (a.sign() < 0)
				a = -a;

			if (b.sign() < 0)
				b = -b;

			size_t result_size = a.size() + b.size();
			
			this_type result;
			result.resize(result_size);
			a.resize(result_size);
			b.resize(result_size);

			for (size_t i = 0; i < result_size; i++)
			{
				for (size_t j = 0, carry = 0; j < result_size - i; j++)
				{
					uint32_t temp = carry + a.m_data[i] * b.m_data[j] + result.m_data[i + j];
					result.m_data[i + j] = temp & 0xFF;
					carry = temp >> 8;
				}
			}

			result.fit();

			if (change_sign)
				return -result;

			return result;
		}

		[[nodiscard]] this_type operator/(const this_type& rhs) const
		{
			return _division_impl(rhs).first;
		}

		[[nodiscard]] this_type operator%(const this_type& rhs) const
		{
			return _division_impl(rhs).second;
		}

		this_type& operator+=(const this_type& rhs) noexcept
		{
			*this = *this + rhs;
			return *this;
		}

		this_type& operator-=(const this_type& rhs) noexcept
		{
			*this = *this - rhs;
			return *this;
		}

		this_type& operator*=(const this_type& rhs) noexcept
		{
			*this = *this * rhs;
			return *this;
		}

		this_type& operator/=(const this_type& rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		this_type& operator%=(const this_type& rhs)
		{
			*this = *this % rhs;
			return *this;
		}

	public:
		[[nodiscard]] this_type operator&(const this_type& rhs) const noexcept
		{
			this_type result = *this;
			result.resize(rhs.size());

			for (size_t i = 0; i < result.size(); i++)
				result.m_data[i] &= rhs.m_data[i];

			result.fit();

			return result;
		}

		[[nodiscard]] this_type operator|(const this_type& rhs) const noexcept
		{
			this_type result = *this;
			
			if (result.size() < rhs.size())
				result.resize(rhs.size());

			for (size_t i = 0; i < rhs.size(); i++)
				result.m_data[i] |= rhs.m_data[i];

			result.fit();

			return result;
		}

		[[nodiscard]] this_type operator^(const this_type& rhs) const noexcept
		{
			this_type result = *this;
			this_type temp = rhs;

			if (result.size() < temp.size())
				result.resize(temp.size());
			else
				temp.resize(result.size());

			for (size_t i = 0; i < result.size(); i++)
				result.m_data[i] ^= temp.m_data[i];

			result.fit();

			return result;
		}

		[[nodiscard]] this_type operator<<(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (is_zero())
				return ZERO();

			const uint32_t byte_pos = rhs / 8;
			const uint32_t bit_offs = rhs % 8;

			this_type result;
			result.resize(size() + (rhs + 7) / 8);

			result.m_data[byte_pos] = m_data[0] << bit_offs;
			
			for (size_t i = byte_pos + 1; i < result.size() - 1; i++)
				result.m_data[i] |= (m_data[i - byte_pos] << bit_offs) | (m_data[i - byte_pos - 1] >> (8 - bit_offs));

			result.m_data.back() = m_data.back() >> ((8 - bit_offs) % 8);

			result.fit();

			return result;
		}

		[[nodiscard]] this_type operator<<(const this_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			this_type temp = rhs;
			
			if (rhs.size() < 4)
				temp.resize(4);

			uint32_t shift = rb::bit::read_le<uint32_t>(temp.m_data.data());

			return *this << shift;
		}

		[[nodiscard]] this_type operator>>(uint32_t rhs) const noexcept
		{
			if (rhs == 0)
				return *this;

			if (rhs >= size() * 8)
				return 0;

			const uint32_t byte_pos = rhs / 8;
			const uint32_t bit_offs = rhs % 8;

			this_type result;
			result.resize(size() - byte_pos);

			for (size_t i = 0; i < result.size(); i++)
				result.m_data[i] |= (m_data[i + byte_pos] >> bit_offs) | (byte_pos + i + 1 < size() ? (m_data[byte_pos + i + 1] << (8 - bit_offs)) : 0);

			result.fit();

			return result;
		}

		[[nodiscard]] this_type operator>>(const this_type& rhs) const noexcept
		{
			if (rhs.is_zero())
				return *this;

			this_type temp = rhs;

			if (rhs.size() < 4)
				temp.resize(4);

			uint32_t shift = rb::bit::read_le<uint32_t>(temp.m_data.data());

			return *this >> shift;
		}

		[[nodiscard]] this_type operator~() const noexcept
		{
			this_type result = *this;

			for (size_t i = 0; i < result.size(); i++)
				result.m_data[i] = ~result.m_data[i];

			result.fit();

			return result;
		}

		this_type& operator&=(const this_type& rhs) noexcept
		{
			if (size() > rhs.size())
				resize(rhs.size());

			for (size_t i = 0; i < size(); i++)
				m_data[i] &= rhs.m_data[i];

			fit();

			return *this;
		}

		this_type& operator|=(const this_type& rhs) noexcept
		{
			if (size() < rhs.size())
				resize(rhs.size());

			for (size_t i = 0; i < rhs.size(); i++)
				m_data[i] |= rhs.m_data[i];

			fit();

			return *this;
		}

		this_type& operator^=(const this_type& rhs) noexcept
		{
			this_type temp = rhs;

			if (size() < temp.size())
				resize(temp.size());
			else
				temp.resize(size());

			for (size_t i = 0; i < size(); i++)
				m_data[i] ^= temp.m_data[i];

			fit();

			return *this;
		}

		this_type& operator>>=(uint32_t rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		this_type& operator>>=(const this_type& rhs) noexcept
		{
			*this = *this >> rhs;
			return *this;
		}

		this_type& operator<<=(uint32_t rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

		this_type& operator<<=(const this_type& rhs) noexcept
		{
			*this = *this << rhs;
			return *this;
		}

	public:
		[[nodiscard]] bool operator<(const this_type& rhs) const noexcept
		{
			if (sign() < rhs.sign())
				return true;

			if (sign() > rhs.sign())
				return false;

			if (sign() < 0)
				return -(*this) > -rhs;

			if (size() > rhs.size())
				for (int32_t i = static_cast<int32_t>(size()) - 1; i >= static_cast<int32_t>(rhs.size()); i--)
					if (m_data[i])
						return false;

			if (size() < rhs.size())
				for (int32_t i = static_cast<int32_t>(rhs.size()) - 1; i >= static_cast<int32_t>(size()); i--)
					if (rhs.m_data[i])
						return true;

			for (int32_t i = static_cast<int32_t>(std::min(size(), rhs.size())) - 1; i >= 0; i--)
				if (m_data[i] != rhs.m_data[i])
					return m_data[i] < rhs.m_data[i];

			return false;
		}

		[[nodiscard]] bool operator<=(const this_type& rhs) const noexcept
		{
			return !(rhs < *this);
		}

		[[nodiscard]] bool operator>(const this_type& rhs) const noexcept
		{
			return rhs < *this;
		}

		[[nodiscard]] bool operator>=(const this_type& rhs) const noexcept
		{
			return !(*this < rhs);
		}

		[[nodiscard]] bool operator==(const this_type& rhs) const noexcept
		{
			if (sign() != rhs.sign())
				return false;

			if (size() != rhs.size())
				return false;

			for (size_t i = 0; i < size(); i++)
				if (m_data[i] != rhs.m_data[i])
					return false;

			return true;
		}

		[[nodiscard]] bool operator!=(const this_type& rhs) const noexcept
		{
			return !(*this == rhs);
		}

	public:
		friend std::ostream& operator<<(std::ostream& os, const this_type& v)
		{
			if (v.sign() < 0)
				os << "-";

			data_type::const_reverse_iterator it = v.m_data.rbegin();

			if (it == v.m_data.rend())
				os << "0";
			else
			{
				os << std::hex << static_cast<uint32_t>(*(it++));

				while (it != v.m_data.rend())
					os << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(*(it++));
			}

			return os;
		}

		friend std::istream& operator>>(std::istream& is, this_type& v)
		{
			std::string hex_str;
			is >> hex_str;

			if (hex_str[0] == '-')
			{
				hex_str.erase(hex_str.begin());
				v = -this_type(hex_str);
			}
			else
				v = this_type(hex_str);

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
		int m_sign;
		data_type m_data;
	};
}
