// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

// TODO move it somewhere appropriate (include for std::max)
#include <algorithm>

namespace Sa
{
	ITexture* ITexture::CreateInstance(const IRenderInstance& _instance, const RawTexture& _rawTexture)
	{
#if SA_RENDERING_API == SA_VULKAN

		ITexture* result = new VkTexture;

#endif

		result->Create(_instance, _rawTexture);

		return result;
	}

	uint32 ITexture::ComputeMipMapLevels(uint32 _width, uint32 height)
	{
		// Source https://vulkan-tutorial.com/Generating_Mipmaps.

		return static_cast<uint32_t>(std::floor(std::log2(std::max(_width, height)))) + 1u;
	}
}
