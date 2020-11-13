// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_CREATE_INFOS_GUARD

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Framework/Misc/ImageType.hpp>
#include <Rendering/Framework/Misc/Format.hpp>
#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	struct ImageBufferCreateInfos
	{
		uint32 imageFlags = 0u;
		ImageType imageType = ImageType::Image2D;

		Format format;
		Vec2ui extent;

		uint32 mipLevels = 1u;
		
		SampleBits sampling = SampleBits::Sample1Bit;
		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
		
		uint32 usage = 0u;

		VkImageAspectFlags aspectFlags = 0u;

		VkImageCreateInfo VkImageInfos() const noexcept;
		VkImageViewCreateInfo VkImageViewInfos(VkImage _image) const noexcept;
	};
}

#endif

#endif // GUARD
