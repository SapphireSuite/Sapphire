// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_TEXTURE_GUARD
#define SAPPHIRE_RENDERING_VK_TEXTURE_GUARD

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <Rendering/Vulkan/Buffer/VkImageBuffer.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkTexture : public ITexture
	{
		VkImageBuffer mBuffer;

		VkSampler mSampler = VK_NULL_HANDLE;

	public:
		void Create(const IRenderInstance& _instance, const RawTexture& _rawTexture) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		VkDescriptorImageInfo CreateDescriptorImageInfo() const noexcept;
		static VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem = 0u) noexcept;
	};
}

#endif

#endif // GUARD