// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/FrontFaceMode.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkFrontFace API_GetFrontFaceMode(FrontFaceMode _mode) noexcept
	{
		switch (_mode)
		{
			case Sa::FrontFaceMode::Clockwise:
				return VK_FRONT_FACE_CLOCKWISE;
			case Sa::FrontFaceMode::CounterClockwise:
				return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			default:
				SA_LOG("Vulkan API GetFrontFaceMode: default used!", Warning, Rendering);
				return VK_FRONT_FACE_CLOCKWISE;
		}
	}

#endif

}