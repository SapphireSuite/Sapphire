// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_TEXTURE_GUARD
#define SAPPHIRE_RENDERING_VK_TEXTURE_GUARD

#include <Rendering/Framework/Model/ITexture.hpp>

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
		// TODO: Remoce SA_ENGINE_API.
		SA_ENGINE_API void Create(const IRenderInstance& _instance, const std::string& _fileName) override final;
		// TODO: Remoce SA_ENGINE_API.
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance) override final;

		VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding) const noexcept;
	};
}

#endif

#endif // GUARD