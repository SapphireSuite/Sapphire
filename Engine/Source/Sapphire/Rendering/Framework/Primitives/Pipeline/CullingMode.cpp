// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/CullingMode.hpp>

#include <Collections/Debug>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	VkCullModeFlags API_GetCullingMode(CullingMode _mode) noexcept
	{
		switch (_mode)
		{
			case Sa::CullingMode::None:
				return VK_CULL_MODE_NONE;
			case Sa::CullingMode::Front:
				return VK_CULL_MODE_FRONT_BIT;
			case Sa::CullingMode::Back:
				return VK_CULL_MODE_BACK_BIT;
			default:
				SA_LOG("Vulkan API GetCullingMode: default used!", Warning, Rendering);
				return VK_CULL_MODE_NONE;
		}
	}
}

#endif
