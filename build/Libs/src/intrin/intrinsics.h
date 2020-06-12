#pragma once

#include <cstdint>
#include <type_traits>

#include <intrin.h>

namespace rb::intrin
{
	// ADDITION

	template<typename T>
	inline void add256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void add256(const uint8_t* a, const uint8_t* b, uint8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_adds_epu8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void add256(const uint16_t* a, const uint16_t* b, uint16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_adds_epu16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void add256(const int8_t* a, const int8_t* b, int8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_add_epi8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void add256(const int16_t* a, const int16_t* b, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_add_epi16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void add256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_add_epi32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void add256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_add_epi64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void add256(const float* a, const float* b, float* res) noexcept
	{
		__m256 ma = _mm256_loadu_ps(a);
		__m256 mb = _mm256_loadu_ps(b);

		__m256 mr = _mm256_add_ps(ma, mb);

		_mm256_storeu_ps(res, mr);
	}

	template<>
	inline void add256(const double* a, const double* b, double* res) noexcept
	{
		__m256d ma = _mm256_loadu_pd(a);
		__m256d mb = _mm256_loadu_pd(b);

		__m256d mr = _mm256_add_pd(ma, mb);

		_mm256_storeu_pd(res, mr);
	}

	// SUBTRACTION

	template<typename T>
	inline void sub256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void sub256(const uint8_t* a, const uint8_t* b, uint8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_subs_epu8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void sub256(const uint16_t* a, const uint16_t* b, uint16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_subs_epu16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void sub256(const int8_t* a, const int8_t* b, int8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_sub_epi8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void sub256(const int16_t* a, const int16_t* b, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_sub_epi16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void sub256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_sub_epi32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void sub256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_sub_epi64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void sub256(const float* a, const float* b, float* res) noexcept
	{
		__m256 ma = _mm256_loadu_ps(a);
		__m256 mb = _mm256_loadu_ps(b);

		__m256 mr = _mm256_sub_ps(ma, mb);

		_mm256_storeu_ps(res, mr);
	}

	template<>
	inline void sub256(const double* a, const double* b, double* res) noexcept
	{
		__m256d ma = _mm256_loadu_pd(a);
		__m256d mb = _mm256_loadu_pd(b);

		__m256d mr = _mm256_sub_pd(ma, mb);

		_mm256_storeu_pd(res, mr);
	}

	// MULTIPLICATION

	template<typename T>
	inline void mul256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void mul256(const uint32_t* a, const uint32_t* b, uint32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_mul_epu32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void mul256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_mul_epi32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void mul256(const float* a, const float* b, float* res) noexcept
	{
		__m256 ma = _mm256_loadu_ps(a);
		__m256 mb = _mm256_loadu_ps(b);

		__m256 mr = _mm256_mul_ps(ma, mb);

		_mm256_storeu_ps(res, mr);
	}

	template<>
	inline void mul256(const double* a, const double* b, double* res) noexcept
	{
		__m256d ma = _mm256_loadu_pd(a);
		__m256d mb = _mm256_loadu_pd(b);

		__m256d mr = _mm256_mul_pd(ma, mb);

		_mm256_storeu_pd(res, mr);
	}

	// DIVISION

	template<typename T>
	inline void div256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void div256(const uint8_t* a, const uint8_t* b, uint8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epu8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const uint16_t* a, const uint16_t* b, uint16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epu16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const uint32_t* a, const uint32_t* b, uint32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epu32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const uint64_t* a, const uint64_t* b, uint64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epu64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const int8_t* a, const int8_t* b, int8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epi8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const int16_t* a, const int16_t* b, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epi16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epi32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_div_epi64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void div256(const float* a, const float* b, float* res) noexcept
	{
		__m256 ma = _mm256_loadu_ps(a);
		__m256 mb = _mm256_loadu_ps(b);

		__m256 mr = _mm256_div_ps(ma, mb);

		_mm256_storeu_ps(res, mr);
	}

	template<>
	inline void div256(const double* a, const double* b, double* res) noexcept
	{
		__m256d ma = _mm256_loadu_pd(a);
		__m256d mb = _mm256_loadu_pd(b);

		__m256d mr = _mm256_div_pd(ma, mb);

		_mm256_storeu_pd(res, mr);
	}

	// REMAINDER

	template<typename T>
	inline void rem256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void rem256(const uint8_t* a, const uint8_t* b, uint8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epu8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rem256(const uint16_t* a, const uint16_t* b, uint16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epu16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rem256(const uint32_t* a, const uint32_t* b, uint32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epu32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rem256(const uint64_t* a, const uint64_t* b, uint64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epu64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rem256(const int8_t* a, const int8_t* b, int8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epi8(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rem256(const int16_t* a, const int16_t* b, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epi16(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rem256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epi32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rem256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_rem_epi64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	// BITWISE AND

	template<typename T>
	inline void and256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void and256(const uint8_t* a, const uint8_t* b, uint8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const uint16_t* a, const uint16_t* b, uint16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const uint32_t* a, const uint32_t* b, uint32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const uint64_t* a, const uint64_t* b, uint64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const int8_t* a, const int8_t* b, int8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const int16_t* a, const int16_t* b, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_and_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void and256(const float* a, const float* b, float* res) noexcept
	{
		__m256 ma = _mm256_loadu_ps(a);
		__m256 mb = _mm256_loadu_ps(b);

		__m256 mr = _mm256_and_ps(ma, mb);

		_mm256_storeu_ps(res, mr);
	}

	template<>
	inline void and256(const double* a, const double* b, double* res) noexcept
	{
		__m256d ma = _mm256_loadu_pd(a);
		__m256d mb = _mm256_loadu_pd(b);

		__m256d mr = _mm256_and_pd(ma, mb);

		_mm256_storeu_pd(res, mr);
	}

	// BITWISE OR

	template<typename T>
	inline void or256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void or256(const uint8_t* a, const uint8_t* b, uint8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const uint16_t* a, const uint16_t* b, uint16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const uint32_t* a, const uint32_t* b, uint32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const uint64_t* a, const uint64_t* b, uint64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const int8_t* a, const int8_t* b, int8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const int16_t* a, const int16_t* b, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_or_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void or256(const float* a, const float* b, float* res) noexcept
	{
		__m256 ma = _mm256_loadu_ps(a);
		__m256 mb = _mm256_loadu_ps(b);

		__m256 mr = _mm256_or_ps(ma, mb);

		_mm256_storeu_ps(res, mr);
	}

	template<>
	inline void or256(const double* a, const double* b, double* res) noexcept
	{
		__m256d ma = _mm256_loadu_pd(a);
		__m256d mb = _mm256_loadu_pd(b);

		__m256d mr = _mm256_or_pd(ma, mb);

		_mm256_storeu_pd(res, mr);
	}

	// BITWISE XOR

	template<typename T>
	inline void xor256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void xor256(const uint8_t* a, const uint8_t* b, uint8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const uint16_t* a, const uint16_t* b, uint16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const uint32_t* a, const uint32_t* b, uint32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const uint64_t* a, const uint64_t* b, uint64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const int8_t* a, const int8_t* b, int8_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const int16_t* a, const int16_t* b, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_xor_si256(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void xor256(const float* a, const float* b, float* res) noexcept
	{
		__m256 ma = _mm256_loadu_ps(a);
		__m256 mb = _mm256_loadu_ps(b);

		__m256 mr = _mm256_xor_ps(ma, mb);

		_mm256_storeu_ps(res, mr);
	}

	template<>
	inline void xor256(const double* a, const double* b, double* res) noexcept
	{
		__m256d ma = _mm256_loadu_pd(a);
		__m256d mb = _mm256_loadu_pd(b);

		__m256d mr = _mm256_xor_pd(ma, mb);

		_mm256_storeu_pd(res, mr);
	}

	// BITWISE LEFT-SHIFT

	template<typename T>
	inline void lsh256(const T* a, int64_t c, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void lsh256(const int16_t* a, int64_t c, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m128i mc = _mm_set_epi64x(0LL, c);

		__m256i mr = _mm256_sll_epi16(ma, mc);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void lsh256(const int32_t* a, int64_t c, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m128i mc = _mm_set_epi64x(0LL, c);

		__m256i mr = _mm256_sll_epi32(ma, mc);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void lsh256(const int64_t* a, int64_t c, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m128i mc = _mm_set_epi64x(0LL, c);

		__m256i mr = _mm256_sll_epi64(ma, mc);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<typename T>
	inline void lsh256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void lsh256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_sllv_epi32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void lsh256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_sllv_epi64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	// BITWISE RIGHT-SHIFT

	template<typename T>
	inline void rsh256(const T* a, int64_t c, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void rsh256(const int16_t* a, int64_t c, int16_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m128i mc = _mm_set_epi64x(0LL, c);

		__m256i mr = _mm256_srl_epi16(ma, mc);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rsh256(const int32_t* a, int64_t c, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m128i mc = _mm_set_epi64x(0LL, c);

		__m256i mr = _mm256_srl_epi32(ma, mc);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rsh256(const int64_t* a, int64_t c, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m128i mc = _mm_set_epi64x(0LL, c);

		__m256i mr = _mm256_srl_epi64(ma, mc);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<typename T>
	inline void rsh256(const T* a, const T* b, T* res) noexcept
	{
		static_assert(false);
	}

	template<>
	inline void rsh256(const int32_t* a, const int32_t* b, int32_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_srlv_epi32(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	template<>
	inline void rsh256(const int64_t* a, const int64_t* b, int64_t* res) noexcept
	{
		__m256i ma = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a));
		__m256i mb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b));

		__m256i mr = _mm256_srlv_epi64(ma, mb);

		_mm256_storeu_si256(reinterpret_cast<__m256i*>(res), mr);
	}

	// STORING

	template<typename T, typename... I>
	inline void store256(T* p, I... v) noexcept
	{
		static_assert(false);
	}

	template<typename... I>
	inline void store256(uint8_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi8(v...));
	}

	template<typename... I>
	inline void store256(uint16_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi16(v...));
	}

	template<typename... I>
	inline void store256(uint32_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi32(v...));
	}

	template<typename... I>
	inline void store256(uint64_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi64x(v...));
	}

	template<typename... I>
	inline void store256(int8_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi8(v...));
	}

	template<typename... I>
	inline void store256(int16_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi16(v...));
	}

	template<typename... I>
	inline void store256(int32_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi32(v...));
	}

	template<typename... I>
	inline void store256(int64_t* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_epi64x(v...));
	}

	template<typename... I>
	inline void store256(float* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_ps(v...));
	}

	template<typename... I>
	inline void store256(double* p, I... v)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(p), _mm256_set_pd(v...));
	}
}
