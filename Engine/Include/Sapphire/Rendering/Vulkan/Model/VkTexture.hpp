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
		// TODO: PRIVATE.
	public:
		VkImage mImage;
		VkImageView mImageView;

		VkSampler mSampler;

		VkDeviceMemory mImageMemory = VK_NULL_HANDLE;

		// TODO: REMOVE LATER.
		static VkTexture TEST;

		void Create(const IRenderInstance& _instance, const std::string& _fileName) override final;
		void Destroy(const IRenderInstance& _instance) override final;
	};
}

#endif

#endif // GUARD