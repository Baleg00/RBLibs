#pragma once

#include <tuple>
#include <utility>
#include <random>
#include <type_traits>

#include "math/bigint.h"
#include "crypto/sha2.h"

namespace rb::crypto
{
	using namespace rb::math::literals;

	struct ec_secp256k1
	{
		using hash_type = typename rb::crypto::sha2_256;

		static inline const rb::math::bigint p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"_bigint;
		static inline const rb::math::bigint a = "0000000000000000000000000000000000000000000000000000000000000000"_bigint;
		static inline const rb::math::bigint b = "0000000000000000000000000000000000000000000000000000000000000007"_bigint;
		static inline const rb::math::bigint n = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"_bigint;
		static inline const rb::math::bigint h = "0000000000000000000000000000000000000000000000000000000000000001"_bigint;
		
		static inline const rb::math::bigint Gx = "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"_bigint;
		static inline const rb::math::bigint Gy = "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"_bigint;
	};

	template<typename C>
	class ECDSA
	{
	public:
		using params = typename C;
		
		using hash_type = typename params::hash_type;

		using int_type = typename rb::math::bigint;
		
	private:
		using affine_point_type = typename std::tuple<int_type, int_type>;
		using jacobian_point_type = typename std::tuple<int_type, int_type, int_type>;
		
	public:
		using private_key_type = typename int_type;
		using public_key_type = typename affine_point_type;
		using key_pair_type = typename std::tuple<private_key_type, public_key_type>;

		using signature_type = typename std::tuple<int_type, int_type>;

	private:
		static inline affine_point_type AFFINE_INFINITY = { 0, 0 };
		static inline jacobian_point_type JACOBIAN_INFINITY = { 1, 1, 0 };
		
		static inline affine_point_type GENERATOR = { params::Gx, params::Gy };
		
	public:
		ECDSA() noexcept = delete;
		ECDSA(const ECDSA&) noexcept = delete;
		ECDSA(ECDSA&&) noexcept = delete;

	private:
		// Calculate modular multiplicative inverse (a * a^-1 = 1 (mod n)) using the Extended Euclidean Algorithm
		// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
		[[nodiscard]] static int_type inverse(int_type a, const int_type& n) noexcept
		{
			int_type t = int_type::ZERO(), newt = int_type::ONE();
			int_type r = n, newr = a;
			int_type q, oldt, oldr;

			while (!newr.is_zero())
			{
				q = r / newr;

				oldt = t;
				t = newt;
				newt = oldt - q * newt;
				
				oldr = r;
				r = newr;
				newr = oldr - q * newr;
			}

			if (r > int_type::ONE())
				return int_type::ZERO();

			if (t < int_type::ZERO())
				t = t + n;

			return t;
		}

		// Modulo using Barret reduction
		// https://en.wikipedia.org/wiki/Barrett_reduction
		// https://www.nayuki.io/page/barrett-reduction-algorithm
		[[nodiscard]] static int_type mod_p(int_type x)
		{
			static const int_type k = params::p.bits();
			static const int_type p_sqr = params::p * params::p;
			static const int_type k2 = k << 1;
			static const int_type r = (int_type::ONE() << k2) / params::p;

			bool negative = false;
			if (x.sign() < 0)
			{
				negative = true;
				x = -x;
			}

			if (x >= p_sqr)
				return x % params::p;

			int_type t = x - ((x * r) >> k2) * params::p;

			if (t >= params::p)
				t -= params::p;

			if (negative)
				t = params::p - t;

			return t;
		}

	private:
		// Convert point from affine to jacobian coordinates
		// https://crypto.stackexchange.com/questions/19598/how-can-convert-affine-to-jacobian-coordinates
		[[nodiscard]] static jacobian_point_type point_affine_to_jacobian(const affine_point_type& p) noexcept
		{
			if (p == AFFINE_INFINITY)
				return JACOBIAN_INFINITY;

			return { std::get<0>(p), std::get<1>(p), int_type::ONE() };
		}

		// Convert point from jacobian to affine coordinates
		// https://crypto.stackexchange.com/questions/19598/how-can-convert-affine-to-jacobian-coordinates
		[[nodiscard]] static affine_point_type point_jacobian_to_affine(const jacobian_point_type& p) noexcept
		{
			if (p == JACOBIAN_INFINITY)
				return AFFINE_INFINITY;

			int_type z_sqr = mod_p(std::get<2>(p) * std::get<2>(p));

			return {
				mod_p(std::get<0>(p) * inverse(z_sqr, params::p)),
				mod_p(std::get<1>(p) * inverse(mod_p(z_sqr * std::get<2>(p)), params::p))
			};
		}

	private:
		// Prime curve affine point addition
		// https://en.wikibooks.org/wiki/Cryptography/Prime_Curve/Affine_Coordinates
		[[nodiscard]] static affine_point_type point_add(const affine_point_type& p, const affine_point_type& q) noexcept
		{
			if (p == AFFINE_INFINITY)
				return q;

			if (q == AFFINE_INFINITY)
				return p;

			if (std::get<0>(p) == std::get<0>(q))
			{
				if (std::get<1>(p) != std::get<1>(q))
					return AFFINE_INFINITY;

				return point_double(p);
			}

			int_type lambda;
			lambda = (std::get<1>(q) - std::get<1>(p)) % params::p;
			lambda = (lambda * inverse((std::get<0>(q) - std::get<0>(p)) % params::p, params::p)) % params::p;

			int_type x = (lambda * lambda - std::get<0>(p) - std::get<0>(q)) % params::p;
			int_type y = (lambda * (std::get<0>(p) - x) - std::get<1>(p)) % params::p;

			return { x, y };
		}

		// Prime curve affine point doubling
		// https://en.wikibooks.org/wiki/Cryptography/Prime_Curve/Affine_Coordinates
		[[nodiscard]] static affine_point_type point_double(const affine_point_type& p) noexcept
		{
			if (std::get<1>(p).is_zero())
				return AFFINE_INFINITY;

			int_type lambda;
			lambda = (std::get<0>(p) * std::get<0>(p)) % params::p;
			lambda = ((lambda << int_type::ONE()) + lambda + params::a) % params::p;
			lambda = (lambda * inverse(std::get<1>(p) << int_type::ONE(), params::p)) % params::p;

			int_type x = (lambda * lambda - (std::get<0>(p) << int_type::ONE())) % params::p;
			int_type y = (lambda * (std::get<0>(p) - x) - std::get<1>(p)) % params::p;

			return { x, y };
		}

		// Prime curve affine point multiplication by scalar using the Montgomery ladder method
		// https://en.wikipedia.org/wiki/Elliptic_curve_point_multiplication
		[[nodiscard]] static affine_point_type point_multiply(int_type d, const affine_point_type& p) noexcept
		{
			affine_point_type r0 = AFFINE_INFINITY;
			affine_point_type r1 = p;
			int32_t m = static_cast<int32_t>(d.bits());

			for (int32_t i = m - 1; i >= 0; i--)
			{
				if ((d.data()[i / 8] >> (i % 8)) & 1)
				{
					r0 = point_add(r0, r1);
					r1 = point_double(r1);
				}
				else
				{
					r1 = point_add(r0, r1);
					r0 = point_double(r0);
				}
			}

			return r0;
		}

	private:
		// Prime curve jacobian point addition
		// https://en.wikibooks.org/wiki/Cryptography/Prime_Curve/Jacobian_Coordinates
		[[nodiscard]] static jacobian_point_type point_add(const jacobian_point_type& p, const jacobian_point_type& q) noexcept
		{
			if (p == JACOBIAN_INFINITY)
				return q;

			if (q == JACOBIAN_INFINITY)
				return p;

			int_type qz_sqr = mod_p(std::get<2>(q) * std::get<2>(q)); // Z2^2
			int_type u1 = mod_p(std::get<0>(p) * qz_sqr); // U1 = X1 * Z2^2

			int_type pz_sqr = mod_p(std::get<2>(p) * std::get<2>(p)); // Z1^2
			int_type u2 = mod_p(std::get<0>(q) * pz_sqr); // U2 = X2 * Z1^2

			int_type s1 = mod_p(mod_p(std::get<1>(p) * qz_sqr) * std::get<2>(q)); // S1 = Y1 * Z2^3
			int_type s2 = mod_p(mod_p(std::get<1>(q) * pz_sqr) * std::get<2>(p)); // S2 = Y2 * Z1^3

			if (u1 == u2)
			{
				if (s1 != s2)
					return JACOBIAN_INFINITY;
				
				return point_double(p);
			}

			int_type h = mod_p(u2 - u1); // H = U2 - U1
			int_type r = mod_p(s2 - s1); // R = S2 - S1

			int_type h_sqr = mod_p(h * h); // H^2
			int_type h_cb = mod_p(h_sqr * h); // H^3
			
			int_type x = mod_p(mod_p(r * r) - h_cb - mod_p(mod_p(u1 << 1) * h_sqr)); // X3 = R^2 - H^3 - 2 * U1 * H^2
			int_type y = mod_p(mod_p(r * mod_p(mod_p(u1 * h_sqr - x))) - mod_p(s1 * h_cb)); // Y3 = R * (U1 * H^2 - X3) - S1 * H^3
			int_type z = mod_p(mod_p(h * std::get<2>(p)) * std::get<2>(q)); // Z3 = H * Z1 * Z2

			return { x, y, z };
		}

		// Prime curve jacobian point doubling
		// https://en.wikibooks.org/wiki/Cryptography/Prime_Curve/Jacobian_Coordinates
		[[nodiscard]] static jacobian_point_type point_double(const jacobian_point_type& p) noexcept
		{
			if (std::get<1>(p).is_zero())
				return JACOBIAN_INFINITY;

			int_type s = mod_p(mod_p(std::get<0>(p) * mod_p(std::get<1>(p) * std::get<1>(p))) << 2); // S = 4 * X * Y^2

			int_type z_qd = mod_p(std::get<2>(p) * std::get<2>(p)); // Z^4
			z_qd = mod_p(z_qd * z_qd);

			int_type m = mod_p(mod_p(mod_p(std::get<0>(p) * std::get<0>(p)) * 3) + mod_p(params::a * z_qd)); // M = 3 * X^2 + a * Z^4

			int_type x = mod_p(mod_p(m * m) - mod_p(s << 1)); // X' = M^2 - 2 * S

			int_type y_qd = mod_p(std::get<1>(p) * std::get<1>(p)); // Y^4
			y_qd = mod_p(y_qd * y_qd);

			int_type y = mod_p(mod_p(m * (s - x)) - mod_p(y_qd << 3)); // Y' = M * (S - X') - 8 * Y^4

			int_type z = mod_p(mod_p(std::get<1>(p) * std::get<2>(p)) << 1); // Z' = 2 * Y * Z
			
			return { x, y, z };
		}

		// Prime curve jacobian point multiplication by scalar
		// https://en.wikibooks.org/wiki/Cryptography/Prime_Curve/Jacobian_Coordinates
		[[nodiscard]] static jacobian_point_type point_multiply(const int_type& d, const jacobian_point_type& p) noexcept
		{
			jacobian_point_type  r0 = JACOBIAN_INFINITY;
			jacobian_point_type  r1 = p;
			int32_t m = static_cast<int32_t>(d.bits());

			for (int32_t i = m - 1; i >= 0; i--)
			{
				if ((d >> i).data()[0] & 1)
				{
					r0 = point_add(r0, r1);
					r1 = point_double(r1);
				}
				else
				{
					r1 = point_add(r0, r1);
					r0 = point_double(r0);
				}
			}

			return r0;
		}

	public:
		[[nodiscard]] static key_pair_type generate_key_pair() noexcept
		{
			static jacobian_point_type JACOBIAN_GENERATOR = point_affine_to_jacobian(GENERATOR);
			static std::random_device rand;

			int_type d = params::n;

			for (size_t i = 0; i < d.size(); i++)
				d.data()[i] = static_cast<uint8_t>(rand() % 0x100U);

			d = (d % (params::n - int_type::ONE())) + int_type::ONE();

			jacobian_point_type jQ = point_multiply(d, JACOBIAN_GENERATOR);
			affine_point_type aQ = point_jacobian_to_affine(jQ);

			return { d, aQ };
		}

	public:
		// Signature generation algorithm
		// https://www.secg.org/sec1-v2.pdf
		[[nodiscard]] static signature_type sign(const private_key_type& d, const void* data, size_t size) noexcept
		{
			int_type r, s;

			uint8_t digest[hash_type::DIGEST_SIZE / 8];
			
			hash_type hash;
			hash.write(data, size);
			hash.digest(digest);
			
			int_type e = int_type(digest, hash_type::DIGEST_SIZE / 8);
			
			while (true)
			{
				const key_pair_type secret = generate_key_pair();

				const private_key_type& k = std::get<0>(secret);
				const public_key_type& R = std::get<1>(secret);

				r = std::get<0>(R) % params::n;

				if (r.is_zero())
					continue;

				s = (inverse(k, params::n) * (e + r * d)) % params::n;

				if (s.is_zero())
					continue;

				break;
			}

			return { r, s };
		}

		// Signature verification algorithm
		// https://www.secg.org/sec1-v2.pdf
		[[nodiscard]] static bool verify(const public_key_type& Q, const signature_type& S, const void* data, size_t size) noexcept
		{
			static jacobian_point_type JACOBIAN_GENERATOR = point_affine_to_jacobian(GENERATOR);

			const int_type& r = std::get<0>(S);
			const int_type& s = std::get<1>(S);

			if (r < int_type::ONE() || r > params::n - int_type::ONE())
				return false;

			if (s < int_type::ONE() || s > params::n - int_type::ONE())
				return false;

			uint8_t digest[hash_type::DIGEST_SIZE / 8];

			hash_type hash;
			hash.write(data, size);
			hash.digest(digest);

			int_type e = int_type(digest, hash_type::DIGEST_SIZE / 8);

			const int_type inv_s = inverse(s, params::n);
			int_type u1 = (e * inv_s) % params::n;
			int_type u2 = (r * inv_s) % params::n;

			jacobian_point_type U1 = point_multiply(u1, JACOBIAN_GENERATOR);
			jacobian_point_type U2 = point_multiply(u2, point_affine_to_jacobian(Q));
			affine_point_type R = point_jacobian_to_affine(point_add(U1, U2));

			if (R == AFFINE_INFINITY)
				return false;

			int_type v = std::get<0>(R) % params::n;

			return v == r;
		}
	};

	using ECDSA_secp256k1 = ECDSA<ec_secp256k1>;
}
