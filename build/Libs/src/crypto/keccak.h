#pragma once

#include "bit/rotate.h"

#include "crypto/basic_sponge.h"

namespace rb::crypto
{
	void keccak_f1600_perm(uint8_t* state, size_t size) noexcept;

	template<size_t H>
	using shake128 = basic_sponge<1344, 256, 0x1F, H, &keccak_f1600_perm>;

	template<size_t H>
	using shake256 = basic_sponge<1088, 512, 0x1F, H, &keccak_f1600_perm>;

	using sha3_224 = basic_sponge<1152, 448, 0x06, 224, &keccak_f1600_perm>;
	using sha3_256 = basic_sponge<1088, 512, 0x06, 256, &keccak_f1600_perm>;
	using sha3_384 = basic_sponge<832, 768, 0x06, 384, &keccak_f1600_perm>;
	using sha3_512 = basic_sponge<576, 1024, 0x06, 512, &keccak_f1600_perm>;
}
