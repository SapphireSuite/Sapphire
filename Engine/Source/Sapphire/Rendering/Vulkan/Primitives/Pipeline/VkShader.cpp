// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkShader.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	VkShader::operator VkShaderModule() const
	{
		return mHandle;
	}
}

#endif