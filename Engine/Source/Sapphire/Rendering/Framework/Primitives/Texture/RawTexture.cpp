// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/RawTexture.hpp>

namespace Sa
{
	uint32 RawTexture::GetSize() const noexcept
	{
		uint32 result = width * height * static_cast<uint32>(channel);

		if (type == TextureType::Cubemap)
			result *= 6u;

		return result;
	}
}
