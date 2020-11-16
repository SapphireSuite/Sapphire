// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/RawCubemap.hpp>

namespace Sa
{
	const uint32 RawCubemap::maxRoughLevel = 5u;

	RawCubemap::RawCubemap() noexcept
	{
		format = Format::sRGBA_32; // default format.
	}

	uint64 RawCubemap::GetMapSize() const noexcept
	{
		return GetMainSize() * 6u;
	}

	uint64 RawCubemap::GetTotalMapSize() const noexcept
	{
		return GetTotalSize() * 6u;
	}
}
