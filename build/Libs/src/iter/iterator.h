#pragma once

#include <iterator>

namespace rb::iter
{
	template<class T>
	class iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using difference_type = std::ptrdiff_t;
		using this_type = iterator<value_type>;

	public:
		constexpr explicit iterator(pointer ptr)
			: m_ptr(ptr)
		{
		}

	public:
		this_type& operator++() { m_ptr++; return *this; }
		this_type operator++(int) { this_type temp(*this); operator++(); return temp; }

		this_type& operator--() { m_ptr--; return *this; }
		this_type operator--(int) { this_type temp(*this); operator--(); return temp; }

		[[nodiscard]] this_type operator+(const difference_type& off) const { return this_type(m_ptr + off); }
		this_type& operator+=(const difference_type& off) { *this = *this + off; return *this; }

		[[nodiscard]] this_type operator-(const difference_type& off) const { return this_type(m_ptr - off); }
		this_type& operator-=(const difference_type& off) { *this = *this - off; return *this; }

		[[nodiscard]] difference_type operator-(const this_type& rhs) const { return m_ptr - rhs.m_ptr; }

		[[nodiscard]] reference operator*() { return *m_ptr; }
		[[nodiscard]] pointer operator->() { return m_ptr; }
		[[nodiscard]] reference operator[](const difference_type& off) { return *(*this + off); }

		[[nodiscard]] bool operator==(const this_type& rhs) const { return m_ptr == rhs.m_ptr; }
		[[nodiscard]] bool operator!=(const this_type& rhs) const { return m_ptr != rhs.m_ptr; }
		[[nodiscard]] bool operator<(const this_type& rhs) const { return m_ptr < rhs.m_ptr; }
		[[nodiscard]] bool operator<=(const this_type& rhs) const { return m_ptr <= rhs.m_ptr; }
		[[nodiscard]] bool operator>(const this_type& rhs) const { return m_ptr > rhs.m_ptr; }
		[[nodiscard]] bool operator>=(const this_type& rhs) const { return m_ptr >= rhs.m_ptr; }

	private:
		pointer m_ptr;
	};

	template<class T>
	class const_iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
		using this_type = const_iterator<T>;

	public:
		constexpr explicit const_iterator(pointer ptr)
			: m_ptr(ptr)
		{
		}

	public:
		this_type& operator++() { m_ptr++; return *this; }
		this_type operator++(int) { this_type temp(*this); operator++(); return temp; }

		this_type& operator--() { m_ptr--; return *this; }
		this_type operator--(int) { this_type temp(*this); operator--(); return temp; }

		[[nodiscard]] this_type operator+(const difference_type& off) const { return this_type(m_ptr + off); }
		this_type& operator+=(const difference_type& off) { *this = *this + off; return *this; }

		[[nodiscard]] this_type operator-(const difference_type& off) const { return this_type(m_ptr - off); }
		this_type& operator-=(const difference_type& off) { *this = *this - off; return *this; }

		[[nodiscard]] difference_type operator-(const this_type& rhs) const { return m_ptr - rhs.m_ptr; }

		[[nodiscard]] reference operator*() { return *m_ptr; }
		[[nodiscard]] pointer operator->() { return m_ptr; }
		[[nodiscard]] reference operator[](const difference_type& off) { return *(*this + off); }

		[[nodiscard]] bool operator==(const this_type& rhs) const { return m_ptr == rhs.m_ptr; }
		[[nodiscard]] bool operator!=(const this_type& rhs) const { return m_ptr != rhs.m_ptr; }
		[[nodiscard]] bool operator<(const this_type& rhs) const { return m_ptr < rhs.m_ptr; }
		[[nodiscard]] bool operator<=(const this_type& rhs) const { return m_ptr <= rhs.m_ptr; }
		[[nodiscard]] bool operator>(const this_type& rhs) const { return m_ptr > rhs.m_ptr; }
		[[nodiscard]] bool operator>=(const this_type& rhs) const { return m_ptr >= rhs.m_ptr; }

	private:
		pointer m_ptr;
	};
}
