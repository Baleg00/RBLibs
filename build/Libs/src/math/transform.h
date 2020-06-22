#pragma once

#include "math/matrix.h"
#include "math/vector.h"

namespace rb::math
{
	template<class T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr T PI = T(3.14159265358979323846L);

	template<class T, size_t S, size_t C>
	[[nodiscard]] vector<T, C> operator*(const vector<T, S>& lhs, const matrix<T, S, C>& rhs)
	{
		vector<T, C> v;

		for (size_t i = 0; i < C; i++)
			for (size_t j = 0; j < S; j++)
				v[i] += lhs[j] * rhs.at(j, i);

		return v;
	}

	template<class T, size_t S>
	[[nodiscard]] matrix<T, S, S> translate(const matrix<T, S, S>& m, const vector<T, S>& v) noexcept
	{
		matrix<T, S, S> _m = matrix<T, S, S>::make_identity();

		for (size_t i = 0; i < S - 1; i++)
			_m.at(S - 1, i) = v[i];

		return m * _m;
	}

	template<class T, size_t S>
	[[nodiscard]] matrix<T, S, S> scale(const matrix<T, S, S>& m, const vector<T, S>& v) noexcept
	{
		matrix<T, S, S> _m;

		for (size_t i = 0, j = 0; i < S * S && j < S; i++)
			if (i % (S + 1) == 0)
				_m[i] = v[j++];

		return m * _m;
	}

	template<class T>
	[[nodiscard]] mat3<T> shear(const mat3<T>& m, const vec3<T>& v) noexcept
	{
		return m * mat3<T>{
			1, v[0], 0,
			v[1], 1, 0,
			0, 0, 1
		};
	}

	template<class T>
	[[nodiscard]] mat3<T> rotate(const mat3<T>& m, T t) noexcept
	{
		return m * mat3<T>{
			std::cosf(t), -std::sinf(t), 0,
			std::sinf(t), std::cosf(t), 0,
			0, 0, 1
		};
	}

	template<class T>
	[[nodiscard]] mat4<T> rotate(const mat4<T>& m, const vec4<T>& v, T t) noexcept
	{
		return m * mat4<T>{
			v[0] * v[0] * (1 - std::cosf(t)) + std::cosf(t), v[1] * v[0] * (1 - std::cosf(t)) - v[2] * std::sinf(t), v[2] * v[0] * (1 - std::cosf(t)) + v[1] * std::sinf(t), 0,
			v[0] * v[1] * (1 - std::cosf(t)) + v[2] * std::sinf(t), v[1] * v[1] * (1 - std::cosf(t)) + std::cosf(t), v[2] * v[1] * (1 - std::cosf(t)) - v[0] * std::sinf(t), 0,
			v[0] * v[2] * (1 - std::cosf(t)) - v[1] * std::sinf(t), v[1] * v[2] * (1 - std::cosf(t)) + v[0] * std::sinf(t), v[2] * v[2] * (1 - std::cosf(t)) + std::cosf(t), 0,
			0, 0, 0, 1
		};
	}

	template<class T>
	[[nodiscard]] mat4<T> rotate_x(const mat4<T>& m, T t) noexcept
	{
		return m * mat4<T>{
			1, 0, 0, 0,
			0, std::cosf(t), -std::sinf(t), 0,
			0, std::sinf(t), std::cosf(t), 0,
			0, 0, 0, 1
		};
	}

	template<class T>
	[[nodiscard]] mat4<T> rotate_y(const mat4<T>& m, T t) noexcept
	{
		return m * mat4<T>{
			std::cosf(t), 0, std::sinf(t), 0,
			0, 1, 0, 0,
			-std::sinf(t), 0, std::cosf(t), 0,
			0, 0, 0, 1
		};
	}

	template<class T>
	[[nodiscard]] mat4<T> rotate_z(const mat4<T>& m, T t) noexcept
	{
		return m * mat4<T>{
			std::cosf(t), -std::sinf(t), 0, 0,
			std::sinf(t), std::cosf(t), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template<class T>
	[[nodiscard]] mat4<T> perspective(const mat4<T>& m, T width, T height, T far, T near, T fov) noexcept
	{
		const T s = 1.0f / std::tanf(fov / 2.0f);

		return m * mat4<T>{
			height / width * s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, -far / (far - near), 1,
			0, 0, -(far * near) / (far - near), 0
		};
	}

	template<class T>
	[[nodiscard]] mat4<T> orthographic(const mat4<T>& m, T left, T right, T bottom, T top, T near, T far) noexcept
	{
		return m * mat4<T>{
			2.0f / (right - left), 0, 0, -(right + left) / (right - left),
			0, 2.0f / (top - bottom), 0, -(top + bottom) / (top - bottom),
			0, 0, -2.0f / (far - near), -(far + near) / (far - near),
			0, 0, 0, 1
		};
	}

	template<class T>
	[[nodiscard]] vec4<T> project(const mat4<T>& m, const vec4<T>& v) noexcept
	{
		vec4<T> _v = v * m;

		if (_v[3] != 0)
			_v /= _v[3];

		return _v;
	}
}
