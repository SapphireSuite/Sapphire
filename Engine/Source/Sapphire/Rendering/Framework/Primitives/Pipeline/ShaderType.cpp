// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/ShaderType.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkShaderStageFlagBits API_GetShaderType(ShaderType _type) noexcept
	{
		switch (_type)
		{
			case Sa::ShaderType::Vertex:
				return VK_SHADER_STAGE_VERTEX_BIT;
			case Sa::ShaderType::Fragment:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			case Sa::ShaderType::Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			case Sa::ShaderType::Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
			case Sa::ShaderType::Unknown:
			default:
				SA_LOG("Vulkan API GetShaderType: default or unknown used!", Warning, Rendering);
				return VkShaderStageFlagBits(0);
		}
	}

#endif
}
