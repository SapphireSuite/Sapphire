// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/IRenderInstance.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

namespace Sa
{
	void IRenderInstance::Init()
	{
#if SA_RENDERING_API == SA_VULKAN

		VkRenderInstance::Init();

#endif
	}
	
	void IRenderInstance::UnInit()
	{
#if SA_RENDERING_API == SA_VULKAN

		VkRenderInstance::UnInit();

#endif
	}
}
