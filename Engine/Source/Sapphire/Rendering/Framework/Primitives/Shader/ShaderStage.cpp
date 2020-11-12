// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Shader/ShaderStage.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkShaderStageFlagBits API_GetShaderStage(ShaderStage _stage) noexcept
	{
		switch (_stage)
		{
			case Sa::ShaderStage::Vertex:
				return VK_SHADER_STAGE_VERTEX_BIT;
			case Sa::ShaderStage::Fragment:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			case Sa::ShaderStage::Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			case Sa::ShaderStage::Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
			case Sa::ShaderStage::Unknown:
			default:
				SA_LOG("Vulkan API GetShaderStage: default or unknown used!", Warning, Rendering);
				return VkShaderStageFlagBits(0);
		}
	}

	VkShaderStageFlags API_GetShaderStageFlags(ShaderStage _type) noexcept
	{
		VkShaderStageFlags result = 0;

		if (static_cast<uint8>(_type) & static_cast<uint8>(ShaderStage::Vertex))
			result |= VK_SHADER_STAGE_VERTEX_BIT;

		if (static_cast<uint8>(_type) & static_cast<uint8>(ShaderStage::Fragment))
			result |= VK_SHADER_STAGE_FRAGMENT_BIT;

		if (static_cast<uint8>(_type) & static_cast<uint8>(ShaderStage::Geometry))
			result |= VK_SHADER_STAGE_GEOMETRY_BIT;

		if (static_cast<uint8>(_type) & static_cast<uint8>(ShaderStage::Compute))
			result |= VK_SHADER_STAGE_COMPUTE_BIT;

		return result;
	}

#endif
}
