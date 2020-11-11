// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SURFACE_RENDERPASS_INFOS_GUARD
#define SAPPHIRE_RENDERING_VK_SURFACE_RENDERPASS_INFOS_GUARD

#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	struct SurfaceRenderPassInfos
	{
		uint32 ID = 0u;
		std::vector<FrameBuffer> frameBuffers;
	};
}

#endif

#endif // GUARD
