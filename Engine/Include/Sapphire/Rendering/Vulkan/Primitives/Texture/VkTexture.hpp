// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_TEXTURE_GUARD
#define SAPPHIRE_RENDERING_VK_TEXTURE_GUARD

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <Rendering/Vulkan/Buffers/VkImageBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class SA_ENGINE_API Texture : public ITexture
	{
		ImageBuffer mBuffer;

		VkSampler mSampler = VK_NULL_HANDLE;

	public:
		void Create(const IRenderInstance& _instance, const RawTexture& _rawTexture) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		VkDescriptorImageInfo CreateDescriptorImageInfo() const noexcept;
	};
}

#endif

#endif // GUARD