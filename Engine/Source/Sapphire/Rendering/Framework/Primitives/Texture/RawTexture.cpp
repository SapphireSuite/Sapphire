// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/RawTexture.hpp>

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>

namespace Sa
{
	uint64 RawTexture::GetMainSize() const noexcept
	{
		return extent.x * extent.y * API_GetChannelNum(format);
	}

	uint64 RawTexture::GetTotalSize() const noexcept
	{
		return Mipmap::ComputeTotalSize(extent, mipLevels) * API_GetChannelNum(format);
	}
}
