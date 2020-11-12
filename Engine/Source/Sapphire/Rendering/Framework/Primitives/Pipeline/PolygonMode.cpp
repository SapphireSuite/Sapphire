// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/PolygonMode.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkPolygonMode API_GetPolygonMode(PolygonMode _mode) noexcept
	{
		switch (_mode)
		{
			case Sa::PolygonMode::Point:
				return VK_POLYGON_MODE_POINT;
			case Sa::PolygonMode::Line:
				return VK_POLYGON_MODE_LINE;
			case Sa::PolygonMode::Fill:
				return VK_POLYGON_MODE_FILL;
			default:
				SA_LOG("Vulkan API GetPolygonMode: default used!", Warning, Rendering);
				return VK_POLYGON_MODE_FILL;
		}
	}

#endif

}