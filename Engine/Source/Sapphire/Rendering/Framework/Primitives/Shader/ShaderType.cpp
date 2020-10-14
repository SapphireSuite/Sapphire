// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Shader/ShaderType.hpp>

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

	VkShaderStageFlags API_GetShaderFlags(uint8 _flags) noexcept
	{
		VkShaderStageFlags result = 0;

		if (_flags & static_cast<uint8>(ShaderType::Vertex))
			result |= VK_SHADER_STAGE_VERTEX_BIT;

		if (_flags & static_cast<uint8>(ShaderType::Fragment))
			result |= VK_SHADER_STAGE_FRAGMENT_BIT;

		if (_flags & static_cast<uint8>(ShaderType::Geometry))
			result |= VK_SHADER_STAGE_GEOMETRY_BIT;

		if (_flags & static_cast<uint8>(ShaderType::Compute))
			result |= VK_SHADER_STAGE_COMPUTE_BIT;

		return result;
	}

#endif
}
