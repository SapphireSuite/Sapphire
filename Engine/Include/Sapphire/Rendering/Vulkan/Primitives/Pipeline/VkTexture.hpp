// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_TEXTURE_GUARD
#define SAPPHIRE_RENDERING_VK_TEXTURE_GUARD

#include <Rendering/Framework/Primitives/Pipeline/ITexture.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkTexture : public ITexture
	{
		VkImage mImage = VK_NULL_HANDLE;
		VkImageView mImageView = VK_NULL_HANDLE;

		VkSampler mSampler = VK_NULL_HANDLE;

		VkDeviceMemory mImageMemory = VK_NULL_HANDLE;

	public:
		void Create(const IRenderInstance& _instance, const void* _data, uint32 _width, uint32 _height) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		VkDescriptorImageInfo CreateDescriptorImageInfo() const noexcept;
		VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem = 0u) const noexcept;
	};
}

#endif

#endif // GUARD