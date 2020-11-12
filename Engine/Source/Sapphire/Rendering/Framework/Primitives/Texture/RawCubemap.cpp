// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/RawCubemap.hpp>

namespace Sa
{
	const uint32 RawCubemap::maxRoughLevel = 5u;

	uint64 RawCubemap::GetMapSize() const noexcept
	{
		return GetMainSize() * 6u;
	}

	uint64 RawCubemap::GetTotalMapSize() const noexcept
	{
		return GetTotalSize() * 6u;
	}
}
