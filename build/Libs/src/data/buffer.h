#pragma once

#include <initializer_list>
#include <cstdlib>
#include <utility>
#include <limits>
#include <memory>
#include <new>

#include "iter/iterator.h"

namespace rb::data
{
	template<typename T, class A = std::allocator<T>>
	class buffer
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		using this_type = buffer<value_type>;

		using iterator = iter::iterator<value_type>;
		using const_iterator = iter::const_iterator<value_type>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using allocator_type = A;

	public:
		buffer() noexcept
			: m_size(0), m_buf(nullptr)
		{
		}

		explicit buffer(size_type n)
			: m_size(n), m_buf(nullptr)
		{
			m_buf = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size);
		}

		buffer(std::initializer_list<value_type> il)
			: buffer(il.size())
		{
			std::memcpy(m_buf, il.begin(), m_size * sizeof(value_type));
		}

		buffer(const this_type& other)
			: buffer(other.m_size)
		{
			std::memcpy(m_buf, other.m_buf, m_size * sizeof(value_type));
		}

		buffer(this_type&& other)
			: buffer(other.m_size)
		{
			m_alloc = std::move(other.m_alloc);
			m_buf = std::move(other.m_buf);
			other.m_size = 0;
			other.m_buf = nullptr;
		}

		~buffer()
		{
			std::allocator_traits<allocator_type>::deallocate(m_alloc, m_buf, m_size);
		}

	public:
		this_type& operator=(const this_type& rhs)
		{
			std::allocator_traits<allocator_type>::deallocate(m_alloc, m_buf, m_size);
			
			m_size = rhs.m_size;
			m_buf = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size);
			
			std::memcpy(m_buf, rhs.m_buf, m_size * sizeof(value_type));

			return *this;
		}

		this_type& operator=(this_type&& rhs)
		{
			std::allocator_traits<allocator_type>::deallocate(m_alloc, m_buf, m_size);
			
			m_size = rhs.m_size;
			m_alloc = std::move(rhs.m_alloc);
			m_buf = std::move(rhs.m_buf);

			rhs.m_size = 0;
			rhs.m_buf = nullptr;

			return *this;
		}

		[[nodiscard]] size_type size() const noexcept { return m_size; }
		[[nodiscard]] constexpr size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); }

		[[nodiscard]] bool empty() const noexcept { return m_size == 0; }

		[[nodiscard]] pointer data() noexcept { return m_buf; }
		[[nodiscard]] const_pointer data() const noexcept { return m_buf; }

		[[nodiscard]] allocator_type get_allocator() const noexcept { return m_alloc; }

		void swap(this_type& other) noexcept
		{
			std::swap(m_size, other.m_size);
			std::swap(m_alloc, other.m_alloc);
			std::swap(m_buf, other.m_buf);
		}

		[[nodiscard]] iterator begin() noexcept { return iterator(data()); }
		[[nodiscard]] iterator end() noexcept { return iterator(data() + size()); }
		
		[[nodiscard]] const_iterator begin() const noexcept { return const_iterator(data()); }
		[[nodiscard]] const_iterator end() const noexcept { return const_iterator(data() + size()); }
		
		[[nodiscard]] const_iterator cbegin() const noexcept { return begin(); }
		[[nodiscard]] const_iterator cend() const noexcept { return end(); }
		
		[[nodiscard]] reverse_iterator rbegin() noexcept { return reverse_iterator(begin()); }
		[[nodiscard]] reverse_iterator rend() noexcept { return reverse_iterator(end()); }
		
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(begin()); }
		[[nodiscard]] const_reverse_iterator rend() const noexcept { return const_reverse_iterator(end()); }
		
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return rbegin(); }
		[[nodiscard]] const_reverse_iterator crend() const noexcept { return rend(); }

		[[nodiscard]] reference front() noexcept { return m_buf[0]; }
		[[nodiscard]] const_reference front() const noexcept { return m_buf[0]; }

		[[nodiscard]] reference back() noexcept { return m_buf[m_size - 1]; }
		[[nodiscard]] const_reference back() const noexcept { return m_buf[m_size - 1]; }

		[[nodiscard]] reference at(size_type idx) noexcept { return m_buf[idx]; }
		[[nodiscard]] const_reference at(size_type idx) const noexcept { return m_buf[idx]; }
		
		[[nodiscard]] reference operator[](size_type idx) noexcept { return m_buf[idx]; }
		[[nodiscard]] const_reference operator[](size_type idx) const noexcept { return m_buf[idx]; }

		[[nodiscard]] bool operator==(const this_type& rhs) const noexcept
		{
			if (size() != rhs.size())
				return false;

			for (size_type i = 0; i < size(); i++)
				if (at(i) != rhs.at(i))
					return false;

			return true;
		}

		[[nodiscard]] bool operator!=(const this_type& rhs) const noexcept { return !(*this == rhs); }

	private:
		size_type m_size;
		allocator_type m_alloc;
		pointer m_buf;
	};
}
