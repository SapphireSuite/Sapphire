// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_CUBEMAP_GUARD
#define SAPPHIRE_RENDERING_VK_CUBEMAP_GUARD

#include <Rendering/Framework/Primitives/Texture/ICubemap.hpp>

#include <Rendering/Vulkan/Buffers/VkImageBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Cubemap : public ICubemap
	{
		ImageBuffer mBuffer;
		ImageBuffer mIrradianceBuffer;

		VkSampler mSampler = VK_NULL_HANDLE;

	public:
		void Create(const IRenderInstance& _instance, const RawCubemap& _rawCubemap) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		VkDescriptorImageInfo CreateDescriptorImageInfo() const noexcept;
		VkDescriptorImageInfo CreateIrradianceDescriptorImageInfo() const noexcept;
		static VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem = 0u) noexcept;
	};
}

#endif

#endif // GUARD