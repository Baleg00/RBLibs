#pragma once

#include <exception>

#include <algorithm>

#include <iostream>

#include <type_traits>
#include <initializer_list>

namespace rb::math
{
	template<class T, size_t R, size_t C>
	class matrix
	{
	public:
		static_assert(R > 0 && C > 0, "`matrix<T, R, C>`: matrix must have at least 2 rows `R` and 2 columns `C`");
		static_assert(std::is_arithmetic_v<T>, "`matrix<T, R, C>`: matrix type `T` must be arithmetic");

		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;

		using this_type = matrix<T, R, C>;

		static constexpr size_t ROWS = R;
		static constexpr size_t COLS = C;

		static constexpr size_t SIZE = R * C;

	private:
		static constexpr bool IS_SQUARE = R == C;

	public:
		constexpr matrix() noexcept
			: m_mat{ 0 }
		{
		}

		matrix(std::initializer_list<value_type> il) noexcept
			: matrix()
		{
			for (size_t i = 0; i < std::min(il.size(), SIZE); i++)
				at(i) = il.begin()[i];
		}

		explicit constexpr matrix(const value_type (&m)[R][C]) noexcept
			: matrix()
		{
			for (size_t r = 0; r < ROWS; r++)
				for (size_t c = 0; c < COLS; c++)
					at(r, c) = m[r][c];
		}

		constexpr matrix(const_pointer begin, const_pointer end) noexcept
			: matrix()
		{
			for (size_t i = 0; i < std::min(static_cast<size_t>(end - begin), SIZE); i++)
				at(i) = begin[i];
		}

		constexpr matrix(const_pointer begin, size_t size) noexcept
			: matrix()
		{
			for (size_t i = 0; i < std::min(size, this->SIZE); i++)
				at(i) = begin[i];
		}

		constexpr matrix(const this_type& m) noexcept
			: matrix()
		{
			for (size_t i = 0; i < SIZE; i++)
				at(i) = m.at(i);
		}

		constexpr matrix(this_type&& m) noexcept
			: matrix()
		{
			for (size_t i = 0; i < SIZE; i++)
				at(i) = std::move(m.at(i));
		}

	public:
		template<typename = typename std::enable_if_t<IS_SQUARE>>
		[[nodiscard]] static constexpr this_type IDENTITY() noexcept
		{
			this_type identity;

			for (size_t i = 0; i < SIZE; i += ROWS + 1)
				identity.at(i) = 1;

			return identity;
		}

	private:
		[[noreturn]] static void _Xrange()
		{
			throw std::out_of_range("`matrix<T, R, C>`: subscript out of range");
		}

		constexpr void _check_range(size_t r, size_t c) const
		{
			if (r >= ROWS || c >= COLS)
				_Xrange();
		}

		constexpr void _check_range(size_t idx) const
		{
			if (idx >= SIZE)
				_Xrange();
		}

	public:
		[[nodiscard]] constexpr const_reference at(size_t idx) const
		{
			_check_range(idx);
			return m_mat[idx / COLS][idx % COLS];
		}

		[[nodiscard]] constexpr reference at(size_t idx)
		{
			_check_range(idx);
			return m_mat[idx / COLS][idx % COLS];
		}
		
		[[nodiscard]] constexpr const_reference at(size_t r, size_t c) const
		{
			_check_range(r, c);
			return m_mat[r][c];
		}

		[[nodiscard]] constexpr reference at(size_t r, size_t c)
		{
			_check_range(r, c);
			return m_mat[r][c];
		}

	public:
		[[nodiscard]] constexpr pointer data() noexcept { return &m_mat[0][0]; }
		[[nodiscard]] constexpr const_pointer data() const noexcept { return &m_mat[0][0]; }
		
		[[nodiscard]] constexpr pointer begin() noexcept { return data(); }
		[[nodiscard]] constexpr const_pointer begin() const noexcept { return data(); }

		[[nodiscard]] constexpr pointer end() noexcept { return data() + SIZE; }
		[[nodiscard]] constexpr const_pointer end() const noexcept { return data() + SIZE; }

	public:
		constexpr void fill(const_reference v) noexcept
		{ 
			for (size_t i = 0; i < SIZE; i++)
				at(i) = v;
		}

		constexpr void swap(this_type& other) noexcept
		{
			value_type temp;
			for (size_t i = 0; i < R; i++)
			{
				temp = at(i);
				at(i) = other.at(i);
				other.at(i) = temp;
			}
		}

		[[nodiscard]] constexpr bool is_null() const noexcept
		{
			for (size_t i = 0; i < SIZE; i++)
				if (at(i) != 0)
					return false;

			return true;
		}

		template<typename = typename std::enable_if_t<IS_SQUARE>>
		[[nodiscard]] constexpr bool is_identity() const noexcept
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				if (i % (ROWS + 1) == 0)
				{
					if (at(i) != 1)
						return false;
				}
				else
				{
					if (at(i) != 0)
						return false;
				}
			}

			return true;
		}

		[[nodiscard]] constexpr bool is_square() const noexcept
		{
			return IS_SQUARE;
		}

		[[nodiscard]] constexpr matrix<value_type, COLS, ROWS> transpose() const noexcept
		{
			matrix<value_type, COLS, ROWS> m;

			for (size_t r = 0; r < ROWS; r++)
				for (size_t c = 0; c < COLS; c++)
					m.at(c, r) = at(r, c);

			return m;
		}

		template<typename = typename std::enable_if_t<(ROWS > 1 && COLS > 1)>>
		[[nodiscard]] constexpr matrix<value_type, ROWS - 1, COLS - 1> submatrix(size_t r, size_t c) const noexcept
		{
			matrix<value_type, ROWS - 1, COLS - 1> m;

			for (size_t i = 0; i < ROWS - 1; i++)
				for (size_t j = 0; j < COLS - 1; j++)
					m.at(i, j) = at(i < r ? i : i + 1, j < c ? j : j + 1);

			return m;
		}

		template<size_t _C>
		[[nodiscard]] constexpr matrix<value_type, ROWS, COLS + _C> augment_columns(const matrix<value_type, ROWS, _C>& m) const noexcept
		{
			matrix<value_type, ROWS, COLS + _C> r;

			for (size_t i = 0; i < ROWS; i++)
				for (size_t j = 0; j < COLS; j++)
					r.at(i, j) = at(i, j);

			for (size_t i = 0; i < ROWS; i++)
				for (size_t j = 0; j < _C; j++)
					r.at(i, COLS + j) = m.at(i, j);

			return r;
		}

		template<size_t _R>
		[[nodiscard]] constexpr matrix<value_type, ROWS + _R, COLS> augment_rows(const matrix<value_type, _R, COLS>& m) const noexcept
		{
			matrix<value_type, ROWS + _R, COLS> r;

			for (size_t i = 0; i < ROWS; i++)
				for (size_t j = 0; j < COLS; j++)
					r.at(i, j) = at(i, j);

			for (size_t i = 0; i < _R; i++)
				for (size_t j = 0; j < COLS; j++)
					r.at(ROWS + i, j) = m.at(i, j);

			return r;
		}

	public:
		[[nodiscard]] constexpr this_type switch_rows(size_t r0, size_t r1) const
		{
			_check_range(r0, 0);
			_check_range(r1, 0);

			if (r0 == r1)
				return *this;

			this_type m = *this;

			value_type temp;
			for (size_t i = 0; i < COLS; i++)
			{
				temp = m.at(r0, i);
				m.at(r0, i) = m.at(r1, i);
				m.at(r1, i) = temp;
			}

			return m;
		}

		[[nodiscard]] constexpr this_type switch_columns(size_t c0, size_t c1) const
		{
			_check_range(0, c0);
			_check_range(0, c1);

			if (c0 == c1)
				return *this;

			this_type m = *this;

			for (size_t i = 0; i < ROWS; i++)
			{
				T temp = m.at(i, c0);
				m.at(i, c0) = m.at(i, c1);
				m.at(i, c1) = temp;
			}

			return m;
		}

	private:
		template<typename = typename std::enable_if_t<IS_SQUARE>>
		[[nodiscard]] constexpr value_type _gaussian_determinant() const noexcept
		{
			if constexpr (ROWS == 2)
				return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);

			this_type temp = *this;
			value_type det = 1;

			for (size_t i = 0; i < ROWS - 1; i++)
			{
				size_t max = i;

				for (size_t j = i + 1; j < ROWS; j++)
				{
					value_type abs_j = temp.at(j, i) < 0 ? -temp.at(j, i) : temp.at(j, i);
					value_type abs_max = temp.at(max, i) < 0 ? -temp.at(max, i) : temp.at(max, i);

					if (abs_j > abs_max)
						max = j;
				}

				if (i != max)
				{
					det = -det;
					temp = temp.switch_rows(i, max);
				}

				for (size_t r = i + 1; r < ROWS; r++)
				{
					value_type q = temp.at(r, i) / temp.at(i, i);

					for (size_t c = i; c < ROWS; c++)
						temp.at(r, c) -= q * temp.at(i, c);
				}
			}

			for (size_t i = 0; i < ROWS; i++)
				det *= temp.at(i, i);

			return det;
		}

		template<typename = typename std::enable_if_t<IS_SQUARE>>
		[[nodiscard]] constexpr value_type _recursive_determinant() const noexcept
		{
			if constexpr (ROWS == 2)
				return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);

			value_type det = 0;

			for (size_t i = 0; i < _S; i++)
				det += at(0, i) * submatrix(0, i)._recursive_determinant() * (i % 2 ? -1 : 1);

			return det;
		}

		template<typename = typename std::enable_if_t<IS_SQUARE>>
		[[nodiscard]] constexpr this_type _inverse() const noexcept
		{
			value_type det = determinant();

			if (det == 0)
				return {};

			if constexpr (ROWS == 2)
				return this_type{
					m.at(1, 1), -m.at(0, 1),
					-m.at(1, 0), m.at(0, 0)
				} / det;

			this_type cof = cofactors();
			this_type adj = cof.transpose();

			return adj / det;
		}

	public:
		template<typename = typename std::enable_if_t<IS_SQUARE>>
		[[nodiscard]] constexpr value_type determinant() const noexcept
		{
			return _gaussian_determinant();
		}

		template<typename = typename std::enable_if_t<(IS_SQUARE && ROWS > 1)>>
		[[nodiscard]] constexpr this_type minors() const noexcept
		{
			this_type m;

			for (size_t i = 0; i < ROWS; i++)
				for (size_t j = 0; j < COLS; j++)
					m.at(i, j) = submatrix(i, j).determinant();

			return m;
		}

		template<typename = typename std::enable_if_t<(IS_SQUARE && ROWS > 1)>>
		[[nodiscard]] constexpr this_type cofactors() const noexcept
		{
			this_type m = minors();

			for (size_t i = 1; i < SIZE; i += 2)
				m.at(i) *= -1;

			return m;
		}

		template<typename = typename std::enable_if_t<IS_SQUARE, int>>
		[[nodiscard]] constexpr this_type inverse() const
		{
			return _inverse();
		}

	public:
		[[nodiscard]] constexpr const_reference operator[](size_t idx) const
		{
			return at(idx);
		}

		[[nodiscard]] constexpr reference operator[](size_t idx)
		{
			return at(idx);
		}

		[[nodiscard]] constexpr bool operator==(const this_type& rhs) const
		{
			for (size_t i = 0; i < SIZE; i++)
				if (at(i) != rhs.at(i))
					return false;

			return true;
		}

		[[nodiscard]] constexpr bool operator!=(const this_type& rhs) const
		{
			return !(*this == rhs);
		}

		constexpr this_type& operator=(const this_type& rhs)
		{
			for (size_t i = 0; i < SIZE; i++)
				at(i) = rhs.at(i);

			return *this;
		}

		[[nodiscard]] constexpr this_type operator+() const
		{
			return *this;
		}

		[[nodiscard]] constexpr this_type operator-() const
		{
			return *this * -1;
		}

		[[nodiscard]] constexpr this_type operator+(const this_type& rhs) const
		{
			this_type m = *this;

			for (size_t i = 0; i < SIZE; i++)
				m.at(i) += rhs.at(i);

			return m;
		}

		constexpr this_type& operator+=(const this_type& rhs)
		{
			*this = *this + rhs;
			return *this;
		}

		[[nodiscard]] constexpr this_type operator-(const this_type& rhs) const
		{
			return *this + -rhs;
		}

		constexpr this_type& operator-=(const this_type& rhs)
		{
			*this = *this - rhs;
			return *this;
		}

		[[nodiscard]] constexpr this_type operator*(const_reference rhs) const
		{
			this_type m = *this;

			for (size_t i = 0; i < SIZE; i++)
				m.at(i) *= rhs;

			return m;
		}

		constexpr this_type& operator*=(const_reference rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		[[nodiscard]] constexpr matrix<value_type, ROWS, ROWS> operator*(const matrix<value_type, COLS, ROWS>& rhs) const
		{
			matrix<value_type, ROWS, ROWS> m;

			for (size_t i = 0; i < ROWS; i++)
				for (size_t j = 0; j < ROWS; j++)
					for (size_t k = 0; k < COLS; k++)
						m.at(i, j) += at(i, k) * rhs.at(k, j);

			return m;
		}

		[[nodiscard]] constexpr this_type operator/(const_reference rhs) const
		{
			this_type m = *this;

			for (size_t i = 0; i < SIZE; i++)
				m.at(i) /= rhs;

			return m;
		}

		constexpr this_type& operator/=(const_reference rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& os, const matrix& m)
		{
			os << "[";

			for (size_t i = 0; i < ROWS; i++)
			{
				os << "[";

				for (size_t j = 0; j < COLS; j++)
				{
					os << m.m_mat[i][j];

					if (j + 1 < COLS)
						os << ",";
				}

				os << "]";

				if (i + 1 < ROWS)
					os << ",";
			}

			os << "]";

			return os;
		}

	private:
		value_type m_mat[ROWS][COLS];
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
