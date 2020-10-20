// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	ITexture* ITexture::CreateInstance(const IRenderInstance& _instance, const RawTexture& _rawTexture)
	{
		ITexture* result = nullptr;

#if SA_RENDERING_API == SA_VULKAN

		result = new VkTexture;

#endif

		SA_ASSERT(result, Nullptr, Rendering, L"Texture creation failed! API implemenation may be missing.");

		result->Create(_instance, _rawTexture);

		return result;
	}

	uint32 ITexture::ComputeMipMapLevels(uint32 _width, uint32 height)
	{
		// Source https://vulkan-tutorial.com/Generating_Mipmaps.

		return static_cast<uint32_t>(std::floor(std::log2(Maths::Max(_width, height)))) + 1u;
	}
}
