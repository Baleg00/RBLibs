#pragma once

#include <exception>

#include <algorithm>

#include <random>

#include <iostream>

#include <type_traits>
#include <initializer_list>

#include <iterator>

#include "iter/iterator.h"

namespace rb::math
{
	template<typename T, size_t R, size_t C>
	class matrix
	{
	public: // Using declarations
		static_assert(R > 1 && C > 1, "matrix must have at least two rows and columns");
		static_assert(std::is_arithmetic_v<T>, "type must be arithmetic");

		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using this_type = matrix<T, R, C>;

		using iterator = ::rb::iter::iterator<this_type>;
		using const_iterator = ::rb::iter::const_iterator<this_type>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public: // Constructors
		constexpr matrix() noexcept
			: m_mat{ 0 }
		{
		}

		matrix(const std::initializer_list<value_type>& il) noexcept
			: m_mat{ 0 }
		{
			std::copy(il.begin(), il.end(), data());
		}

		explicit matrix(const value_type m[R][C]) noexcept
			: m_mat{ 0 }
		{
			std::copy(&m[0][0], &m[0][0] + (R * C), data());
		}

		matrix(const_pointer begin, const_pointer end) noexcept
			: m_mat{ 0 }
		{
			std::copy(begin, end, data());
		}

		matrix(const_pointer begin, size_type size) noexcept
			: m_mat{ 0 }
		{
			std::copy(begin, begin + size, data());
		}

		matrix(const this_type& m) noexcept
			: m_mat{ 0 }
		{
			std::copy(m.begin(), m.end(), data());
		}

		matrix(this_type&& m) noexcept
			: m_mat{ 0 }
		{
			std::move(m.begin(), m.end(), data());
		}

		template<class U, typename std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
		explicit matrix(const matrix<U, R, C>& m) noexcept
		{
			for (size_t i = 0; i < size(); i++)
				at(i) = static_cast<T>(m.at(i));
		}

		template<class U, typename std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
		explicit matrix(matrix<U, R, C>&& m) noexcept
		{
			m_mat = std::move(static_cast<T*>(m.data()));
		}

	private: // Exceptions
		[[noreturn]] static void _Xrange()
		{
			throw std::out_of_range("matrix subscript out of range");
		}

		constexpr void _check_range(size_type r, size_type c) const
		{
			if (r >= rows() || c >= columns())
				_Xrange();
		}

		constexpr void _check_range(size_type idx) const
		{
			if (idx >= size())
				_Xrange();
		}

	public: // Element access
		[[nodiscard]] constexpr const_reference at(size_type idx) const
		{
			_check_range(idx);
			return data()[idx];
		}

		[[nodiscard]] constexpr reference at(size_type idx)
		{
			_check_range(idx);
			return data()[idx];
		}
		
		[[nodiscard]] constexpr const_reference at(size_type r, size_type c) const
		{
			_check_range(r, c);
			return m_mat[r][c];
		}

		[[nodiscard]] constexpr reference at(size_type r, size_type c)
		{
			_check_range(r, c);
			return m_mat[r][c];
		}

		[[nodiscard]] constexpr const_pointer data() const noexcept { return &m_mat[0][0]; }
		[[nodiscard]] constexpr pointer data() noexcept { return &m_mat[0][0]; }

	public: // Capacity
		[[nodiscard]] constexpr size_type rows() const noexcept { return R; }
		[[nodiscard]] constexpr size_type columns() const noexcept { return C; }

		[[nodiscard]] constexpr size_type size() const noexcept { return R * C; }
		[[nodiscard]] constexpr size_type max_size() const noexcept { return R * C; }

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
			std::fill(&m_mat[0][0], &m_mat[0][0] + size(), v);
		}

		void swap(this_type& other) noexcept
		{
			for (size_type i = 0; i < R; i++)
				std::swap(m_mat[i], other.m_mat[i]);
		}

		[[nodiscard]] bool is_null() const noexcept
		{
			for (size_type i = 0; i < size(); i++)
				if ((*this)[i] != 0)
					return false;

			return true;
		}

		[[nodiscard]] bool is_identity() const noexcept
		{
			for (size_type i = 0; i < size(); i++)
				if (i % (columns() + 1) == 0 && (*this)[i] != 1)
					return false;

			return true;
		}

		[[nodiscard]] constexpr bool is_square() const noexcept
		{
			return rows() == columns();
		}

		[[nodiscard]] static this_type make_identity() noexcept
		{
			this_type m;

			for (size_type i = 0; i < m.size(); i++)
				if (i % (C + 1) == 0)
					m[i] = 1;

			return m;
		}

		template<typename U = T, typename std::enable_if_t<std::is_floating_point_v<U>, int> = 0>
		[[nodiscard]] static this_type make_random(T minv = std::numeric_limits<T>::min(), T maxv = std::numeric_limits<T>::max()) noexcept
		{
			static std::random_device rd;
			static std::mt19937 gen(rd());
			static std::uniform_real_distribution<T> dis(minv, maxv);

			this_type temp;

			for (size_type i = 0; i < temp.size(); i++)
				temp.at(i) = dis(gen);

			return temp;
		}

		template<typename U = T, typename std::enable_if_t<std::is_integral_v<U>, int> = 0>
		[[nodiscard]] static this_type make_random(T minv = std::numeric_limits<T>::min(), T maxv = std::numeric_limits<T>::max()) noexcept
		{
			static std::random_device rd;
			static std::mt19937 gen(rd());
			static std::uniform_int_distribution<T> dis(minv, maxv);

			this_type temp;

			for (size_type i = 0; i < temp.size(); i++)
				temp.at(i) = dis(gen);

			return temp;
		}

		[[nodiscard]] matrix<value_type, C, R> transpose() const noexcept
		{
			matrix<value_type, C, R> m;

			for (size_type i = 0; i < rows(); i++)
				for (size_type j = 0; j < columns(); j++)
					m.at(j, i) = m_mat[i][j];

			return m;
		}

		template<typename = typename std::enable_if_t<(R > 2 && C > 2)>>
		[[nodiscard]] matrix<value_type, R - 1, C - 1> submatrix(size_t r, size_t c) const noexcept
		{
			matrix<value_type, R - 1, C - 1> m;

			for (size_type i = 0; i < rows() - 1; i++)
				for (size_type j = 0; j < columns() - 1; j++)
					m.at(i, j) = m_mat[i < r ? i : i + 1][j < c ? j : j + 1];

			return m;
		}

		template<size_t _C>
		[[nodiscard]] matrix<value_type, R, C + _C> augment_columns(const matrix<value_type, R, _C>& m) const noexcept
		{
			matrix<value_type, R, C + _C> r;

			for (size_t i = 0; i < rows(); i++)
				for (size_t j = 0; j < columns(); j++)
					r.at(i, j) = at(i, j);

			for (size_t i = 0; i < m.rows(); i++)
				for (size_t j = 0; j < m.columns(); j++)
					r.at(i, columns() + j) = m.at(i, j);

			return r;
		}

		template<size_t _R>
		[[nodiscard]] matrix<value_type, R + _R, C> augment_rows(const matrix<value_type, _R, C>& m) const noexcept
		{
			matrix<value_type, R + _R, C> r;

			for (size_t i = 0; i < rows(); i++)
				for (size_t j = 0; j < columns(); j++)
					r.at(i, j) = at(i, j);

			for (size_t i = 0; i < m.rows(); i++)
				for (size_t j = 0; j < m.columns(); j++)
					r.at(rows() + i, j) = m.at(i, j);

			return r;
		}

	public:
		void switch_rows(size_type r0, size_type r1)
		{
			_check_range(r0, 0);
			_check_range(r1, 0);

			if (r0 == r1)
				return;

			for (size_type i = 0; i < columns(); i++)
			{
				T temp = at(r0, i);
				at(r0, i) = at(r1, i);
				at(r1, i) = temp;
			}
		}

		void switch_columns(size_type c0, size_type c1)
		{
			_check_range(0, c0);
			_check_range(0, c1);

			if (c0 == c1)
				return;

			for (size_type i = 0; i < rows(); i++)
			{
				T temp = at(i, c0);
				at(i, c0) = at(i, c1);
				at(i, c1) = temp;
			}
		}

	private:
		template<typename U, size_t _S, typename = typename std::enable_if_t<(_S > 2 && std::is_arithmetic_v<U>)>>
		[[nodiscard]] static bool _has_identical_rows(const matrix<U, _S, _S>& m) noexcept
		{
			for (size_type i = 0; i < m.rows(); i++)
				for (size_type j = i + 1; j < m.rows(); j++)
					if (i != j)
						if (std::equal(m.begin() + i * m.columns(), m.begin() + (i + 1) * m.columns(), m.begin() + j * m.columns()))
							return true;

			return false;
		}

		template<typename U, size_t _S, typename std::enable_if_t<(_S >= 2 && std::is_arithmetic_v<U>), int> = 0>
		[[nodiscard]] static U _gaussian_determinant(const matrix<U, _S, _S>& m) noexcept
		{
			if constexpr (_S == 2)
				return m.at(0, 0) * m.at(1, 1) - m.at(0, 1) * m.at(1, 0);

			matrix<U, _S, _S> temp = m;

			size_type max_idx = 0;

			for (size_type i = 0; i < temp.rows(); i++)
				if (std::abs(temp.at(i, 0)) > std::abs(temp.at(max_idx, 0)))
					max_idx = i;

			if (temp.at(max_idx, 0) == 0)
				return 0;
			
			U det = 1;

			if (max_idx != 0)
			{
				temp.switch_rows(0, max_idx);
				det = -1;
			}

			for (size_type i = 0; i < temp.columns() - 1; i++)
			{
				for (size_type j = temp.rows() - 1; j > i; j--)
				{
					U r = temp.at(j, i) / temp.at(i, i);

					for (size_type k = 0; k < temp.columns(); k++)
						temp.at(j, k) -= r * temp.at(i, k);
				}
			}

			for (size_type i = 0; i < temp.size(); i++)
				if (i % (temp.columns() + 1) == 0)
					det *= temp.at(i);

			return det;
		}

		template<typename U, size_t _S, typename std::enable_if_t<(_S > 2 && std::is_arithmetic_v<U>), int> = 0>
		[[nodiscard]] static U _recursive_determinant(const matrix<U, _S, _S>& m) noexcept
		{
			value_type det = 0;

			for (size_type i = 0; i < _S; i++)
				det += m.at(0, i) * _recursive_determinant(m.submatrix(0, i)) * (i % 2 ? -1 : 1);

			return det;
		}

		template<typename U, size_t _S, typename std::enable_if_t<(_S == 2 && std::is_arithmetic_v<U>), int> = 0>
		[[nodiscard]] static U _recursive_determinant(const matrix<U, _S, _S>& m) noexcept
		{
			return m.at(0, 0) * m.at(1, 1) - m.at(0, 1) * m.at(1, 0);
		}

	public:
		template<typename = typename std::enable_if_t<(R == C)>>
		[[nodiscard]] value_type determinant() const noexcept
		{
			return _recursive_determinant(*this);
		}

		template<typename = typename std::enable_if_t<(R == C)>>
		[[nodiscard]] this_type minors() const noexcept
		{
			this_type m;

			for (size_type i = 0; i < rows(); i++)
				for (size_type j = 0; j < columns(); j++)
					m.at(i, j) = submatrix(i, j).determinant();

			return m;
		}

		template<typename = typename std::enable_if_t<(R == C)>>
		[[nodiscard]] this_type cofactors() const noexcept
		{
			this_type m = minors();

			for (size_type i = 1; i < size(); i += 2)
				m[i] *= -1;

			return m;
		}

		template<typename = typename std::enable_if_t<(R == C)>>
		[[nodiscard]] this_type inverse() const noexcept
		{
			this_type cof = cofactors();
			this_type adj = cof.transpose();
			value_type det = determinant();

			return adj * (1.0f / det);
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
			return std::equal(begin(), end(), rhs.begin());
		}

		[[nodiscard]] bool operator!=(const this_type& rhs) const
		{
			return !(*this == rhs);
		}

		[[nodiscard]] bool operator<(const this_type& rhs) const
		{
			return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
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
			std::copy(rhs.begin(), rhs.end(), begin());
			return *this;
		}

		[[nodiscard]] this_type operator+() const
		{
			return *this;
		}

		[[nodiscard]] this_type operator-() const
		{
			return *this * -1;
		}

		[[nodiscard]] this_type operator+(const this_type& rhs) const
		{
			this_type m = *this;

			for (size_type i = 0; i < size(); i++)
				m[i] += rhs[i];

			return m;
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

		[[nodiscard]] this_type operator*(const_reference rhs) const
		{
			this_type m = *this;

			for (size_type i = 0; i < size(); i++)
				m[i] *= rhs;

			return m;
		}

		this_type& operator*=(const_reference rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		template<size_t _D>
		[[nodiscard]] matrix<value_type, R, _D> operator*(const matrix<value_type, C, _D>& rhs) const
		{
			value_type m[R][_D] = { 0 };

			for (size_type i = 0; i < R; i++)
				for (size_type j = 0; j < _D; j++)
					for (size_type k = 0; k < C; k++)
						m[i][j] += m_mat[i][k] * rhs.m_mat[k][j];

			return matrix<value_type, R, _D>(m);
		}

		template<size_t _D>
		matrix<value_type, R, _D>& operator*=(const matrix<value_type, C, _D>& rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		[[nodiscard]] this_type operator/(const_reference rhs) const
		{
			this_type m = *this;

			for (size_type i = 0; i < size(); i++)
				m[i] /= rhs;

			return m;
		}

		this_type& operator/=(const_reference rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& os, const matrix& m)
		{
			os << "[";

			for (size_type i = 0; i < m.rows(); i++)
			{
				os << "[";

				for (size_type j = 0; j < m.columns(); j++)
				{
					os << m.m_mat[i][j];

					if (j + 1 < m.columns())
						os << ",";
				}

				os << "]";

				if (i + 1 < m.rows())
					os << ",";
			}

			os << "]";

			return os;
		}

	private: // Variables
		value_type m_mat[R][C];
	};

	template<class T>
	using mat2 = matrix<T, 2, 2>;

	using mat2i = mat2<int>;
	using mat2l = mat2<long>;
	using mat2ll = mat2<long long>;
	using mat2f = mat2<float>;
	using mat2d = mat2<double>;
	using mat2ld = mat2<long double>;

	template<class T>
	using mat3 = matrix<T, 3, 3>;

	using mat3i = mat3<int>;
	using mat3l = mat3<long>;
	using mat3ll = mat3<long long>;
	using mat3f = mat3<float>;
	using mat3d = mat3<double>;
	using mat3ld = mat3<long double>;

	template<class T>
	using mat4 = matrix<T, 4, 4>;

	using mat4i = mat4<int>;
	using mat4l = mat4<long>;
	using mat4ll = mat4<long long>;
	using mat4f = mat4<float>;
	using mat4d = mat4<double>;
	using mat4ld = mat4<long double>;

	template<size_t R, size_t C>
	using mati = matrix<int, R, C>;

	template<size_t R, size_t C>
	using matl = matrix<long, R, C>;

	template<size_t R, size_t C>
	using matll = matrix<long long, R, C>;

	template<size_t R, size_t C>
	using matf = matrix<float, R, C>;

	template<size_t R, size_t C>
	using matd = matrix<double, R, C>;

	template<size_t R, size_t C>
	using matld = matrix<long double, R, C>;
}
