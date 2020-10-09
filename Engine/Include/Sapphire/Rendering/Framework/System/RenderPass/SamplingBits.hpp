// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SAMPLING_BITS_GUARD
#define SAPPHIRE_RENDERING_SAMPLING_BITS_GUARD

namespace Sa
{
	enum class SamplingBits
	{
		/// no multisampling.
		Sample1Bit = 1 << 0,

		/// 2 bits multisampling.
		Sample2Bits = 1 << 1,

		/// 4 bits multisampling.
		Sample4Bits = 1 << 2,

		/// 8 bits multisampling.
		Sample8Bits = 1 << 3,

		/// 16 bits multisampling.
		Sample16Bits = 1 << 4,

		/// 32 bits multisampling.
		Sample32Bits = 1 << 5,

		/// 64 bits multisampling.
		Sample64Bits = 1 << 6
	};
}

#endif // GUARD
