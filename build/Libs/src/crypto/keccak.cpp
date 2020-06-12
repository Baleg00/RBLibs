#include "pch.h"

#include "crypto/keccak.h"

#define read_lane(s, x, y) reinterpret_cast<uint64_t*>(s)[(x) + 5 * (y)]
#define write_lane(s, x, y, v) reinterpret_cast<uint64_t*>(s)[(x) + 5 * (y)] = (v)
#define xor_lane(s, x, y, d) reinterpret_cast<uint64_t*>(s)[(x) + 5 * (y)] ^= (d)

namespace rb::crypto
{
	int LFSR86540(uint8_t* LFSR)
	{
		int result = ((*LFSR) & 0x01) != 0;

		if (((*LFSR) & 0x80) != 0)
			(*LFSR) = ((*LFSR) << 1) ^ 0x71;
		else
			(*LFSR) <<= 1;

		return result;
	}

	void keccak_f1600_perm(uint8_t* state, size_t size) noexcept
	{
		uint8_t LFSRstate = 0x01;

		for (size_t r = 0; r < 24; r++)
		{
			{
				uint64_t c[5] = { 0 }, d = 0;

				for (size_t x = 0; x < 5; x++)
					c[x] = read_lane(state, x, 0) ^ read_lane(state, x, 1) ^
					read_lane(state, x, 2) ^ read_lane(state, x, 3) ^ read_lane(state, x, 4);

				for (size_t x = 0; x < 5; x++)
				{
					d = c[(x + 4) % 5] ^ bit::rotl(c[(x + 1) % 5], 1);

					for (size_t y = 0; y < 5; y++)
						xor_lane(state, x, y, d);
				}
			}

			{
				size_t x = 1, y = 0;
				uint64_t current = read_lane(state, x, y);

				for (size_t t = 0; t < 24; t++)
				{
					uint32_t r = ((t + 1) * (t + 2) / 2) % 64;
					uint32_t y_ = (2 * x + 3 * y) % 5;

					x = y;
					y = y_;

					uint64_t temp = read_lane(state, x, y);
					write_lane(state, x, y, bit::rotl(current, r));
					current = temp;
				}
			}

			{
				uint64_t temp[5] = { 0 };

				for (size_t y = 0; y < 5; y++)
				{
					for (size_t x = 0; x < 5; x++)
						temp[x] = read_lane(state, x, y);

					for (size_t x = 0; x < 5; x++)
						write_lane(state, x, y, temp[x] ^ ((~temp[(x + 1) % 5]) & temp[(x + 2) % 5]));
				}
			}

			{
				for (size_t j = 0; j < 7; j++) {
					uint32_t bit_pos = (1 << j) - 1;

					if (LFSR86540(&LFSRstate))
						xor_lane(state, 0, 0, 1ull << bit_pos);
				}
			}
		}
	}
}
