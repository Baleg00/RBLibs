#pragma once

#include <exception>

#include <algorithm>

#include <iostream>

#include <type_traits>
#include <initializer_list>

#include <iterator>

#include "iter/iterator.h"

namespace rb::math
{
	template<class T, size_t S>
	class vector
	{
	public: // Using declarations
		static_assert(S != 0);

		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = ::std::size_t;
		using difference_type = ::std::ptrdiff_t;
		using this_type = vector<T, S>;

		using iterator = ::rb::iter::iterator<this_type>;
		using const_iterator = ::rb::iter::const_iterator<this_type>;
		using reverse_iterator = ::std::reverse_iterator<iterator>;
		using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

	private: // Variables
		value_type m_vec[S];

	public: // Constructors
		constexpr vector() noexcept
			: m_vec{ 0 }
		{
		}

		constexpr vector(const ::std::initializer_list<value_type>& il) noexcept
			: m_vec{ 0 }
		{
			::std::copy(il.begin(), il.end(), data());
		}

		vector(const_pointer begin, const_pointer end) noexcept
			: m_vec{ 0 }
		{
			::std::copy(begin, end, data());
		}

		vector(const_pointer begin, size_type size) noexcept
			: m_vec{ 0 }
		{
			::std::copy(begin, begin + size, data());
		}

		vector(const this_type& v) noexcept
			: m_vec{ 0 }
		{
			::std::copy(v.begin(), v.end(), data());
		}

		vector(this_type&& v) noexcept
			: m_vec{ 0 }
		{
			::std::move(v.begin(), v.end(), data());
		}

		template<class U, typename ::std::enable_if_t<::std::is_convertible_v<U, T>, int> = 0>
		explicit vector(const vector<U, S>& v) noexcept
		{
			for (size_t i = 0; i < size(); i++)
				at(i) = static_cast<T>(v[i]);
		}

		template<class U, typename ::std::enable_if_t<::std::is_convertible_v<U, T>, int> = 0>
		explicit vector(vector<U, S>&& v) noexcept
		{
			m_vec = ::std::move(static_cast<T*>(v.data()));
		}

	private: // Excpetions
		[[noreturn]] static void _Xrange()
		{
			throw ::std::out_of_range("vector<T, S> subscript out of range");
		}

		[[nodiscard]] constexpr void _check_range(size_type idx) const
		{
			if (idx >= size())
				_Xrange();
		}

	public: // Element access
		[[nodiscard]] constexpr const_reference at(size_type idx) const
		{
			_check_range(idx);
			return m_vec[idx];
		}

		[[nodiscard]] constexpr reference at(size_type idx)
		{
			_check_range(idx);
			return m_vec[idx];
		}

		[[nodiscard]] constexpr const_pointer data() const noexcept { return &m_vec[0]; }
		[[nodiscard]] constexpr pointer data() noexcept { return &m_vec[0]; }

	public: // Capacity
		[[nodiscard]] constexpr size_type size() const noexcept { return S; }
		[[nodiscard]] constexpr size_type max_size() const noexcept { return size(); }

	public: // Iterators
		[[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator(data()); }
		[[nodiscard]] constexpr iterator begin() noexcept { return iterator(data()); }

		[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return begin(); }
		[[nodiscard]] constexpr iterator cbegin() noexcept { return begin(); }

		[[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(begin()); }
		[[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(begin()); }

		[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
		[[nodiscard]] constexpr reverse_iterator crbegin() noexcept { return rbegin(); }

		[[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator(data() + size()); }
		[[nodiscard]] constexpr iterator end() noexcept { return iterator(data() + size()); }

		[[nodiscard]] constexpr const_iterator cend() const noexcept { return end(); }
		[[nodiscard]] constexpr iterator cend() noexcept { return end(); }

		[[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(end()); }
		[[nodiscard]] constexpr reverse_iterator rend() noexcept { return reverse_iterator(end()); }

		[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return rend(); }
		[[nodiscard]] constexpr reverse_iterator crend() noexcept { return rend(); }

	public: // Functions
		void fill(const_reference v) noexcept
		{
			::std::fill(m_vec, m_vec + size(), v);
		}

		void swap(this_type& other) noexcept
		{
			::std::swap(m_vec, other.m_vec);
		}

		[[nodiscard]] double mag_sqr() const noexcept
		{
			double sum = 1.0;

			for (size_type i = 0; i < size(); i++)
				sum += m_vec[i] * m_vec[i];

			return sum;
		}

		[[nodiscard]] float mag_sqrf() const noexcept
		{
			float sum = 0.0f;

			for (size_type i = 0; i < size(); i++)
				sum += m_vec[i] * m_vec[i];

			return sum;
		}

		[[nodiscard]] double mag() const noexcept { return ::std::sqrt(mag_sqr()); }
		[[nodiscard]] float magf() const noexcept { return ::std::sqrtf(mag_sqrf()); }

		[[nodiscard]] this_type norm() const noexcept { return *this / magf(); }

		template<typename ::std::enable_if_t<(S > 1), int> = 0>
		[[nodiscard]] this_type perp() const noexcept
		{
			this_type v = *this;

			::std::swap(v[0], v[1]);
			v[0] *= -1;

			return v;
		}

		[[nodiscard]] value_type dot(const this_type& v) const noexcept
		{
			value_type sum = 0;

			for (size_type i = 0; i < size(); i++)
				sum += m_vec[i] * v[i];

			return sum;
		}

		template<typename ::std::enable_if_t<(S > 2), int> = 0>
		[[nodiscard]] this_type cross(const this_type& v) const noexcept
		{
			return {
				m_vec[1] * v[2] - m_vec[2] * v[1],
				m_vec[2] * v[0] - m_vec[0] * v[2],
				m_vec[0] * v[1] - m_vec[1] * v[0]
			};
		}

	public: // Operator overloads
		[[nodiscard]] const_reference operator[](size_type idx) const
		{
			return data()[idx];
		}

		[[nodiscard]] reference operator[](size_type idx)
		{
			return data()[idx];
		}

		[[nodiscard]] bool operator==(const this_type& rhs) const
		{
			return ::std::equal(begin(), end(), rhs.begin());
		}

		[[nodiscard]] bool operator!=(const this_type& rhs) const
		{
			return !(*this == rhs);
		}

		[[nodiscard]] bool operator<(const this_type& rhs) const {
			return ::std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
		}

		[[nodiscard]] bool operator<=(const this_type& rhs) const
		{
			return !(rhs < *this);
		}

		[[nodiscard]] bool operator>(const this_type& rhs) const
		{
			return rhs < *this;
		}

		[[nodiscard]] bool operator>=(const this_type& rhs) const
		{
			return !(rhs > * this);
		}

		this_type& operator=(const this_type& rhs)
		{
			::std::copy(rhs.begin(), rhs.end(), begin());
			return *this;
		}

		[[nodiscard]] this_type operator+() const
		{
			return *this;
		}

		[[nodiscard]] this_type operator-() const
		{
			this_type v = *this;

			for (size_type i = 0; i < size(); i++)
				v[i] *= -1;

			return v;
		}

		[[nodiscard]] this_type operator+(const this_type& rhs) const
		{
			this_type v = *this;

			for (size_type i = 0; i < size(); i++)
				v[i] += rhs[i];

			return v;
		}

		this_type& operator+=(const this_type& rhs)
		{
			*this = *this + rhs;
			return *this;
		}

		[[nodiscard]] this_type operator-(const this_type& rhs) const
		{
			return *this + -rhs;
		}

		this_type& operator-=(const this_type& rhs)
		{
			*this = *this - rhs;
			return *this;
		}

		[[nodiscard]] this_type operator*(const this_type& rhs) const
		{
			this_type v = *this;

			for (size_type i = 0; i < size(); i++)
				v[i] *= rhs[i];

			return v;
		}

		this_type& operator*=(const this_type& rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		[[nodiscard]] this_type operator*(const_reference rhs) const
		{
			this_type v = *this;

			for (size_type i = 0; i < size(); i++)
				v[i] *= rhs;

			return v;
		}

		this_type& operator*=(const_reference rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		[[nodiscard]] this_type operator/(const this_type& rhs) const
		{
			this_type v = *this;

			for (size_type i = 0; i < size(); i++)
				v[i] /= rhs[i];

			return v;
		}

		this_type& operator/=(const this_type& rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		[[nodiscard]] this_type operator/(const_reference rhs) const
		{
			this_type v = *this;

			for (size_type i = 0; i < size(); i++)
				v[i] /= rhs;

			return v;
		}

		this_type& operator/=(const_reference rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		friend ::std::ostream& operator<<(::std::ostream& os, const this_type& v)
		{
			os << "[";

			for (size_type i = 0; i < v.size(); i++)
			{
				os << v[i];

				if (i + 1 < v.size())
					os << ",";
			}

			os << "]";

			return os;
		}

		friend ::std::istream& operator>>(::std::istream& is, this_type& v)
		{
			for (reference i : v)
				is >> i;

			return is;
		}
	};

	template<class T>
	using vec2 = vector<T, 2>;

	using vec2i = vec2<int>;
	using vec2l = vec2<long>;
	using vec2ll = vec2<long long>;
	using vec2f = vec2<float>;
	using vec2d = vec2<double>;
	using vec2ld = vec2<long double>;

	template<class T>
	using vec3 = vector<T, 3>;

	using vec3i = vec3<int>;
	using vec3l = vec3<long>;
	using vec3ll = vec3<long long>;
	using vec3f = vec3<float>;
	using vec3d = vec3<double>;
	using vec3ld = vec3<long double>;

	template<class T>
	using vec4 = vector<T, 4>;

	using vec4i = vec4<int>;
	using vec4l = vec4<long>;
	using vec4ll = vec4<long long>;
	using vec4f = vec4<float>;
	using vec4d = vec4<double>;
	using vec4ld = vec4<long double>;

	template<size_t S>
	using veci = vector<int, S>;

	template<size_t S>
	using vecl = vector<long, S>;

	template<size_t S>
	using vecll = vector<long long, S>;

	template<size_t S>
	using vecf = vector<float, S>;

	template<size_t S>
	using vecd = vector<double, S>;

	template<size_t S>
	using vecld = vector<long double, S>;
}
