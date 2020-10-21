// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/RawTexture.hpp>

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>

namespace Sa
{
	uint32 RawTexture::GetMainSize(bool _bApplyType) const noexcept
	{
		uint32 result = width * height * static_cast<uint32>(channel);

		if (_bApplyType)
		{
			if (type == TextureType::Cubemap)
				result *= 6u;
		}

		return result;
	}

	uint32 RawTexture::GetTotalSize(bool _bApplyType) const noexcept
	{
		uint32 result = Mipmap::ComputeTotalSize(width, height, mipLevels) * static_cast<uint32>(channel);

		if (_bApplyType)
		{
			if (type == TextureType::Cubemap)
				result *= 6u;
		}

		return result;
	}
}
